#include "ModListSource.hpp"

static size_t INSTALLED_MODS_PAGE_SIZE = 10;

Promise<typename ModListSource::Page> ModListSource::loadPage(size_t page, bool update) {
    if (!update && m_cachedPages.contains(page)) {
        return Promise<Page>([this, page](auto resolve, auto) { resolve(m_cachedPages.at(page)); });
    }
    m_cachedPages.erase(page);
    return Promise<Page>([this, page](auto resolve, auto reject, auto progress, auto const&) {
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
            });
    });
}

std::optional<size_t> ModListSource::getPageCount() const {
    return m_cachedPageCount;
}

Promise<typename ModListSource::Page> InstalledModsList::reloadPage(size_t page) {
    m_cachedPageCount = Loader::get()->getAllMods().size() / INSTALLED_MODS_PAGE_SIZE + 1;
    return Promise<Page>([page](auto resolve, auto, auto, auto const&) {
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
}

InstalledModsList* InstalledModsList::get() {
    static auto inst = new InstalledModsList();
    return inst;
}

Promise<typename ModListSource::Page> ModPacksModsList::reloadPage(size_t page) {
    m_cachedPageCount = 0;
    return Promise<Page>([](auto, auto reject) {
        reject("Coming soon! ;)");
    });
}

ModPacksModsList* ModPacksModsList::get() {
    static auto inst = new ModPacksModsList();
    return inst;
}
