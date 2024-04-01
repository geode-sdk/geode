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

typename ModListSource::PagePromise ModListSource::loadPage(size_t page, bool update) {
    if (!update && m_cachedPages.contains(page)) {
        return PagePromise([this, page](auto resolve, auto) {
            Loader::get()->queueInMainThread([this, page, resolve] {
                resolve(m_cachedPages.at(page));
            });
        });
    }
    m_cachedPages.erase(page);
    return this->fetchPage(page, PER_PAGE)
        .then<Page, ModListSource::LoadPageError>([page, this](auto result) -> Result<Page, ModListSource::LoadPageError> {
            if (result) {
                auto data = result.unwrap();
                if (data.totalModCount == 0 || data.mods.empty()) {
                    return Err(ModListSource::LoadPageError("No mods found :("));
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
                return Err(result.unwrapErr());
            }
        });
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

InstalledModListSource::ProviderPromise InstalledModListSource::fetchPage(size_t page, size_t pageSize) {
    m_query.page = page;
    m_query.pageSize = pageSize;
    return ModListSource::ProviderPromise([query = m_query](auto resolve, auto, auto, auto const&) {
        Loader::get()->queueInMainThread([query = std::move(query), resolve = std::move(resolve)] {
            auto content = ModListSource::ProvidedMods();
            for (auto& mod : Loader::get()->getAllMods()) {
                content.mods.push_back(ModSource(mod));
            }
            // If we're only checking mods that have updates, we first have to run 
            // update checks every mod...
            if (query.onlyUpdates && content.mods.size()) {
                struct Waiting final {
                    ModListSource::ProvidedMods content;
                    std::atomic_size_t waitingFor;
                };
                auto waiting = std::make_shared<Waiting>();
                waiting->waitingFor = content.mods.size();
                waiting->content = std::move(content);
                for (auto& src : waiting->content.mods) {
                    // todo: Promise::finallyOrCancelled
                    src.checkUpdates().finally([resolve, query, waiting] {
                        waiting->waitingFor -= 1;
                        if (waiting->waitingFor == 0) {
                            filterModsWithQuery(waiting->content, query);
                            resolve(std::move(waiting->content));
                        }
                    }).cancelled([resolve, query, waiting] {
                        waiting->waitingFor -= 1;
                        if (waiting->waitingFor == 0) {
                            filterModsWithQuery(waiting->content, query);
                            resolve(std::move(waiting->content));
                        }
                    });
                }
            }
            // Otherwise simply construct the result right away
            else {
                filterModsWithQuery(content, query);
                resolve(content);
            }
        });
    });
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

ServerModListSource::ProviderPromise ServerModListSource::fetchPage(size_t page, size_t pageSize) {
    m_query.page = page;
    m_query.pageSize = pageSize;
    return server::ServerResultCache<&server::getMods>::shared().get(m_query)
        .then<ModListSource::ProvidedMods>([](server::ServerModsList list) {
            auto content = ModListSource::ProvidedMods();
            for (auto&& mod : std::move(list.mods)) {
                content.mods.push_back(ModSource(std::move(mod)));
            }
            content.totalModCount = list.totalModCount;
            return content;
        })
        .expect<ModListSource::LoadPageError>([](auto error) {
            return ModListSource::LoadPageError("Error loading mods", error.details);
        })
        .progress<std::optional<uint8_t>>([](auto prog) {
            return prog.percentage;
        });
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
ModPackListSource::ProviderPromise ModPackListSource::fetchPage(size_t page, size_t pageSize) {
    return ProviderPromise([](auto, auto reject) {
        reject(LoadPageError("Coming soon ;)"));
    });
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
