#include "ui/mods/GeodeStyle.hpp"

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/ui/Dropdown.hpp>
#include <Geode/ui/NineSlice.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/utils/ColorProvider.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode;
using namespace geode::cocos;
using namespace cocos2d;

class DropdownOverlay;

class Dropdown::Impl {
public:
    Dropdown* m_self;
    std::vector<std::string> m_options;
    Function<void(std::string const&, size_t)> m_callback;
    size_t m_selectedIndex = 0;
    float m_width;
    bool m_enabled = true;
    int m_savedZOrder = 0;

    CCLabelBMFont* m_label = nullptr;
    NineSlice* m_bg = nullptr;
    CCSprite* m_arrow = nullptr;
    CCMenuItemSpriteExtra* m_button = nullptr;
    CCMenu* m_menu = nullptr;
    DropdownOverlay* m_overlay = nullptr;

    bool init(
        float width, std::vector<std::string> options,
        Function<void(std::string const&, size_t)> callback
    );

    void updateLabel();
    void openOverlay();
    void closeOverlay();
    void selectOption(size_t index);
    void setEnabled(bool enabled);
};

class DropdownOverlay : public CCLayerColor {
    Dropdown::Impl* m_dropdown;
    ScrollLayer* m_scrollLayer = nullptr;
    CCRect m_panelRect;

    float m_itemHeight;
    float m_itemWidth;
    float m_panelPadding;

public:
    static DropdownOverlay* create(Dropdown::Impl* dropdown) {
        auto ret = new DropdownOverlay();

        if (ret->init(dropdown)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }

    bool init(Dropdown::Impl* dropdown) {
        if (!CCLayerColor::initWithColor({0, 0, 0, 0})) return false;

        m_dropdown = dropdown;
        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);

        bool geodeTheme = isGeodeTheme();

        auto winSize = CCDirector::get()->getWinSize();
        auto buttonWorldPos = m_dropdown->m_self->convertToWorldSpace(ccp(0, 0));
        auto buttonSize = m_dropdown->m_self->getContentSize();

        float dropdownWidth = m_dropdown->m_width;
        m_itemHeight = 28.f;
        float itemSpacing = 2.f;
        float totalListHeight = m_itemHeight * m_dropdown->m_options.size() +
            itemSpacing * (m_dropdown->m_options.size() - 1);
        m_panelPadding = 6.f;

        float maxPanelHeight = 200.f;
        float naturalPanelHeight = totalListHeight + m_panelPadding * 2;
        float panelHeight = std::min(naturalPanelHeight, maxPanelHeight);

        float screenMidY = winSize.height / 2.f;
        float panelY;
        if (buttonWorldPos.y + buttonSize.height / 2.f > screenMidY) {
            panelY = buttonWorldPos.y - panelHeight - 2.f;
            if (panelY < 5.f) panelY = 5.f;
        }
        else {
            panelY = buttonWorldPos.y + buttonSize.height + 2.f;
            if (panelY + panelHeight > winSize.height - 5.f) {
                panelY = winSize.height - panelHeight - 5.f;
            }
        }

        float panelX = buttonWorldPos.x + buttonSize.width - dropdownWidth;
        if (panelX + dropdownWidth > winSize.width - 5.f) {
            panelX = winSize.width - dropdownWidth - 5.f;
        }
        if (panelX < 5.f) panelX = 5.f;

        m_panelRect = CCRect(panelX, panelY, dropdownWidth, panelHeight);

        auto panelBG = NineSlice::create(geodeTheme ? "GE_square02.png"_spr : "GJ_square02.png");
        panelBG->setID("dropdown-panel-bg");
        panelBG->setContentSize({dropdownWidth, panelHeight});
        panelBG->setAnchorPoint({0, 0});
        panelBG->setPosition(panelX, panelY);
        this->addChild(panelBG);

        m_itemWidth = dropdownWidth - m_panelPadding * 2;
        float scrollAreaWidth = m_itemWidth;
        float scrollAreaHeight = panelHeight - m_panelPadding * 2;

        m_scrollLayer = ScrollLayer::create({scrollAreaWidth, scrollAreaHeight}, true, true);
        m_scrollLayer->setAnchorPoint({0, 0});
        m_scrollLayer->setPosition(panelX + m_panelPadding, panelY + m_panelPadding);
        this->addChild(m_scrollLayer);

        for (size_t i = 0; i < m_dropdown->m_options.size(); i++) {
            float itemY = totalListHeight - (m_itemHeight + itemSpacing) * i - m_itemHeight;

            auto itemBG = NineSlice::createWithSpriteFrameName("tab-bg.png"_spr);
            itemBG->setScale(.5f);
            itemBG->setContentSize({m_itemWidth / .5f, m_itemHeight / .5f});

            if (i == m_dropdown->m_selectedIndex) {
                itemBG->setColor(to3B(ColorProvider::get()->color("mod-list-tab-selected-bg"_spr)));
            }
            else {
                itemBG->setColor(to3B(ColorProvider::get()->color("mod-list-tab-deselected-bg"_spr)));
            }

            auto label = CCLabelBMFont::create(m_dropdown->m_options[i].c_str(), "bigFont.fnt");
            label->setScale(0.35f / .5f);
            label->setAnchorPoint({0.f, 0.5f});
            label->limitLabelWidth((m_itemWidth - 14.f) / .5f, 0.35f / .5f, 0.1f / .5f);

            if (i == m_dropdown->m_selectedIndex) {
                label->setColor(ccWHITE);
            }
            else {
                label->setColor(ccc3(200, 200, 200));
            }

            itemBG->addChildAtPosition(label, Anchor::Left, ccp(8.f / .5f, 0));

            if (i == m_dropdown->m_selectedIndex) {
                auto check = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
                check->setScale(0.4f / .5f);
                itemBG->addChildAtPosition(check, Anchor::Right, ccp(-10.f / .5f, 0));
            }

            size_t index = i;
            auto btn = Button::createWithNode(itemBG, [this, index](Button*) {
                m_dropdown->selectOption(index);
            });
            btn->setContentSize({m_itemWidth, m_itemHeight});
            btn->setAnchorPoint({0, 0});
            btn->setPosition(0, itemY);
            btn->setTouchPriority(-501);
            btn->setAnimationType(Button::AnimationType::None);
            m_scrollLayer->m_contentLayer->addChild(btn);
        }

        m_scrollLayer->m_contentLayer->setContentSize({scrollAreaWidth, totalListHeight});
        m_scrollLayer->scrollToTop();

        return true;
    }

    void registerWithTouchDispatcher() override {
        CCTouchDispatcher::get()->addTargetedDelegate(this, -500, true);
    }

    bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
        auto loc = touch->getLocation();

        if (m_panelRect.containsPoint(loc)) {
            return false;
        }

        m_dropdown->closeOverlay();
        return true;
    }

    void ccTouchEnded(CCTouch*, CCEvent*) override {}

    void ccTouchCancelled(CCTouch*, CCEvent*) override {}

    void ccTouchMoved(CCTouch*, CCEvent*) override {}

    void keyBackClicked() override {
        m_dropdown->closeOverlay();
    }
};

bool Dropdown::Impl::init(
    float width, std::vector<std::string> options, Function<void(std::string const&, size_t)> callback
) {
    m_width = width;
    m_options = std::move(options);
    m_callback = std::move(callback);

    bool geodeTheme = isGeodeTheme();

    float height = 30.f;
    m_self->setContentSize({width, height});
    m_self->setAnchorPoint({0.5f, 0.5f});

    m_bg = NineSlice::createWithSpriteFrameName("tab-bg.png"_spr);
    m_bg->setScale(.5f);
    m_bg->setContentSize({width / .5f, height / .5f});
    m_bg->setColor(to3B(ColorProvider::get()->color("mod-list-search-bg"_spr)));
    m_self->addChildAtPosition(m_bg, Anchor::Center);

    m_arrow = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
    m_arrow->setScale(0.25f);
    m_arrow->setRotation(90.f);
    m_self->addChildAtPosition(m_arrow, Anchor::Right, ccp(-12, 0));

    m_label = CCLabelBMFont::create("", "bigFont.fnt");
    m_label->setScale(0.35f);
    m_label->setAnchorPoint({0.f, 0.5f});
    m_self->addChildAtPosition(m_label, Anchor::Left, ccp(8, 0));

    auto clickArea = CCSprite::create();
    clickArea->setContentSize({width, height});
    clickArea->setOpacity(0);
    m_button = CCMenuItemSpriteExtra::create(clickArea, m_self, menu_selector(Dropdown::onOpen));
    m_button->setContentSize({width, height});

    m_menu = CCMenu::create();
    m_menu->setContentSize({width, height});
    m_menu->addChildAtPosition(m_button, Anchor::Center);
    m_self->addChildAtPosition(m_menu, Anchor::Center);

    if (!m_options.empty()) {
        updateLabel();
    }

    return true;
}

void Dropdown::Impl::updateLabel() {
    if (m_options.empty()) {
        m_label->setString("");
        return;
    }
    m_label->setString(m_options[m_selectedIndex].c_str());
    m_label->limitLabelWidth(m_width - 30.f, 0.35f, 0.1f);
}

void Dropdown::Impl::openOverlay() {
    if (!m_enabled || m_overlay) return;

    auto scene = CCDirector::get()->getRunningScene();
    if (!scene) return;

    m_savedZOrder = m_self->getZOrder();
    m_self->setZOrder(9998);

    m_overlay = DropdownOverlay::create(this);
    if (m_overlay) {
        CCTouchDispatcher::get()->registerForcePrio(m_overlay, 2);
        scene->addChild(m_overlay, 9999);
    }
}

void Dropdown::Impl::closeOverlay() {
    if (m_overlay) {
        CCTouchDispatcher::get()->unregisterForcePrio(m_overlay);
        m_overlay->removeFromParentAndCleanup(true);
        m_overlay = nullptr;

        m_self->setZOrder(m_savedZOrder);
    }
}

void Dropdown::Impl::selectOption(size_t index) {
    if (index >= m_options.size()) return;
    m_selectedIndex = index;
    updateLabel();
    closeOverlay();
    if (m_callback) {
        m_callback(m_options[m_selectedIndex], m_selectedIndex);
    }
}

void Dropdown::Impl::setEnabled(bool enabled) {
    m_enabled = enabled;
    m_button->setEnabled(enabled);
    GLubyte opacity = enabled ? 255 : 155;
    auto color = enabled ? ccWHITE : ccGRAY;
    m_label->setOpacity(opacity);
    m_label->setColor(color);
    m_arrow->setOpacity(opacity);
    m_arrow->setColor(color);
    m_bg->setOpacity(enabled ? 255 : 155);
}

Dropdown::Dropdown() : m_impl(std::make_unique<Impl>()) {
    m_impl->m_self = this;
}

Dropdown::~Dropdown() {
    if (m_impl->m_overlay) {
        m_impl->closeOverlay();
    }
}

void Dropdown::onOpen(CCObject*) {
    m_impl->openOverlay();
}

bool Dropdown::init(
    float width, std::vector<std::string> options, Function<void(std::string const&, size_t)> callback
) {
    if (!CCNode::init()) return false;
    return m_impl->init(width, std::move(options), std::move(callback));
}

Dropdown* Dropdown::create(
    float width, std::vector<std::string> options, Function<void(std::string const&, size_t)> callback
) {
    auto ret = new Dropdown();
    if (ret->init(width, std::move(options), std::move(callback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void Dropdown::setSelected(size_t index) {
    if (index < m_impl->m_options.size()) {
        m_impl->m_selectedIndex = index;
        m_impl->updateLabel();
    }
}

void Dropdown::setSelected(std::string_view value) {
    for (size_t i = 0; i < m_impl->m_options.size(); i++) {
        if (m_impl->m_options[i] == value) {
            m_impl->m_selectedIndex = i;
            m_impl->updateLabel();
            return;
        }
    }
}

size_t Dropdown::getSelectedIndex() const {
    return m_impl->m_selectedIndex;
}

std::string Dropdown::getSelectedValue() const {
    if (m_impl->m_options.empty()) return "";
    return m_impl->m_options[m_impl->m_selectedIndex];
}

void Dropdown::setEnabled(bool enabled) {
    m_impl->setEnabled(enabled);
}

bool Dropdown::isEnabled() const {
    return m_impl->m_enabled;
}

void Dropdown::setItems(std::vector<std::string> options) {
    m_impl->m_options = std::move(options);
    m_impl->m_selectedIndex = 0;
    m_impl->updateLabel();
}
