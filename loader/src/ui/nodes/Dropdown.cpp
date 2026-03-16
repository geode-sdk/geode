#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/ui/Dropdown.hpp>
#include <Geode/ui/NineSlice.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode;
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

    struct ItemRect {
        CCRect rect;
        size_t index;
    };

    std::vector<ItemRect> m_itemRects;

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
        if (!CCLayerColor::initWithColor({0, 0, 0, 85})) return false;

        m_dropdown = dropdown;
        this->setKeypadEnabled(true);

        auto winSize = CCDirector::get()->getWinSize();
        auto buttonWorldPos = m_dropdown->m_self->convertToWorldSpace(ccp(0, 0));
        auto buttonSize = m_dropdown->m_self->getContentSize();

        float dropdownWidth = m_dropdown->m_width;
        float itemHeight = 25.f;
        float listHeight = itemHeight * m_dropdown->m_options.size();
        float panelPadding = 4.f;
        float panelHeight = listHeight + panelPadding * 2;
        bool openBelow = true;
        float panelY = buttonWorldPos.y - panelHeight;

        if (panelY < 5.f) {
            openBelow = false;
            panelY = buttonWorldPos.y + buttonSize.height;
        }

        float panelX = buttonWorldPos.x;
        if (panelX + dropdownWidth > winSize.width - 5.f) {
            panelX = winSize.width - dropdownWidth - 5.f;
        }
        if (panelX < 5.f) panelX = 5.f;

        auto panelBG = NineSlice::create("square02b_001.png", {0, 0, 80, 80});
        panelBG->setColor({0, 0, 0});
        panelBG->setOpacity(200);
        panelBG->setContentSize({dropdownWidth / 0.25f, panelHeight / 0.25f});
        panelBG->setScale(0.25f);
        panelBG->setAnchorPoint({0, 0});
        panelBG->setPosition(panelX, panelY);
        this->addChild(panelBG);

        float itemWidth = dropdownWidth - panelPadding * 2;
        for (size_t i = 0; i < m_dropdown->m_options.size(); i++) {
            float itemY;
            if (openBelow) {
                itemY = panelY + panelHeight - panelPadding - itemHeight * i - itemHeight;
            }
            else {
                itemY = panelY + panelPadding + itemHeight * (m_dropdown->m_options.size() - 1 - i);
            }
            float itemX = panelX + panelPadding;

            auto itemBG = NineSlice::create("square02b_001.png", {0, 0, 80, 80});
            itemBG->setScale(0.25f);
            if (i == m_dropdown->m_selectedIndex) {
                itemBG->setColor({80, 80, 80});
                itemBG->setOpacity(200);
            }
            else {
                itemBG->setColor({0, 0, 0});
                itemBG->setOpacity(0);
            }
            itemBG->setContentSize({itemWidth / 0.25f, itemHeight / 0.25f});
            itemBG->setAnchorPoint({0, 0});
            itemBG->setPosition(itemX, itemY);
            this->addChild(itemBG);

            auto label = CCLabelBMFont::create(m_dropdown->m_options[i].c_str(), "bigFont.fnt");
            label->setScale(0.4f / 0.25f);
            label->setAnchorPoint({0.f, 0.5f});
            label->limitLabelWidth((itemWidth - 10.f) / 0.25f, 0.4f / 0.25f, 0.1f / 0.25f);
            itemBG->addChildAtPosition(label, Anchor::Left, ccp(5.f / 0.25f, 0));

            m_itemRects.push_back({CCRect(itemX, itemY, itemWidth, itemHeight), i});
        }

        return true;
    }

    void registerWithTouchDispatcher() override {
        CCTouchDispatcher::get()->addTargetedDelegate(this, -500, true);
    }

    bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
        auto loc = touch->getLocation();

        for (auto const& item : m_itemRects) {
            if (item.rect.containsPoint(loc)) {
                m_dropdown->selectOption(item.index);
                return true;
            }
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

    float height = 30.f;
    m_self->setContentSize({width, height});
    m_self->setAnchorPoint({0.5f, 0.5f});

    m_bg = NineSlice::create("square02b_001.png", {0, 0, 80, 80});
    m_bg->setColor({0, 0, 0});
    m_bg->setOpacity(90);
    m_bg->setContentSize({width / 0.25f, height / 0.25f});
    m_bg->setScale(0.25f);
    m_self->addChildAtPosition(m_bg, Anchor::Center);

    m_arrow = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
    m_arrow->setScale(0.3f);
    m_arrow->setRotation(90.f);
    m_self->addChildAtPosition(m_arrow, Anchor::Right, ccp(-10, 0));

    m_label = CCLabelBMFont::create("", "bigFont.fnt");
    m_label->setScale(0.4f);
    m_label->setAnchorPoint({0.f, 0.5f});
    m_self->addChildAtPosition(m_label, Anchor::Left, ccp(5, 0));

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
    m_label->limitLabelWidth(m_width - 25.f, 0.4f, 0.1f);
}

void Dropdown::Impl::openOverlay() {
    if (!m_enabled || m_overlay) return;

    auto scene = CCDirector::get()->getRunningScene();
    if (!scene) return;

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
