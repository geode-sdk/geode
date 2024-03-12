#include <Geode/DefaultInclude.hpp>
#include <Geode/modify/LevelPage.hpp>

using geode::prelude::LevelPage;
using cocos2d::CCObject;
using cocos2d::CCScene;
using cocos2d::CCTransitionScene;

struct CustomLevelPage : geode::modify::Modify<CustomLevelPage, LevelPage> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("Impostor PlayLayer Fix")

    void onPlay(CCObject* sender) override {
        if (!dynamic_cast<CCTransitionScene*>(CCScene::get())) {
            LevelPage::onPlay(sender);
        }
    }
};
