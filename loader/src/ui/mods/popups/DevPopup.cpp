#include <Geode/binding/ButtonSprite.hpp>
#include "DevPopup.hpp"
#include "../UpdateModListState.hpp"
#include "ui/mods/list/ModDeveloperList.hpp"
#include "ui/mods/sources/ModSource.hpp"

bool DevListPopup::init(ModSource meta) {
    if (!GeodePopup::init(250.f, 210.f))
        return false;

    m_source = std::move(meta);

    this->setTitle(fmt::format("Developers for {}", m_source.getMetadata().getName()));
    m_title->limitLabelWidth(m_size.width - 50, .7f, .1f);

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

DevListPopup* DevListPopup::create(ModSource meta) {
    auto ret = new DevListPopup();
    if (ret->init(std::move(meta))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
