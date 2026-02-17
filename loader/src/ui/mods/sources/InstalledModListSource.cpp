#include "ModListSource.hpp"
#include <arc/future/Join.hpp>
#include <server/DownloadManager.hpp>

void InstalledModsQuery::filter(ModListSource::ProvidedMods& mods) {
    std::vector<std::pair<ModSource, double>> filtered;

    // Filter installed mods based on query
    // TODO: maybe skip fuzzy matching altogether if query is empty?
    for (auto& src : mods.mods) {
        if (std::holds_alternative<SpecialModListItemSource>(src)) {
            continue;
        }
        auto mod = std::get<ModSource>(std::move(src));

        double weighted = 0;
        bool addToList = true;
        // Do any checks additional this query has to start off with
        if (!this->preCheck(mod)) {
            addToList = false;
        }
        // If some tags are provided, only return mods that match
        if (addToList && this->tags.size()) {
            auto compare = mod.getMetadata().getTags();
            for (auto& tag : this->tags) {
                if (!compare.contains(tag)) {
                    addToList = false;
                }
            }
        }
        // Don't bother with unnecessary fuzzy match calculations if this mod isn't going to be added anyway
        if (addToList) {
            addToList = this->queryCheck(mod, weighted);
        }
        if (addToList) {
            filtered.push_back({ std::move(mod), weighted });
        }
    }

    // Sort list based on score
    std::sort(filtered.begin(), filtered.end(), [](auto& a, auto& b) {
        // Sort primarily by score
        if (a.second != b.second) {
            return a.second > b.second;
        }
        // Deprecated mods are first by default
        auto aIsDeprecated = a.first.hasUpdates().deprecation.has_value();
        auto bIsDeprecated = b.first.hasUpdates().deprecation.has_value();
        if (aIsDeprecated != bIsDeprecated) {
            return aIsDeprecated;
        }
        // Outdated mods are always last by default
        auto aIsOutdated = a.first.getMetadata().checkTargetVersions().isErr();
        auto bIsOutdated = b.first.getMetadata().checkTargetVersions().isErr();
        if (aIsOutdated != bIsOutdated) {
            return !aIsOutdated;
        }
        // Otherwise sort alphabetically
        return utils::string::caseInsensitiveCompare(
            a.first.getMetadata().getName(),
            b.first.getMetadata().getName()
        ) == std::strong_ordering::less;
    });

    mods.mods.clear();
    // Pick out only the mods in the page and page size specified in the query
    for (
        size_t i = this->page * this->pageSize;
        i < filtered.size() && i < (this->page + 1) * this->pageSize;
        i += 1
    ) {
        mods.mods.push_back(std::move(filtered.at(i).first));
    }

    mods.totalModCount = filtered.size();
}
bool InstalledModsQuery::preCheck(ModSource const& src) const {
    // Invalid .geode files are collected up into one entry instead of 
    // flooding the mods list
    // if (
    //     src.asMod() &&
    //     src.asMod()->failedToLoad() &&
    //     src.asMod()->failedToLoad()->type == LoadProblem::Type::InvalidGeodeFile
    // ) {
    //     return false;
    // }
    // If we only want mods with updates, then only give mods with updates
    // NOTE: The caller of filterModsWithQuery() should have ensured that
    // `src.checkUpdates()` has been called and has finished
    if (this->type == InstalledModListType::OnlyUpdates) {
        return src.hasUpdates().update || src.hasUpdates().deprecation;
    }
    // If only errors requested, only show mods with errors (duh)
    if (this->type == InstalledModListType::OnlyOutdated) {
        return src.asMod() && src.asMod()->targetsOutdatedVersion().has_value();
    }
    if (this->type == InstalledModListType::OnlyErrors) {
        return src.asMod() && src.asMod()->failedToLoad();
    }
    // If we're sorting by recently updated, don't show mods that haven't been 
    // updated since the addition of this feature in v5
    if (this->sort == InstalledModListSort::RecentlyUpdated) {
        auto const& mods = server::ModDownloadManager::get()->getRecentlyUpdatedMods();
        return std::ranges::find_if(mods, [&src](auto& u) { return u.modID == src.getID(); }) != mods.end();
    }
    return true;
}
bool InstalledModsQuery::queryCheck(ModSource const& src, double& weighted) const {
    bool addToList = true;
    if (enabledOnly) {
        addToList = src.asMod()->isLoaded() == *enabledOnly;
    }
    if (query) {
        addToList = modFuzzyMatch(src.asMod()->getMetadata(), *query, weighted);
    }
    // Loader gets boost to ensure it's normally always top of the list
    if (addToList && src.asMod()->isInternal()) {
        weighted += 5;
    }
    if (addToList && enabledFirst && src.asMod()->isLoaded()) {
        weighted += 3;
    }
    // Sort recently updated mods by order of recent update
    if (this->sort == InstalledModListSort::RecentlyUpdated) {
        auto const& mods = server::ModDownloadManager::get()->getRecentlyUpdatedMods();

        // This substracts from `mods.end()` instead of `mods.begin()` so that 
        // the result is lower index -> higher score (which is what we want)
        weighted -= (std::ranges::find_if(
            mods, [&src](auto& u) { return u.modID == src.getID(); }
        ) - mods.end()) * .1f;
    }
    // todo: favorites
    return addToList;
}
bool InstalledModsQuery::isDefault() const {
    return !this->query.has_value() && this->tags.empty() && !this->enabledOnly.value_or(false);
}
matjson::Value InstalledModsQuery::dumpFilters() const {
    matjson::Value out;
    out["enabledOnly"] = this->enabledOnly.value_or(false);
    out["enabledFirst"] = this->enabledFirst.value_or(false);
    return out;
}

InstalledModListSource::InstalledModListSource(InstalledModListType type)
  : m_type(type)
{
    this->resetQuery();
}

InstalledModListSource* InstalledModListSource::get(InstalledModListType type) {
    switch (type) {
        default:
        case InstalledModListType::All: {
            static auto inst = new InstalledModListSource(InstalledModListType::All);
            return inst;
        } break;

        case InstalledModListType::OnlyUpdates: {
            static auto inst = new InstalledModListSource(InstalledModListType::OnlyUpdates);
            return inst;
        } break;

        case InstalledModListType::OnlyErrors: {
            static auto inst = new InstalledModListSource(InstalledModListType::OnlyErrors);
            return inst;
        } break;

        case InstalledModListType::OnlyOutdated: {
            static auto inst = new InstalledModListSource(InstalledModListType::OnlyOutdated);
            return inst;
        } break;
    }
}

void InstalledModListSource::resetQuery() {
    m_query = InstalledModsQuery {
        .type = m_type,
    };

    // load the enabled only / enable first values if applicable
    auto value = Mod::get()->getSavedValue<matjson::Value>("mod-list-installed-filters");

    m_query.enabledOnly = value["enabledOnly"].asBool().ok();
    m_query.enabledFirst = value["enabledFirst"].asBool().ok();

    // plenty of code checks for the option not being None instead of the inner value :/
    if (!m_query.enabledOnly.value_or(false)) {
        m_query.enabledOnly = std::nullopt;
    }

    if (!m_query.enabledFirst.value_or(false)) {
        m_query.enabledFirst = std::nullopt;
    }
}

InstalledModListSource::ProviderTask InstalledModListSource::fetchPage(size_t page, bool forceUpdate) {
    m_query.page = page;
    m_query.pageSize = m_pageSize;

    // Infinite mods list option
    if (Mod::get()->getSettingValue<bool>("infinite-local-mods-list")) {
        m_query.page = 0;
        m_query.pageSize = Loader::get()->getAllMods().size();
    }

    auto content = ModListSource::ProvidedMods();

    /*
    if (m_query.type != InstalledModListType::OnlyUpdates) {
        size_t invalidFileCount = 0;
        for (auto const& problem : Loader::get()->getLoadProblems()) {
            if (problem.type == LoadProblem::Type::InvalidGeodeFile) {
                invalidFileCount += 1;
            }
        }
        if (invalidFileCount > 0) {
            content.mods.push_back(SpecialModListItemSource {
                .title = fmt::format("({} invalid mods)", invalidFileCount),
                .onDetails = [=]() {
                    createQuickPopup(
                        "Invalid mods",
                        fmt::format(
                            "You have <cy>{}</c> invalid <ca>.geode</c> files in your "
                            "mods directory. These are probably due to a recent GD "
                            "update (or because you are the developer of the mods). "
                            "<cr>Would you like to delete these invalid files?</c>",
                            invalidFileCount
                        ),
                        "OK", "Delete",
                        [](auto, bool btn2) mutable {
                            if (btn2) {
                                // todo: like delete them n shit...
                            }
                        }
                    );
                }
            });
        }
    }
    */
    for (auto& mod : Loader::get()->getAllMods()) {
        content.mods.push_back(ModSource(mod, this));
    }
    // If we're only checking mods that have updates, we first have to run
    // update checks every mod...
    if (m_query.type == InstalledModListType::OnlyUpdates && content.mods.size()) {
        using UpdateTask = server::ServerFuture<server::ServerModUpdateOneCheck>;
        std::vector<UpdateTask> tasks;
        for (auto& src : content.mods) {
            if (auto mod = std::get_if<ModSource>(&src)) {
                tasks.push_back(mod->checkUpdates());
            }
        }

        co_await arc::joinAll(std::move(tasks));
        
        m_query.filter(content);
        co_return Ok(std::move(content));
    }
    // Otherwise simply construct the result right away
    else {
        m_query.filter(content);
        co_return Ok(std::move(content));
    }
}

std::string InstalledModListSource::getNoModsFoundError() const {
    // Epic hardcoded message for this one very specific thing that will save 
    // us from having to deal with like 3 #help posts
    if (
        !this->m_query.query.has_value() && 
        this->m_query.sort == InstalledModListSort::RecentlyUpdated
    ) {
        return "No mods found :(\n"
            "(You probably haven't updated\n"
            "or installed any since the\n"
            "release of Geode for GD 2.2081)";
    }
    return ModListSource::getNoModsFoundError();
}

void InstalledModListSource::setSearchQuery(std::string query) {
    m_query.query = query.size() ? std::optional(std::move(query)) : std::nullopt;
}

std::unordered_set<std::string> InstalledModListSource::getModTags() const {
    return m_query.tags;
}
void InstalledModListSource::setModTags(std::unordered_set<std::string> const& tags) {
    m_query.tags = tags;
    this->clearCache();
}

std::vector<std::pair<size_t, std::string>> InstalledModListSource::getSortingOptions() {
    return {
        { static_cast<size_t>(InstalledModListSort::Alphabetical), "A-Z" },
        { static_cast<size_t>(InstalledModListSort::RecentlyUpdated), "Recently Installed" },
    };
}
size_t InstalledModListSource::getSort() const {
    return static_cast<size_t>(this->getQuery().sort);
}
void InstalledModListSource::setSort(size_t sortingOptionIndex) {
    this->getQueryMut()->sort = static_cast<InstalledModListSort>(sortingOptionIndex);
}

InstalledModsQuery const& InstalledModListSource::getQuery() const {
    return m_query;
}
InvalidateQueryAfter<InstalledModsQuery> InstalledModListSource::getQueryMut() {
    return InvalidateQueryAfter(m_query, this);
}
bool InstalledModListSource::isDefaultQuery() const {
    return m_query.isDefault();
}

bool InstalledModListSource::isLocalModsOnly() const {
    // Uhh I wonder why this is here but uhhh idk why...
    return m_type != InstalledModListType::OnlyUpdates;
}
