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

static std::pair<std::vector<Mod*>, size_t> getModsWithQuery(server::ModsQuery const& query) {
    std::vector<std::pair<Mod*, double>> mods;

    // Filter installed mods based on query
    for (auto& mod : Loader::get()->getAllMods()) {
        bool addToList = !query.query.has_value();
        double weighted = 0;
        if (query.query) {
            addToList |= weightedFuzzyMatch(mod->getName(), *query.query, 2, weighted);
            addToList |= weightedFuzzyMatch(mod->getID(),   *query.query, 1, weighted);
            for (auto& dev : mod->getDevelopers()) {
                addToList |= weightedFuzzyMatch(dev, *query.query, 0.75, weighted);
            }
            if (auto details = mod->getDetails()) {
                addToList |= weightedFuzzyMatch(*details, *query.query, 0.05, weighted);
            }
            if (auto desc = mod->getDescription()) {
                addToList |= weightedFuzzyMatch(*desc, *query.query, 0.2, weighted);
            }
            if (weighted < 2) {
                addToList = false;
            }
        }
        // Loader gets boost to ensure it's normally always top of the list
        if (mod->getID() == "geode.loader") {
            weighted += 5;
        }
        if (addToList) {
            mods.push_back({ mod, weighted });
        }
    }

    // Sort list based on score
    std::sort(mods.begin(), mods.end(), [](auto a, auto b) {
        // Sort primarily by score
        if (a.second != b.second) {
            return a.second > b.second;
        }
        // Sort secondarily alphabetically
        return a.first->getName() < b.first->getName();
    });

    // Pick out only the mods in the page and page size specified in the query
    std::vector<Mod*> result {};
    for (
        size_t i = query.page * query.pageSize;
        i < mods.size() && i < (query.page + 1) * query.pageSize;
        i += 1
    ) {
        result.push_back(mods.at(i).first);
    }
    // Return paged mods and total count of matches
    return { result, mods.size() };
}

static auto loadInstalledModsPage(server::ModsQuery&& query) {
    return ModListSource::ProviderPromise([query = std::move(query)](auto resolve, auto, auto, auto const&) {
        Loader::get()->queueInMainThread([query = std::move(query), resolve = std::move(resolve)] {
            auto content = ModListSource::ProvidedMods();
            auto paged = getModsWithQuery(query);
            for (auto& mod : std::move(paged.first)) {
                content.mods.push_back(ModSource(mod));
            }
            content.totalModCount = paged.second;
            resolve(content);
        });
    });
}

static auto loadServerModsPage(server::ModsQuery&& query) {
    return server::ServerResultCache<&server::getMods>::shared().get(query)
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

typename ModListSource::PagePromise ModListSource::loadPage(size_t page, bool update) {
    // Return a generic "Coming soon" message if there's no provider set
    if (!m_provider.get) {
        return PagePromise([this, page](auto, auto reject) {
            reject(LoadPageError("Coming soon! ;)"));
        });
    }
    if (!update && m_cachedPages.contains(page)) {
        return PagePromise([this, page](auto resolve, auto) {
            Loader::get()->queueInMainThread([this, page, resolve] {
                resolve(m_cachedPages.at(page));
            });
        });
    }
    m_cachedPages.erase(page);
    return m_provider.get(server::ModsQuery {
        .query = m_query,
        .page = page,
        // todo: loader setting to change this
        .pageSize = PER_PAGE,
    })
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
    m_query = std::nullopt;
    m_cachedPages.clear();
    m_cachedItemCount = std::nullopt;
}

void ModListSource::setQuery(std::string const& query) {
    // Set query & reset cache
    if (m_query != query) {
        m_query = query.empty() ? std::nullopt : std::optional(query);
        m_cachedPages.clear();
        m_cachedItemCount = std::nullopt;
    }
}

bool ModListSource::isInstalledMods() const {
    return m_provider.isInstalledMods;
}
bool ModListSource::wantsRestart() const {
    return m_provider.wantsRestart && m_provider.wantsRestart();
}

ModListSource* ModListSource::create(Provider&& provider) {
    auto ret = new ModListSource();
    ret->m_provider = std::move(provider);
    ret->autorelease();
    return ret;
}

ModListSource* ModListSource::get(ModListSourceType type) {
    switch (type) {
        default:
        case ModListSourceType::Installed: {
            static auto inst = Ref(ModListSource::create({
                .get = loadInstalledModsPage,
                .wantsRestart = +[] {
                    for (auto mod : Loader::get()->getAllMods()) {
                        if (mod->getRequestedAction() != ModRequestedAction::None) {
                            return true;
                        }
                    }
                    return false;
                },
                .isInstalledMods = true,
            }));
            return inst;
        } break;

        case ModListSourceType::Updates: {
            static auto inst = Ref(ModListSource::create({}));
            return inst;
        } break;

        case ModListSourceType::Featured: {
            static auto inst = Ref(ModListSource::create({
                .get = +[](server::ModsQuery&& query) {
                    query.featured = true;
                    return loadServerModsPage(std::move(query));
                },
            }));
            return inst;
        } break;

        case ModListSourceType::Trending: {
            static auto inst = Ref(ModListSource::create({}));
            return inst;
        } break;

        case ModListSourceType::ModPacks: {
            static auto inst = Ref(ModListSource::create({}));
            return inst;
        } break;

        case ModListSourceType::All: {
            static auto inst = Ref(ModListSource::create({
                .get = loadServerModsPage,
            }));
            return inst;
        } break;
    }
}
