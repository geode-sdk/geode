#pragma once

#include <Geode/ui/General.hpp>
#include <server/Server.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/ui/ProgressBar.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/ui/Toggler.hpp>
#include <Geode/ui/LoadingSpinner.hpp>
#include <Geode/ui/NineSlice.hpp>
#include <server/DownloadManager.hpp>
#include "../sources/ModSource.hpp"
#include "../UpdateModListState.hpp"
#include "ModListItem.hpp"

// i made it this way just in case someone wanted to add to the enum in the future
// mat is allowed to judge
template<>
struct matjson::Serialize<ModListDisplay> {
    static Result<ModListDisplay> fromJson(matjson::Value const& value) {
        auto saved = GEODE_UNWRAP(value.asString());
        if (saved == "small-list") {
            return Ok(ModListDisplay::SmallList);
        } else if (saved == "big-list") {
            return Ok(ModListDisplay::BigList);
        } else if (saved == "grid") {
            return Ok(ModListDisplay::Grid);
        }

        return Err("unknown display type");
    }

    static matjson::Value toJson(ModListDisplay const& value) {
        switch (value) {
            default:
            case ModListDisplay::SmallList:
                return "small-list";
                break;
            case ModListDisplay::BigList:
                return "big-list";
                break;
            case ModListDisplay::Grid:
                return "grid";
                break;
        }
    }
};

class ModItem : public ModListItem {
protected:
    ModSource m_source;
    CCNode* m_logo = nullptr;
    CCNode* m_infoContainer = nullptr;
    CCNode* m_titleContainer = nullptr;
    Ref<Label> m_titleLabel = nullptr;
    Label* m_versionLabel = nullptr;
    CCNode* m_developers = nullptr;
    CCNode* m_recommendedBy = nullptr;
    NineSlice* m_description = nullptr;
    Button* m_developersBtn = nullptr;
    ButtonSprite* m_restartRequiredLabel = nullptr;
    ButtonSprite* m_outdatedLabel = nullptr;
    ButtonSprite* m_deprecatedLabel = nullptr;
    CCNode* m_modLabelsContainer = nullptr;
    CCNode* m_downloadWaiting = nullptr;
    CCNode* m_downloadBarContainer = nullptr;
    ProgressBar* m_downloadBar = nullptr;
    Toggler* m_enableToggle = nullptr;
    Toggler* m_pinToggle = nullptr;
    Button* m_updateBtn = nullptr;
    ListenerHandle m_updateStateHandle;
    async::TaskHolder<server::ServerResult<server::ServerModUpdateOneCheck>> m_checkUpdateListener;
    ListenerHandle m_downloadHandle;
    server::ServerModUpdateOneCheck m_availableUpdate;
    ListenerHandle m_settingNodeHandle;
    Ref<CCNode> m_badgeContainer = nullptr;
    Ref<CCNode> m_downloadCountContainer = nullptr;
    Ref<CCNode> m_updatedAtContainer = nullptr;
    Label* m_versionDownloadSeparator = nullptr;

    /**
     * @warning Make sure `getMetadata` and `createModLogo` are callable
     * before calling `init`!
    */
    bool init(ModSource&& source, bool listItem);

    void updateState();

    void onCheckUpdates(server::ServerResult<server::ServerModUpdateOneCheck> result);

    void onPin(CCObject*);
    void onEnable(CCObject*);
    void onView(CCObject*);
    void onViewError(CCObject*);
    void onInstall(CCObject*);
    void onDevelopers(CCObject*);

public:
    static ModItem* create(ModSource&& source, bool listItem = true);

    ModSource& getSource() &;
};

/**
 * Standalone ModItem that you give a Mod ID to and it'll either show the mod
 * if it's installed or fetch from server if it is not
 */
class AnyModItem : public ModListItem {
protected:
    ModItem* m_item = nullptr;
    LoadingSpinner* m_loading;
    async::TaskHolder<server::ServerResult<server::ServerModMetadata>> m_listener;

    bool init(ZStringView modID);
    void gotSrc(ModSource&& src);

public:
    static AnyModItem* create(ZStringView modID);

    void updateDisplay(float width, ModListDisplay display) override;
};
