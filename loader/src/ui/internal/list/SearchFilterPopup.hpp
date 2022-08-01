#pragma once

#include <Geode/Geode.hpp>

USE_GEODE_NAMESPACE();

class ModListLayer;

class SearchFilterPopup : public FLAlertLayer {
    protected:
        ModListLayer* m_modLayer;
        CCPoint m_pos;

		bool init(ModListLayer* layer);
        void addToggle(const char* title, int flag);

		void keyDown(cocos2d::enumKeyCodes) override;
		void onClose(cocos2d::CCObject*);
        void onToggle(cocos2d::CCObject*);
		
    public:
        static SearchFilterPopup* create(ModListLayer* layer);
};

