#include "ModListSource.hpp"
#include <server/Server.hpp>

static size_t PER_PAGE = 10;

static size_t ceildiv(size_t a, size_t b) {
    // https://stackoverflow.com/questions/2745074/fast-ceiling-of-an-integer-division-in-c-c
    return a / b + (a % b != 0);
}

static auto loadInstalledModsPage(size_t page) {
    return ModListSource::ProviderPromise([page](auto resolve, auto, auto, auto const&) {
        Loader::get()->queueInMainThread([page, resolve = std::move(resolve)] {
            auto content = ModListSource::Page();
            auto all = Loader::get()->getAllMods();
            for (
                size_t i = page * PER_PAGE;
                i < all.size() && i < (page + 1) * PER_PAGE;
                i += 1
            ) {
                content.push_back(InstalledModItem::create(all.at(i)));
            }
            resolve({ content, all.size() });
        });
    });
}

static auto loadServerModsPage(size_t page, bool featuredOnly) {
    return ModListSource::ProviderPromise([page, featuredOnly](auto resolve, auto reject, auto progress, auto cancelled) {
        server::getMods(server::ModsQuery {
            .featured = featuredOnly ? std::optional(true) : std::nullopt,
            .page = page,
            .pageSize = PER_PAGE,
        })
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
        m_provider(page)
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
            static auto inst = Ref(ModListSource::create(+[](size_t page) {
                return loadServerModsPage(page, true);
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
            static auto inst = Ref(ModListSource::create(+[](size_t page) {
                return loadServerModsPage(page, false);
            }));
            return inst;
        } break;
    }
}
