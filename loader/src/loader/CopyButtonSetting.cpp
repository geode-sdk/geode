#include "CopyButtonSetting.hpp"
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/Notification.hpp>

$on_mod(Loaded) {
    (void)Mod::get()->registerCustomSettingType("copy-mods", &CopyButtonSetting::parse);
}

SettingNodeV3* CopyButtonSetting::createNode(float width) {
    return CopyButtonSettingNode::create(std::static_pointer_cast<CopyButtonSetting>(shared_from_this()), width);
}

void CopyButtonSettingNode::onCopy(CCObject*) {
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
