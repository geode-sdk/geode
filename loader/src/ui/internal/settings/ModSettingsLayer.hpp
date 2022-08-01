/*#pragma once

#include <Geode.hpp>

USE_GEODE_NAMESPACE();

class ModSettingsList;

class ModSettingsLayer : public FLAlertLayer, public FLAlertLayerProtocol {
    protected:
        Mod* m_mod;
        ModSettingsList* m_list;
        ButtonSprite* m_applyBtnSpr;

        void FLAlert_Clicked(FLAlertLayer*, bool) override;

		bool init(Mod* mod);

		void onApply(CCObject*);
		void onResetAllToDefault(CCObject*);
		void keyDown(enumKeyCodes) override;
		void onClose(CCObject*);
        void close();
		
    public:
        static ModSettingsLayer* create(Mod* Mod);

        void updateState();
};

*/