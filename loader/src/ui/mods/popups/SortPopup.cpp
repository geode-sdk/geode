#include "SortPopup.hpp"

#include <Geode/ui/Label.hpp>

bool SortPopup::init(ModListSource* src) {
    auto opts = src->getSortingOptions();
    auto height = opts.size() * 25.0f;

    if (!GeodePopup::init(230.0f, 50.0f + height)) return false;

    m_noElasticity = true;
    m_source = src;

    this->setTitle("Search Sorting");

    auto container = CCNode::create();
    container->setContentSize({ 200.0f, height });

    for (auto& [sort, name] : opts) {
        auto node = CCNode::create();
        node->setContentSize({ container->getContentWidth(), 22.0f });

        auto toggle = Toggler::createWithStandardSprites(0.6f, [this](Toggler* sender, bool toggled) {
            sender->setToggled(!toggled);
            this->onSelect(sender);
        });
        toggle->setTag(sort);
        if (src->getSort() == sort) {
            toggle->toggle(true);
            m_selected = sort;
        }
        node->addChildAtPosition(toggle, Anchor::Left, {15.0f, 0.0f});
        m_options.push_back(toggle);

        auto label = Label::create(std::move(name), "bigFont.fnt");
        label->setScale(.5f);
        node->addChildAtPosition(label, Anchor::Left, {30.0f, 0.0f}, {0.0f, 0.5f});

        container->addChild(node);
    }

    container->setLayout(ColumnLayout::create()->setAxisReverse(true));
    m_mainLayer->addChildAtPosition(container, Anchor::Center, {0.0f, -10.0f}, {0.5f, 0.5f});

    return true;
}

void SortPopup::onSelect(CCObject* sender) {
    m_selected = sender->getTag();
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
