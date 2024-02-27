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

static auto loadInstalledModsPage(server::ModsQuery&& query) {
    return ModListSource::ProviderPromise([query = std::move(query)](auto resolve, auto, auto, auto const&) {
        Loader::get()->queueInMainThread([query = std::move(query), resolve = std::move(resolve)] {
            auto content = ModListSource::Page();
            std::vector<std::pair<Mod*, double>> mods;

            // todo: finish this
            for (auto& mod : Loader::get()->getAllMods()) {
                // bool someMatched = false;
                double weighted = 0;
                // if (query.query) {
                    // someMatched += weightedFuzzyMatch(mod->getName(), *query.query, 2, weighted);
                // }
                // if (someMatched) {
                    mods.push_back({ mod, weighted });
                // }
            }

            // Sort list based on score
            std::sort(mods.begin(), mods.end(), [](auto a, auto b) {
                return a.second < b.second;
            });
            for (
                size_t i = query.page * query.pageSize;
                i < mods.size() && i < (query.page + 1) * query.pageSize;
                i += 1
            ) {
                content.push_back(InstalledModItem::create(mods.at(i).first));
            }
            resolve({ content, mods.size() });
        });
    });
}

static auto loadServerModsPage(server::ModsQuery&& query) {
    return ModListSource::ProviderPromise([query = std::move(query)](auto resolve, auto reject, auto progress, auto cancelled) {
        server::getMods(query)
        .then([resolve, reject](server::ServerModsList list) {
            auto content = ModListSource::Page();
            for (auto mod : list.mods) {
                content.push_back(ServerModItem::create(mod));
            }
            resolve({ content, list.totalModCount });
        })
        .expect([reject](auto error) {
            reject(ModListSource::LoadPageError("Error loading mods", error.details));
        })
        .progress([progress](auto prog) {
            progress(prog.percentage);
        })
        .link(cancelled);
    });
}

typename ModListSource::PagePromise ModListSource::loadPage(size_t page, bool update) {
    // Return a generic "Coming soon" message if there's no provider set
    if (!m_provider) {
        return PagePromise([this, page](auto, auto reject) {
            reject("Coming soon! ;)");
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
    return PagePromise([this, page](auto resolve, auto reject, auto progress, auto cancelled) {
        m_provider(server::ModsQuery {
            .query = m_query,
            .page = page,
            // todo: loader setting to change this
            .pageSize = PER_PAGE,
        })
            .then([page, this, resolve, reject](auto data) {
                if (data.second == 0 || data.first.empty()) {
                    return reject(ModListSource::LoadPageError("No mods found :("));
                }
                m_cachedItemCount = data.second;
                m_cachedPages.insert({ page, data.first });
                resolve(data.first);
            })
            .expect([this, reject = reject](auto error) {
                reject(error);
            })
            .progress([this, progress = std::move(progress)](auto prog) {
                progress(prog);
            })
            .link(cancelled);
    }, false);
}

std::optional<size_t> ModListSource::getPageCount() const {
    return m_cachedItemCount ? std::optional(ceildiv(m_cachedItemCount.value(), PER_PAGE)) : std::nullopt;
}

std::optional<size_t> ModListSource::getItemCount() const {
    return m_cachedItemCount;
}

void ModListSource::reset() {
    m_query.clear();
    m_cachedPages.clear();
    m_cachedItemCount = std::nullopt;
}

void ModListSource::setQuery(std::string const& query) {
    // Set query & reset cache
    if (m_query != query) {
        m_query = query;
        m_cachedPages.clear();
        m_cachedItemCount = std::nullopt;
    }
}

ModListSource* ModListSource::create(Provider* provider) {
    auto ret = new ModListSource();
    ret->m_provider = provider;
    ret->autorelease();
    return ret;
}

ModListSource* ModListSource::get(ModListSourceType type) {
    switch (type) {
        default:
        case ModListSourceType::Installed: {
            static auto inst = Ref(ModListSource::create(loadInstalledModsPage));
            return inst;
        } break;

        case ModListSourceType::Featured: {
            static auto inst = Ref(ModListSource::create(+[](server::ModsQuery&& query) {
                query.featured = true;
                return loadServerModsPage(std::move(query));
            }));
            return inst;
        } break;

        case ModListSourceType::Trending: {
            static auto inst = Ref(ModListSource::create(nullptr));
            return inst;
        } break;

        case ModListSourceType::ModPacks: {
            static auto inst = Ref(ModListSource::create(nullptr));
            return inst;
        } break;

        case ModListSourceType::All: {
            static auto inst = Ref(ModListSource::create(+[](server::ModsQuery&& query) {
                return loadServerModsPage(std::move(query));
            }));
            return inst;
        } break;
    }
}
