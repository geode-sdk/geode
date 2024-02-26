#include "ModListSource.hpp"
#include <server/Server.hpp>

static size_t PAGE_SIZE = 10;

static size_t ceildiv(size_t a, size_t b) {
    // https://stackoverflow.com/questions/2745074/fast-ceiling-of-an-integer-division-in-c-c
    return a / b + (a % b != 0);
}

#define GEODE_GD_VERSION_STRINGIFY(version) # version
#define GEODE_GD_VERSION_STRINGIFY_2(version) GEODE_GD_VERSION_STRINGIFY(version)
#define GEODE_GD_VERSION_STR GEODE_GD_VERSION_STRINGIFY_2(GEODE_GD_VERSION)

typename ModListSource::PagePromise ModListSource::loadPage(size_t page, bool update) {
    if (!update && m_cachedPages.contains(page)) {
        return PagePromise([this, page](auto resolve, auto) {
            Loader::get()->queueInMainThread([this, page, resolve] {
                resolve(m_cachedPages.at(page));
            });
        });
    }
    m_cachedPages.erase(page);
    return PagePromise([this, page](auto resolve, auto reject, auto progress, auto cancelled) {
        this->reloadPage(page)
            .then([page, this, resolve = std::move(resolve)](auto data) {
                m_cachedPages.insert({ page, data });
                resolve(data);
            })
            .expect([this, reject = std::move(reject)](auto error) {
                reject(error);
            })
            .progress([this, progress = std::move(progress)](auto prog) {
                progress(prog);
            })
            .link(cancelled);
    }, false);
}

std::optional<size_t> ModListSource::getPageCount() const {
    return m_cachedItemCount ? std::optional(ceildiv(m_cachedItemCount.value(), PAGE_SIZE)) : std::nullopt;
}

std::optional<size_t> ModListSource::getItemCount() const {
    return m_cachedItemCount;
}

typename ModListSource::PagePromise InstalledModsList::reloadPage(size_t page) {
    m_cachedItemCount = Loader::get()->getAllMods().size();
    return PagePromise([page](auto resolve, auto, auto, auto const&) {
        Loader::get()->queueInMainThread([page, resolve = std::move(resolve)] {
            auto content = Page();
            auto all = Loader::get()->getAllMods();
            for (
                size_t i = page * PAGE_SIZE;
                i < all.size() && i < (page + 1) * PAGE_SIZE;
                i += 1
            ) {
                content.push_back(InstalledModItem::create(all.at(i)));
            }
            resolve(content);
        });
    });
}

InstalledModsList* InstalledModsList::get() {
    static auto inst = new InstalledModsList();
    return inst;
}

typename ModListSource::PagePromise FeaturedModsList::reloadPage(size_t page) {
    return PagePromise([this, page](auto resolve, auto reject, auto progress, auto cancelled) {
        server::getMods(server::ModsQuery {
            .page = page,
            .pageSize = PAGE_SIZE,
        })
        .then([this, resolve, reject](server::ServerModsList list) {
            m_cachedItemCount = list.totalModCount;
            if (list.totalModCount == 0) {
                return reject(LoadPageError("No mods found :("));
            }
            auto content = Page();
            for (auto mod : list.mods) {
                content.push_back(ServerModItem::create(mod));
            }
            resolve(content);
        })
        .expect([reject](auto error) {
            reject(LoadPageError("Error loading mods", error.details));
        })
        .progress([progress](auto prog) {
            progress(prog.percentage);
        })
        .link(cancelled);
    });
}

FeaturedModsList* FeaturedModsList::get() {
    static auto inst = new FeaturedModsList();
    return inst;
}

typename ModListSource::PagePromise ModPacksModsList::reloadPage(size_t page) {
    m_cachedItemCount = 0;
    return PagePromise([](auto, auto reject) {
        reject(LoadPageError("Coming soon! ;)"));
    });
}

ModPacksModsList* ModPacksModsList::get() {
    static auto inst = new ModPacksModsList();
    return inst;
}
