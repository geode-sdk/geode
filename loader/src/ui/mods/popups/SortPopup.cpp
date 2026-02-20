#include "SortPopup.hpp"

bool SortPopup::init(ModListSource* src) {
    if (!GeodePopup::init(230.f, 165.f))
        return false;
    
    m_noElasticity = true;
    m_source = src;

    this->setTitle("Search Sorting");

    auto container = CCNode::create();
    container->setContentSize({ 200, 115 });

    for (auto const& [sort, name] : src->getSortingOptions()) {
        auto node = CCMenu::create();
        node->setContentSize({ container->getContentWidth(), 22 });

        auto toggle = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(SortPopup::onSelect), .6f
        );
        toggle->m_notClickable = true;
        toggle->setTag(sort);
        if (src->getSort() == sort) {
            toggle->toggle(true);
            m_selected = sort;
        }
        node->addChildAtPosition(toggle, Anchor::Left, ccp(15, 0));
        m_options.push_back(toggle);

        auto label = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
        label->setScale(.5f);
        node->addChildAtPosition(label, Anchor::Left, ccp(30, 0), ccp(0, .5f));

        container->addChild(node);
    }

    container->setLayout(ColumnLayout::create()->setAxisReverse(true));
    m_mainLayer->addChildAtPosition(container, Anchor::Center, ccp(0, -5), ccp(.5f, .5f));

    return true;
}

void SortPopup::onSelect(CCObject* sender) {
    m_selected = static_cast<CCMenuItemToggler*>(sender)->getTag();
    for (auto option : m_options) {
        option->toggle(m_selected == option->getTag());
    }
}

void SortPopup::onClose(CCObject* sender) {
    m_source->setSort(m_selected);
    Popup::onClose(sender);
}

SortPopup* SortPopup::create(ModListSource* src) {
    auto ret = new SortPopup();
    if (ret->init(src)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
