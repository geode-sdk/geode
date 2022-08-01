/*#pragma once

#include <Geode.hpp>
#include <nodes/ScrollLayer.hpp>
#include <settings/SettingNode.hpp>

USE_GEODE_NAMESPACE();

class ModSettingsLayer;

class ModSettingsList : public CCLayer {
protected:
    Mod* m_mod;
    ModSettingsLayer* m_settingsLayer;
    ScrollLayer* m_scrollLayer;
    std::vector<SettingNode*> m_settingNodes;

    bool init(Mod* mod, ModSettingsLayer* layer, float width, float height);

public:
    static ModSettingsList* create(
        Mod* mod, ModSettingsLayer* layer, float width, float height
    );

    void updateList();
    void resetAllToDefault();
    bool hasUnsavedModifiedSettings() const;
    void applyChanges();
};*/
