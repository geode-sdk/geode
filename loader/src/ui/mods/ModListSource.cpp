#include "ModListSource.hpp"
#include <Geode/utils/web2.hpp>

static size_t INSTALLED_MODS_PAGE_SIZE = 10;

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
    return m_cachedPageCount;
}

typename ModListSource::PagePromise InstalledModsList::reloadPage(size_t page) {
    m_cachedPageCount = Loader::get()->getAllMods().size() / INSTALLED_MODS_PAGE_SIZE + 1;
    return PagePromise([page](auto resolve, auto, auto, auto const&) {
        Loader::get()->queueInMainThread([page, resolve = std::move(resolve)] {
            auto content = Page();
            auto all = Loader::get()->getAllMods();
            for (
                size_t i = page * INSTALLED_MODS_PAGE_SIZE;
                i < all.size() && i < (page + 1) * INSTALLED_MODS_PAGE_SIZE;
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

Result<typename FeaturedModsList::Page> FeaturedModsList::parseModsListResponse(matjson::Value const& json) {
    log::info("mods list: {}", json);
    return Ok(Page());
}

typename ModListSource::PagePromise FeaturedModsList::reloadPage(size_t page) {
    return PagePromise([](auto resolve, auto reject, auto progress, auto cancelled) {
        web::WebRequest()
            .param("gd", GEODE_GD_VERSION_STR)
            .param("featured", "true")
            .get("https://api.geode-sdk.org/v1/mods")
            .then([resolve, reject](web::WebResponse value) {
                // Validate that the response was JSON
                auto json = value.json();
                if (!json) {
                    return reject(LoadPageError(
                        "Error loading mods",
                        "Response was not valid JSON: " + json.unwrapErr()
                    ));
                }
                
                // Parse the response into a mods list
                auto page = parseModsListResponse(json.unwrap());
                if (!page) {
                    return reject(LoadPageError("Error loading mods", page.unwrapErr()));
                }
                resolve(page.unwrap());
            })
            .expect([reject](web::WebResponse value) {
                // Specialize 404 for a "No mods found" error
                if (value.code() == 404) {
                    return reject(LoadPageError("No mods found :("));
                }
                // Otherwise generic error
                reject(LoadPageError("Error loading mods", value.string().ok()));
            })
            .progress([progress](web::WebProgress info) {
                progress(info.downloadProgress());
            })
            .link(cancelled);
    });
}

FeaturedModsList* FeaturedModsList::get() {
    static auto inst = new FeaturedModsList();
    return inst;
}

typename ModListSource::PagePromise ModPacksModsList::reloadPage(size_t page) {
    m_cachedPageCount = 0;
    return PagePromise([](auto, auto reject) {
        reject(LoadPageError("Coming soon! ;)"));
    });
}

ModPacksModsList* ModPacksModsList::get() {
    static auto inst = new ModPacksModsList();
    return inst;
}
