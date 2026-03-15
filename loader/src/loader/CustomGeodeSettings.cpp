#include <Geode/loader/Mod.hpp>
#include <Geode/ui/Notification.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <ui/mods/GeodeStyle.hpp>
#include <ui/mods/ModsLayer.hpp>

using namespace geode::prelude;

$on_mod(Loaded) {
    
    ButtonSettingPressedEventV3(Mod::get(), "action-buttons").listen([] (auto buttonKey) {
        if (buttonKey == "open-mods-folder") {
            file::openFolder(dirs::getModsDir());
        }
        else if (buttonKey == "copy-mods-list") {
            auto mods = Loader::get()->getAllMods();
            if (mods.empty()) {
                Notification::create("No mods installed", NotificationIcon::Info, 0.5f)->show();
                return;
            }

            std::sort(mods.begin(), mods.end(), [](Mod* a, Mod* b) {
                auto const s1 = a->getID();
                auto const s2 = b->getID();
                return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(), [](auto a, auto b) {
                    return std::tolower(a) < std::tolower(b);
                });
            });

            std::string modsList;
            using namespace std::string_view_literals;
            for (int i = 0; i < mods.size(); i++) {
                auto& mod = mods[i];
                modsList += fmt::format("{} | [{}] {}{}",
                    mod->isLoaded() ? "x"sv :
                    mod->targetsOutdatedVersion() ? "*"sv :
                    mod->failedToLoad() ? "!"sv :
                    " "sv,
                    mod->getVersion().toVString(), mod->getID(),
                    i < mods.size() ? "\n" : ""
                );
            }
            clipboard::write(modsList);

            Notification::create("Mods list copied to clipboard", NotificationIcon::Info, 0.5f)->show();
        }
        else if (buttonKey == "install-from-file") {
            ModsLayer::installModFromFile();
        }
    }).leak();
}
