#pragma once

#include <Geode/ui/General.hpp>
#include <server/Server.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/ui/NineSlice.hpp>
#include <server/DownloadManager.hpp>
#include "../sources/ModSource.hpp"
#include "../UpdateModListState.hpp"
#include "ModListItem.hpp"

using namespace geode::prelude;

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
    CCNode* m_logo;
    CCNode* m_infoContainer;
    CCNode* m_titleContainer;
    Ref<CCLabelBMFont> m_titleLabel;
    CCLabelBMFont* m_versionLabel;
    CCNode* m_developers;
    CCNode* m_recommendedBy;
    NineSlice* m_description;
    CCLabelBMFont* m_developerLabel;
    ButtonSprite* m_restartRequiredLabel;
    ButtonSprite* m_outdatedLabel;
    CCNode* m_downloadWaiting;
    CCNode* m_downloadBarContainer;
    Slider* m_downloadBar;
    CCMenuItemToggler* m_enableToggle = nullptr;
    CCMenuItemSpriteExtra* m_updateBtn = nullptr;
    ListenerHandle m_updateStateHandle;
    async::TaskHolder<server::ServerResult<server::ServerModUpdateOneCheck>> m_checkUpdateListener;
    ListenerHandle m_downloadHandle;
    server::ServerModUpdateOneCheck m_availableUpdate;
    ListenerHandle m_settingNodeHandle;
    Ref<CCNode> m_badgeContainer = nullptr;
    Ref<CCNode> m_downloadCountContainer;
    CCLabelBMFont* m_versionDownloadSeparator;

    /**
     * @warning Make sure `getMetadata` and `createModLogo` are callable
     * before calling `init`!
    */
    bool init(ModSource&& source);

    void updateState();

    void onCheckUpdates(server::ServerResult<server::ServerModUpdateOneCheck> result);

    void onEnable(CCObject*);
    void onView(CCObject*);
    void onViewError(CCObject*);
    void onInstall(CCObject*);
    void onDevelopers(CCObject*);

public:
    static ModItem* create(ModSource&& source);

    ModSource& getSource() &;
};
