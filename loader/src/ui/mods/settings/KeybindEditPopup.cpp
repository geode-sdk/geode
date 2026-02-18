#include "KeybindEditPopup.hpp"

#include <Geode/ui/Scrollbar.hpp>
#include <Geode/ui/SimpleAxisLayout.hpp>

CCNode* createKeybindButton(Keybind const& keybind) {
    if (keybind.key >= CONTROLLER_A && keybind.key <= CONTROLLER_RTHUMBSTICK_RIGHT) {
        return createGeodeButton(keybind.createNode(), "");
    }
    return createGeodeButton(nullptr, keybind.toString(), true);
}

bool KeybindEditPopup::init(
    std::shared_ptr<KeybindSettingV3> setting,
    Keybind const& keybind,
    Function<void(Keybind const&)> callback
) {
    if (!GeodePopup::init(220, 170))
        return false;

    this->setTitle(setting->getDisplayName());
    m_noElasticity = true;

    if (auto mod = setting->getMod()) {
        auto fromModLabel = CCLabelBMFont::create(mod->getName().c_str(), "bigFont.fnt");
        fromModLabel->setScale(.4f);
        fromModLabel->setColor(ccc3(55, 155, 255));
        m_mainLayer->addChildAtPosition(fromModLabel, Anchor::Top, ccp(0, -40));
    }

    m_setting = setting;
    m_callback = std::move(callback);
    m_currentKeybind = keybind;
    auto bottomMenu = CCMenu::create();
    bottomMenu->setContentWidth(220.f);

    if (keybind.key == KEY_None) {
        auto addButton = CCMenuItemSpriteExtra::create(
            createGeodeButton("Add", true), this, menu_selector(KeybindEditPopup::onSet)
        );
        bottomMenu->addChild(addButton);
    }
    else {
        m_originalKeybind = keybind;

        auto setButton = CCMenuItemSpriteExtra::create(
            createGeodeButton("Set", true), this, menu_selector(KeybindEditPopup::onSet)
        );
        bottomMenu->addChild(setButton);

        auto removeButton = CCMenuItemSpriteExtra::create(
            createGeodeButton("Remove", true, GeodeButtonSprite::Delete), this, menu_selector(KeybindEditPopup::onRemove)
        );
        bottomMenu->addChild(removeButton);
    }

    bottomMenu->setLayout(RowLayout::create()->setGap(10.f));
    m_mainLayer->addChildAtPosition(bottomMenu, Anchor::Bottom, ccp(0, 25));

    this->addEventListener(KeyboardInputEvent(), [this](KeyboardInputData& data) {
        if (data.action == KeyboardInputData::Action::Press) {
            m_currentKeybind.key = data.key;
            m_currentKeybind.modifiers = data.modifiers;
            this->updateLabel();
        }
    });
    this->addEventListener(MouseInputEvent(), [this](MouseInputData& data) {
        auto key = MouseInputData::buttonToKeyCode(data.button);
        if (key != KEY_None && data.action == MouseInputData::Action::Press) {
            m_currentKeybind.key = key;
            m_currentKeybind.modifiers = data.modifiers;
            this->updateLabel();
        }
    });
    this->updateLabel();

    return true;
}

void KeybindEditPopup::updateLabel() {
    if (m_originalKeybind) {
        if (m_originalKeybindContainer) {
            m_originalKeybindContainer->removeFromParent();
        }
        if (*m_originalKeybind != m_currentKeybind) {
            m_originalKeybindContainer = CCNode::create();
            m_originalKeybindContainer->setContentWidth(200);
            m_originalKeybindContainer->setScale(.4f);
            m_originalKeybindContainer->setAnchorPoint(ccp(.5f, .5f));

            auto originalKeybindInfoStart = CCLabelBMFont::create("(Previous: ", "bigFont.fnt");
            originalKeybindInfoStart->setColor(ccc3(55, 255, 55));
            m_originalKeybindContainer->addChild(originalKeybindInfoStart);

            auto originalKeybind = m_originalKeybind->createNode();
            m_originalKeybindContainer->addChild(originalKeybind);
            
            auto originalKeybindInfoEnd = CCLabelBMFont::create(")", "bigFont.fnt");
            originalKeybindInfoEnd->setColor(ccc3(55, 255, 55));
            m_originalKeybindContainer->addChild(originalKeybindInfoEnd);

            m_originalKeybindContainer->setLayout(SimpleRowLayout::create());
            m_mainLayer->addChildAtPosition(m_originalKeybindContainer, Anchor::Bottom, ccp(0, 60));
        }
    }

    if (m_keybindNode) {
        m_keybindNode->removeFromParent();
    }
    if (m_currentKeybind.key == KEY_None && m_currentKeybind.modifiers == KeyboardModifier::None) {
        auto label = CCLabelBMFont::create("Press a key...", "bigFont.fnt");
        label->setOpacity(150);
        label->setScale(.75f);
        m_keybindNode = label;
    }
    else {
        m_keybindNode = m_currentKeybind.createNode();
        // It just looks better if controller binds are scale 1
        if (!typeinfo_cast<CCSprite*>(m_keybindNode)) {
            limitNodeWidth(m_keybindNode, m_mainLayer->getContentWidth() - 10, .75f, .1f);
        }
    }
    float keybindOffset = m_originalKeybindContainer ? 10 : 0;
    m_mainLayer->addChildAtPosition(m_keybindNode, Anchor::Center, ccp(0, keybindOffset));
}

void KeybindEditPopup::onSet(CCObject*) {
    if (m_currentKeybind.key == KEY_None || m_currentKeybind.key == KEY_Unknown) {
        return;
    }
    m_callback(m_currentKeybind);
    this->onClose(nullptr);
}

void KeybindEditPopup::onRemove(CCObject*) {
    m_callback(Keybind());
    this->onClose(nullptr);
}

KeybindEditPopup* KeybindEditPopup::create(
    std::shared_ptr<KeybindSettingV3> setting,
    Keybind const& keybind,
    Function<void(Keybind const&)> callback
) {
    auto ret = new KeybindEditPopup();
    if (ret->init(setting, keybind, std::move(callback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool KeybindListPopup::init(
    std::shared_ptr<KeybindSettingV3> setting,
    std::vector<Keybind> const& keybinds,
    Function<void(std::vector<Keybind>)> callback
) {
    if (!GeodePopup::init(220.f, 250.f))
        return false;

    this->setTitle(setting->getDisplayName());
    m_noElasticity = true;

    m_callback = std::move(callback);
    m_setting = setting;
    m_currentKeybinds = keybinds;
    m_hasChanged = false;

    auto background = CCLayerColor::create(ccc4(0, 0, 0, 75), 190.f, 150.f);
    background->ignoreAnchorPointForPosition(false);
    m_mainLayer->addChildAtPosition(background, Anchor::Center, ccp(-5, 5));

    m_scrollLayer = ScrollLayer::create({ 190.f, 150.f });
    m_scrollLayer->ignoreAnchorPointForPosition(false);
    m_scrollLayer->m_contentLayer->setLayout(ColumnLayout::create()->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End)->setAutoGrowAxis(0));
    m_mainLayer->addChildAtPosition(m_scrollLayer, Anchor::Center, ccp(-5, 5));

    m_mainLayer->addChildAtPosition(Scrollbar::create(m_scrollLayer), Anchor::Right, ccp(-10, 5));

    m_mainLayer->addChildAtPosition(createGeodeListBorders({ 190.f, 150.f }), Anchor::Center, ccp(-5, 5));

    auto bottomMenu = CCMenu::create();
    bottomMenu->setContentWidth(220.f);

    auto addButton = CCMenuItemSpriteExtra::create(
        createGeodeButton("Add", true), this, menu_selector(KeybindListPopup::onAdd)
    );
    bottomMenu->addChild(addButton);

    auto saveButton = CCMenuItemSpriteExtra::create(
        createGeodeButton("Save", true), this, menu_selector(KeybindListPopup::onSave)
    );
    bottomMenu->addChild(saveButton);

    bottomMenu->setLayout(RowLayout::create()->setGap(10.f));
    m_mainLayer->addChildAtPosition(bottomMenu, Anchor::Bottom, ccp(0, 25));

    this->updateKeybinds();

    return true;
}

void KeybindListPopup::updateKeybinds() {
    m_scrollLayer->m_contentLayer->removeAllChildren();
    size_t index = 0;
    for (auto& keybind : m_currentKeybinds) {
        auto bspr = createKeybindButton(keybind);
        auto button = CCMenuItemSpriteExtra::create(bspr, this, menu_selector(KeybindListPopup::onKeybind));
        button->setTag(index);
        auto menu = CCMenu::createWithItem(button);
        menu->setContentSize({ 190.f, button->getScaledContentHeight() });
        button->setPosition({ 95.f, button->getScaledContentHeight() / 2 });
        m_scrollLayer->m_contentLayer->addChild(menu);
        index += 1;
    }
    m_scrollLayer->m_contentLayer->updateLayout();
    m_scrollLayer->scrollToTop();
}

void KeybindListPopup::onAdd(CCObject*) {
    KeybindEditPopup::create(m_setting, Keybind(), [this](Keybind const& newKeybind) {
        if (std::ranges::contains(m_currentKeybinds, newKeybind)) return;
        m_hasChanged = true;
        m_currentKeybinds.push_back(newKeybind);
        this->updateKeybinds();
    })->show();
}

void KeybindListPopup::onSave(CCObject*) {
    m_callback(m_currentKeybinds);
    Popup::onClose(nullptr);
}

void KeybindListPopup::onKeybind(CCObject* sender) {
    auto index = sender->getTag();
    KeybindEditPopup::create(m_setting, m_currentKeybinds[index], [this, index](Keybind const& newKeybind) {
        if (m_currentKeybinds[index] == newKeybind) return;
        m_hasChanged = true;
        if (newKeybind.key == KEY_None || std::ranges::contains(m_currentKeybinds, newKeybind)) {
            m_currentKeybinds.erase(m_currentKeybinds.begin() + index);
        }
        else {
            m_currentKeybinds[index] = newKeybind;
        }
        this->updateKeybinds();
    })->show();
}

void KeybindListPopup::onClose(CCObject*) {
    if (m_hasChanged) {
        createQuickPopup(
            "Unsaved Changes",
            "You have <cr>unsaved changes</c>! Are you sure you want to exit?",
            "Cancel", "Discard",
            [this](auto, bool btn2) {
                if (btn2) {
                    Popup::onClose(nullptr);
                }
            }
        );
    }
    else {
        Popup::onClose(nullptr);
    }
}

KeybindListPopup* KeybindListPopup::create(
    std::shared_ptr<KeybindSettingV3> setting,
    std::vector<Keybind> const& keybinds,
    Function<void(std::vector<Keybind>)> callback
) {
    auto ret = new KeybindListPopup();
    if (ret->init(setting, keybinds, std::move(callback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
