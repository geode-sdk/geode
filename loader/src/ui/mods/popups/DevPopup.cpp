#include <Geode/binding/ButtonSprite.hpp>
#include "DevPopup.hpp"
#include "../UpdateModListState.hpp"
#include "ui/mods/list/ModDeveloperList.hpp"
#include "ui/mods/sources/ModSource.hpp"

bool DevListPopup::setup(ModSource const& meta) {

    m_source = meta;

    this->setTitle(fmt::format("Developers for {}", m_source.getMetadata().getName()));
    m_title->limitLabelWidth(m_mainLayer->getContentSize().width - 50, .7f, .1f);
    CCSize contentSize = m_mainLayer->getContentSize();
    CCArray* elements = CCArray::create();
    ModDeveloperList* list = ModDeveloperList::create(this, m_source, {210.f, 150.f});
    m_mainLayer->addChildAtPosition(
        list,
        Anchor::Center,
        { 0.0f, -10.0f }
    );

    return true;
}

void DevListPopup::onClose(cocos2d::CCObject*){
    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);
    this->removeFromParentAndCleanup(true);
}

void DevListPopup::onMoreByThisDev(CCObject* sender) {
    auto str = static_cast<CCString*>(static_cast<CCNode*>(sender)->getUserObject());
    UpdateModListStateEvent(UpdateWholeState(str->getCString())).post();
    this->onClose(nullptr);
}

DevListPopup* DevListPopup::create(ModSource const& meta) {
    auto ret = new DevListPopup();
    if (ret->init(250, 210, meta)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
