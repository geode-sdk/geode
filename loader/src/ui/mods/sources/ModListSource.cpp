#include "ModListSource.hpp"

#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include <Geode/external/fts/fts_fuzzy_match.h>

static constexpr size_t PER_PAGE = 10;

static size_t ceildiv(size_t a, size_t b) {
    // https://stackoverflow.com/questions/2745074/fast-ceiling-of-an-integer-division-in-c-c
    return a / b + (a % b != 0);
}

static bool weightedFuzzyMatch(std::string const& str, std::string const& kw, double weight, double& out) {
    int score;
    if (fts::fuzzy_match(kw.c_str(), str.c_str(), score)) {
        out = std::max(out, score * weight);
        return true;
    }
    return false;
}

static void filterModsWithQuery(InstalledModListSource::ProvidedMods& mods, InstalledModsQuery const& query) {
    std::vector<std::pair<Mod*, double>> filtered;

    // Filter installed mods based on query
    for (auto& src : mods.mods) {
        auto mod = src.asMod();
        double weighted = 0;
        bool addToList = true;
        // If we only want mods with updates, then only give mods with updates
        // NOTE: The caller of filterModsWithQuery() should have ensured that 
        // `src.checkUpdates()` has been called and has finished
        if (auto updates = src.hasUpdates(); query.onlyUpdates && !(updates && updates->hasUpdateForInstalledMod())) {
            addToList = false;
        }
        // If some tags are provided, only return mods that match
        if (addToList && query.tags.size()) {
            auto compare = mod->getMetadata().getTags();
            for (auto& tag : query.tags) {
                if (!compare.contains(tag)) {
                    addToList = false;
                }
            }
        }
        // Don't bother with unnecessary fuzzy match calculations if this mod isn't going to be added anyway
        if (addToList && query.query) {
            // By default don't add anything
            addToList = false;
            addToList |= weightedFuzzyMatch(mod->getName(), *query.query, 1, weighted);
            addToList |= weightedFuzzyMatch(mod->getID(),   *query.query, 0.5, weighted);
            for (auto& dev : mod->getDevelopers()) {
                addToList |= weightedFuzzyMatch(dev, *query.query, 0.25, weighted);
            }
            if (auto details = mod->getDetails()) {
                addToList |= weightedFuzzyMatch(*details, *query.query, 0.005, weighted);
            }
            if (auto desc = mod->getDescription()) {
                addToList |= weightedFuzzyMatch(*desc, *query.query, 0.02, weighted);
            }
            if (weighted < 2) {
                addToList = false;
            }
        }
        // Loader gets boost to ensure it's normally always top of the list
        if (addToList && mod->getID() == "geode.loader") {
            weighted += 5;
        }
        if (addToList) {
            filtered.push_back({ mod, weighted });
        }
    }

    // Sort list based on score
    std::sort(filtered.begin(), filtered.end(), [](auto a, auto b) {
        // Sort primarily by score
        if (a.second != b.second) {
            return a.second > b.second;
        }
        // Sort secondarily alphabetically
        return a.first->getName() < b.first->getName();
    });

    mods.mods.clear();
    // Pick out only the mods in the page and page size specified in the query
    for (
        size_t i = query.page * query.pageSize;
        i < filtered.size() && i < (query.page + 1) * query.pageSize;
        i += 1
    ) {
        mods.mods.push_back(filtered.at(i).first);
    }
    
    mods.totalModCount = filtered.size();
}

InvalidateCacheEvent::InvalidateCacheEvent(ModListSource* src) : source(src) {}

ListenerResult InvalidateCacheFilter::handle(MiniFunction<Callback> fn, InvalidateCacheEvent* event) {
    if (event->source == m_source) {
        fn(event);
    }
    return ListenerResult::Propagate;
}

InvalidateCacheFilter::InvalidateCacheFilter(ModListSource* src) : m_source(src) {}

typename ModListSource::PageLoadTask ModListSource::loadPage(size_t page, bool update) {
    if (!update && m_cachedPages.contains(page)) {
        return PageLoadTask::immediate(Ok(m_cachedPages.at(page)));
    }
    m_cachedPages.erase(page);
    return this->fetchPage(page, PER_PAGE).map(
        [this, page](Result<ProvidedMods, LoadPageError>* result) -> Result<Page, LoadPageError> {
            if (result->isOk()) {
                auto data = result->unwrap();
                if (data.totalModCount == 0 || data.mods.empty()) {
                    return Err(LoadPageError("No mods found :("));
                }
                auto pageData = Page();
                for (auto mod : std::move(data.mods)) {
                    pageData.push_back(ModItem::create(std::move(mod)));
                }
                m_cachedItemCount = data.totalModCount;
                m_cachedPages.insert({ page, pageData });
                return Ok(pageData);
            }
            else {
                return Err(result->unwrapErr());
            }
        }
    );
}

std::optional<size_t> ModListSource::getPageCount() const {
    return m_cachedItemCount ? std::optional(ceildiv(m_cachedItemCount.value(), PER_PAGE)) : std::nullopt;
}

std::optional<size_t> ModListSource::getItemCount() const {
    return m_cachedItemCount;
}

void ModListSource::reset() {
    this->clearCache();
    this->resetQuery();
}
void ModListSource::clearCache() {
    m_cachedPages.clear();
    m_cachedItemCount = std::nullopt;
    InvalidateCacheEvent(this).post();
}
void ModListSource::search(std::string const& query) {
    this->setSearchQuery(query);
    this->clearCache();
}

ModListSource::ModListSource() {}

InstalledModListSource::InstalledModListSource(bool onlyUpdates)
  : m_onlyUpdates(onlyUpdates)
{
    this->resetQuery();
}

InstalledModListSource* InstalledModListSource::get(bool onlyUpdates) {
    if (onlyUpdates) {
        static auto inst = new InstalledModListSource(true);
        return inst;
    }
    else {
        static auto inst = new InstalledModListSource(false);
        return inst;
    }
}

void InstalledModListSource::resetQuery() {
    m_query = InstalledModsQuery {
        .onlyUpdates = m_onlyUpdates,
    };
}

InstalledModListSource::ProviderTask InstalledModListSource::fetchPage(size_t page, size_t pageSize) {
    m_query.page = page;
    m_query.pageSize = pageSize;

    auto content = ModListSource::ProvidedMods();
    for (auto& mod : Loader::get()->getAllMods()) {
        content.mods.push_back(ModSource(mod));
    }
    // If we're only checking mods that have updates, we first have to run 
    // update checks every mod...
    if (m_query.onlyUpdates && content.mods.size()) {
        return ProviderTask::runWithCallback([content, query = m_query](auto finish, auto progress, auto hasBeenCancelled) {
            struct Waiting final {
                ModListSource::ProvidedMods content;
                std::atomic_size_t waitingFor;
                std::vector<Task<std::monostate>> waitingTasks;
            };
            auto waiting = std::make_shared<Waiting>();
            waiting->waitingFor = content.mods.size();
            waiting->content = std::move(content);
            waiting->waitingTasks.reserve(content.mods.size());
            for (auto& src : waiting->content.mods) {
                // Need to store the created task so it doesn't get destroyed
                waiting->waitingTasks.emplace_back(src.checkUpdates().map(
                    [waiting, finish, query](auto* result) {
                        waiting->waitingFor -= 1;
                        if (waiting->waitingFor == 0) {
                            filterModsWithQuery(waiting->content, query);
                            finish(Ok(std::move(waiting->content)));
                        }
                        return std::monostate();
                    }, 
                    [](auto*) { return std::monostate(); }
                ));
            }
        });
    }
    // Otherwise simply construct the result right away
    else {
        filterModsWithQuery(content, m_query);
        return ProviderTask::immediate(Ok(content));
    }
}

void InstalledModListSource::setSearchQuery(std::string const& query) {
    m_query.query = query.size() ? std::optional(query) : std::nullopt;
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

bool InstalledModListSource::wantsRestart() const {
    for (auto mod : Loader::get()->getAllMods()) {
        if (mod->getRequestedAction() != ModRequestedAction::None) {
            return true;
        }
    }
    return false;
}

void ServerModListSource::resetQuery() {
    switch (m_type) {
        case ServerModListType::Download: {
            m_query = server::ModsQuery {};
        } break;

        case ServerModListType::Featured: {
            m_query = server::ModsQuery {
                .featured = true,
            };
        } break;

        case ServerModListType::Trending: {
            m_query = server::ModsQuery {
                .sorting = server::ModsSort::RecentlyUpdated,
            };
        } break;

        case ServerModListType::Recent: {
            m_query = server::ModsQuery {
                .sorting = server::ModsSort::RecentlyPublished,
            };
        } break;
    }
}

ServerModListSource::ProviderTask ServerModListSource::fetchPage(size_t page, size_t pageSize) {
    m_query.page = page;
    m_query.pageSize = pageSize;
    return server::getMods(m_query).map(
        [](Result<server::ServerModsList, server::ServerError>* result) -> ProviderTask::Value {
            if (result->isOk()) {
                auto list = result->unwrap();
                auto content = ModListSource::ProvidedMods();
                for (auto&& mod : std::move(list.mods)) {
                    content.mods.push_back(ModSource(std::move(mod)));
                }
                content.totalModCount = list.totalModCount;
                return Ok(content);
            }
            return Err(LoadPageError("Error loading mods", result->unwrapErr().details));
        },
        [](auto* prog) {
            return prog->percentage;
        }
    );
}

ServerModListSource::ServerModListSource(ServerModListType type)
  : m_type(type)
{
    this->resetQuery();
}

ServerModListSource* ServerModListSource::get(ServerModListType type) {
    switch (type) {
        default:
        case ServerModListType::Download: {
            static auto inst = new ServerModListSource(ServerModListType::Download);
            return inst;
        } break;

        case ServerModListType::Featured: {
            static auto inst = new ServerModListSource(ServerModListType::Featured);
            return inst;
        } break;

        case ServerModListType::Trending: {
            static auto inst = new ServerModListSource(ServerModListType::Trending);
            return inst;
        } break;

        case ServerModListType::Recent: {
            static auto inst = new ServerModListSource(ServerModListType::Recent);
            return inst;
        } break;
    }
}

void ServerModListSource::setSearchQuery(std::string const& query) {
    m_query.query = query.size() ? std::optional(query) : std::nullopt;
}

std::unordered_set<std::string> ServerModListSource::getModTags() const {
    return m_query.tags;
}
void ServerModListSource::setModTags(std::unordered_set<std::string> const& tags) {
    m_query.tags = tags;
    this->clearCache();
}

server::ModsQuery const& ServerModListSource::getQuery() const {
    return m_query;
}
InvalidateQueryAfter<server::ModsQuery> ServerModListSource::getQueryMut() {
    return InvalidateQueryAfter(m_query, this);
}

bool ServerModListSource::wantsRestart() const {
    // todo
    return false;
}

void ModPackListSource::resetQuery() {}
ModPackListSource::ProviderTask ModPackListSource::fetchPage(size_t page, size_t pageSize) {
    return ProviderTask::immediate(Err(LoadPageError("Coming soon ;)")));
}

ModPackListSource::ModPackListSource() {}

ModPackListSource* ModPackListSource::get() {
    static auto inst = new ModPackListSource();
    return inst;
}

void ModPackListSource::setSearchQuery(std::string const& query) {}

std::unordered_set<std::string> ModPackListSource::getModTags() const {
    return {};
}
void ModPackListSource::setModTags(std::unordered_set<std::string> const& set) {}

bool ModPackListSource::wantsRestart() const {
    return false;
}
