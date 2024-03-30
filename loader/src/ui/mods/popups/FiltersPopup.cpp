#include "FiltersPopup.hpp"

bool FiltersPopup::setup(ModListSource* src) {
    m_noElasticity = true;
    m_source = src;
    m_selectedTags = src->getModTags();

    this->setTitle("Search Filters");

    auto tagsContainer = CCNode::create();
    tagsContainer->setContentSize(ccp(310, 80));
    tagsContainer->setAnchorPoint({ .5f, .5f });

    auto tagsBG = CCScale9Sprite::create("square02b_001.png");
    tagsBG->setColor({ 0, 0, 0 });
    tagsBG->setOpacity(75);
    tagsBG->setScale(.3f);
    tagsBG->setContentSize(tagsContainer->getContentSize() / tagsBG->getScale());
    tagsContainer->addChildAtPosition(tagsBG, Anchor::Center);

    m_tagsMenu = CCMenu::create();
    m_tagsMenu->setContentSize(tagsContainer->getContentSize() - ccp(10, 10));
    m_tagsMenu->addChild(createLoadingCircle(40));
    m_tagsMenu->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(.1f, 1.f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
            ->setAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisAlignment(AxisAlignment::Center)
    );
    tagsContainer->addChildAtPosition(m_tagsMenu, Anchor::Center);

    auto tagsTitleMenu = CCMenu::create();
    tagsTitleMenu->setAnchorPoint({ .5f, 0 });
    tagsTitleMenu->setContentWidth(tagsContainer->getContentWidth());

    auto tagsTitle = CCLabelBMFont::create("Tags", "bigFont.fnt");
    tagsTitleMenu->addChild(tagsTitle);

    tagsTitleMenu->addChild(SpacerNode::create());

    auto resetSpr = CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png");
    auto resetBtn = CCMenuItemSpriteExtra::create(
        resetSpr, this, menu_selector(FiltersPopup::onResetTags)
    );
    tagsTitleMenu->addChild(resetBtn);

    tagsTitleMenu->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(.1f, .4f)
    );
    tagsContainer->addChildAtPosition(tagsTitleMenu, Anchor::Top, ccp(0, 4));

    m_mainLayer->addChildAtPosition(tagsContainer, Anchor::Center, ccp(0, 30));

    auto okSpr = createGeodeButton("OK");
    okSpr->setScale(.7f);
    auto okBtn = CCMenuItemSpriteExtra::create(
        okSpr, this, menu_selector(FiltersPopup::onClose)
    );
    m_buttonMenu->addChildAtPosition(okBtn, Anchor::Bottom, ccp(0, 20));

    m_tagsListener.bind(this, &FiltersPopup::onLoadTags);
    m_tagsListener.setFilter(server::ServerResultCache<&server::getTags>::shared().get().listen());

    return true;
}

void FiltersPopup::onLoadTags(PromiseEvent<std::unordered_set<std::string>, server::ServerError>* event) {
    if (auto tags = event->getResolve()) {
        m_tagsMenu->removeAllChildren();
        for (auto& tag : *tags) {
            auto offSpr = createGeodeTagLabel(tag);
            offSpr->m_BGSprite->setOpacity(105);
            offSpr->m_label->setOpacity(105);
            auto onSpr = createGeodeTagLabel(tag);
            auto btn = CCMenuItemToggler::create(
                offSpr, onSpr, this, menu_selector(FiltersPopup::onSelectTag)
            );
            btn->m_notClickable = true;
            btn->setUserObject("tag", CCString::create(tag));
            m_tagsMenu->addChild(btn);
        }
        m_tagsMenu->updateLayout();
        this->updateTags();
    }
    else if (event->getReject()) {
        m_tagsMenu->removeAllChildren();
        auto label = CCLabelBMFont::create("Unable to load tags", "bigFont.fnt");
        label->setOpacity(105);
        m_tagsMenu->addChild(label);
        m_tagsMenu->updateLayout();
    }
}

void FiltersPopup::updateTags() {
    for (auto node : CCArrayExt<CCNode*>(m_tagsMenu->getChildren())) {
        if (auto toggle = typeinfo_cast<CCMenuItemToggler*>(node)) {
            auto tag = static_cast<CCString*>(toggle->getUserObject("tag"))->getCString();
            toggle->toggle(m_selectedTags.contains(tag));
        }
    }
}

void FiltersPopup::onSelectTag(CCObject* sender) {
    auto toggle = static_cast<CCMenuItemToggler*>(sender);
    auto tag = static_cast<CCString*>(toggle->getUserObject("tag"))->getCString();
    if (m_selectedTags.contains(tag)) {
        m_selectedTags.erase(tag);
    }
    else {
        m_selectedTags.insert(tag);
    }
    this->updateTags();
}

void FiltersPopup::onResetTags(CCObject*) {
    m_selectedTags.clear();
    this->updateTags();
}

void FiltersPopup::onClose(CCObject* sender) {
    m_source->setModTags(m_selectedTags);
    Popup::onClose(sender);
}

FiltersPopup* FiltersPopup::create(ModListSource* src) {
    auto ret = new FiltersPopup();
    if (ret && ret->init(350, 250, src)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
