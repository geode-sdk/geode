#include "ModSettingsPopup.hpp"
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/loader/Setting.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/convert.hpp>

bool ModSettingsPopup::setup(Mod* mod) {
    m_noElasticity = true;
    m_mod = mod;

    this->setTitle((mod->getName() + "'s Settings").c_str());

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto const layerSize = CCSize { 360.f, 220.f };

    auto layerBG = CCLayerColor::create({ 0, 0, 0, 75 });
    layerBG->setContentSize(layerSize);
    layerBG->setPosition(winSize / 2 - layerSize / 2);
    m_mainLayer->addChild(layerBG);
    
    auto layer = ScrollLayer::create(layerSize);
    layer->setPosition(winSize / 2 - layerSize / 2);

    CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
    layer->registerWithTouchDispatcher();

    float totalHeight = .0f;
    std::vector<CCNode*> rendered;
    bool hasBG = true;
    for (auto& [_, sett] : mod->getSettings()) {
        auto node = sett->createNode(layerSize.width);

        totalHeight += node->getScaledContentSize().height;

        if (hasBG) {
            auto bg = CCLayerColor::create({ 0, 0, 0, 75 });
            bg->setContentSize(node->getScaledContentSize());
            bg->setPosition(0.f, -totalHeight);
            bg->setZOrder(-10);
            layer->m_contentLayer->addChild(bg);

            rendered.push_back(bg);

            hasBG = false;
        } else {
            hasBG = true;
        }

        node->setPosition(0.f, -totalHeight);
        layer->m_contentLayer->addChild(node);

        rendered.push_back(node);
        m_settings.push_back(node);
    }
    if (totalHeight < layerSize.height) {
        totalHeight = layerSize.height;
    }
    for (auto& node : rendered) {
        node->setPositionY(node->getPositionY() + totalHeight);
    }
    layer->m_contentLayer->setContentSize({ layerSize.width, totalHeight });

    m_mainLayer->addChild(layer);

    m_applyBtnSpr = ButtonSprite::create("Apply");
    m_applyBtnSpr->setScale(.8f);

    m_applyBtn = CCMenuItemSpriteExtra::create(
        m_applyBtnSpr, this, makeMenuSelector([this](CCObject*) {
            bool someChangesMade = false;
            for (auto& sett : m_settings) {
                if (sett->hasUncommittedChanges()) {
                    sett->commit();
                    someChangesMade = true;
                }
            }
            if (!someChangesMade) {
                FLAlertLayer::create(
                    "Info",
                    "No changes have been made.",
                    "OK"
                )->show();
            }
        })
    );
    m_applyBtn->setPosition(.0f, -m_size.height / 2 + 20.f);
    m_buttonMenu->addChild(m_applyBtn);

    return true;
}

bool ModSettingsPopup::hasUncommitted() const {
    for (auto& sett : m_settings) {
        if (sett->hasUncommittedChanges()) {
            return true;
        }
    }
    return false;
}

void ModSettingsPopup::onClose(CCObject* sender) {
    if (sender && this->hasUncommitted()) {
        return createQuickPopup(
            "Unsaved Changes",
            "You have <cr>unsaved changes</c>! Are you sure you "
            "want to exit?",
            "Cancel", "Discard",
            [this](FLAlertLayer*, bool btn2) {
                if (btn2) this->onClose(nullptr);
            }
        );
    }
    Popup<Mod*>::onClose(sender);
}

ModSettingsPopup* ModSettingsPopup::create(Mod* mod) {
    auto ret = new ModSettingsPopup();
    if (ret && ret->init(440.f, 290.f, mod)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
