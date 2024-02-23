#include "ModListSource.hpp"

static size_t INSTALLED_MODS_PAGE_SIZE = 10;

struct InstalledModsList : ModListSource {
    std::string_view getID() const override {
        return "installed";
    }

    Promise<Page> loadNewPage(size_t page, bool) {
        // You can't load new mods at runtime so update does nothing
        return Promise([](auto resolve, auto _reject, auto _progress) {
            std::thread([res = std::move(resolve)] {
                std::this_thread::sleep_for(std::chrono::seconds(3));
                auto page = Page();
                auto all = Loader::get()->getAllMods();
                for (size_t i = page * INSTALLED_MODS_PAGE_SIZE; i < all.size(); i += 1) {
                    page.push_back(InstalledModItem::create(all.at(i)));
                }
                res(page);
            }).detach();
        });
    }

    Promise<size_t> loadTotalPageCount() const {
        return Promise([](auto resolve, auto _reject, auto _progress) {
            resolve(Loader::get()->getAllMods().size() / INSTALLED_MODS_PAGE_SIZE + 1);
        });
    }

    static InstalledModsList& get() {
        static auto inst = InstalledModsList();
        return inst;
    }
};

std::optional<std::reference_wrapper<ModListSource>> ModListSource::get(std::string_view id) {
    switch (hash(id)) {
        case hash("installed"): {
            return InstalledModsList::get();
        } break;
    }
    return std::nullopt;
}
