#include "SortPopup.hpp"

bool SortPopup::setup(ModListSource* src) {
    m_noElasticity = true;
    m_source = src;

    this->setTitle("Search Sorting");

    auto container = CCNode::create();
    container->setContentSize({ 200, 115 });

    if (auto server = typeinfo_cast<ServerModListSource*>(src)) {
        for (auto& [name, sort] : std::initializer_list<std::pair<const char*, server::ModsSort>> {
            { "Most Downloads", server::ModsSort::Downloads },
            { "Recently Published", server::ModsSort::RecentlyPublished },
            { "Recently Updated", server::ModsSort::RecentlyUpdated },
        }) {
            auto node = CCMenu::create();
            node->setContentSize({ container->getContentWidth(), 22 });

            auto toggle = CCMenuItemToggler::createWithStandardSprites(
                this, menu_selector(SortPopup::onSelect), .6f
            );
            toggle->m_notClickable = true;
            toggle->setUserData(reinterpret_cast<void*>(sort));
            if (server->getQuery().sorting == sort) {
                toggle->toggle(true);
                m_selected = static_cast<size_t>(sort);
            }
            node->addChildAtPosition(toggle, Anchor::Left, ccp(15, 0));
            m_options.push_back(toggle);

            auto label = CCLabelBMFont::create(name, "bigFont.fnt");
            label->setScale(.5f);
            node->addChildAtPosition(label, Anchor::Left, ccp(30, 0), ccp(0, .5f));

            container->addChild(node);
        }
    }

    container->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
    );
    m_mainLayer->addChildAtPosition(container, Anchor::Center, ccp(0, -5), ccp(.5f, .5f));

    return true;
}

void SortPopup::onSelect(CCObject* sender) {
    m_selected = reinterpret_cast<uintptr_t>(static_cast<CCMenuItemToggler*>(sender)->getUserData());
    for (auto option : m_options) {
        option->toggle(m_selected == reinterpret_cast<uintptr_t>(option->getUserData()));
    }
}

void SortPopup::onClose(CCObject* sender) {
    if (auto server = typeinfo_cast<ServerModListSource*>(m_source)) {
        server->getQueryMut()->sorting = static_cast<server::ModsSort>(m_selected);
    }
    Popup::onClose(sender);
}

SortPopup* SortPopup::create(ModListSource* src) {
    auto ret = new SortPopup();
    if (ret->init(230, 165, src)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
