#include "ModListSource.hpp"
#include <arc/future/Join.hpp>

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
    if (type == InstalledModListType::OnlyUpdates) {
        return src.hasUpdates().update || src.hasUpdates().deprecation;
    }
    // If only errors requested, only show mods with errors (duh)
    if (type == InstalledModListType::OnlyOutdated) {
        return src.asMod() && src.asMod()->targetsOutdatedVersion().has_value();
    }
    if (type == InstalledModListType::OnlyErrors) {
        return src.asMod() && src.asMod()->failedToLoad();
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
    // todo: favorites
    return addToList;
}
bool InstalledModsQuery::isDefault() const {
    return LocalModsQueryBase::isDefault() && !enabledOnly.value_or(false);
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
        content.mods.push_back(ModSource(mod));
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
        
        filterModsWithLocalQuery(content, m_query);
        co_return Ok(std::move(content));
    }
    // Otherwise simply construct the result right away
    else {
        filterModsWithLocalQuery(content, m_query);
        co_return Ok(std::move(content));
    }
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
    return m_type == InstalledModListType::All ||
        m_type == InstalledModListType::OnlyErrors ||
        m_type == InstalledModListType::OnlyOutdated;
}
