#include <Geode/ui/ColorPickPopup.hpp>
#include <Geode/utils/operators.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/binding/SliderThumb.hpp>

USE_GEODE_NAMESPACE();

static GLubyte parseInt(const char* str) {
    try {
        return static_cast<GLubyte>(std::stoi(str));
    } catch(...) {
        return 255;
    }
}

static GLubyte parseFloat(const char* str) {
    try {
        return static_cast<GLubyte>(std::stof(str) * 255.f);
    } catch(...) {
        return 255;
    }
}

bool ColorPickPopup::setup(ccColor4B const& color, bool isRGBA) {
    m_noElasticity = true;
    m_color = color;
    m_originalColor = color;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    this->setTitle("Select Color");

    auto bg = cocos2d::extension::CCScale9Sprite::create(
        "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
    );
    bg->setScale(.5f);
    bg->setColor({ 0, 0, 0 });
    bg->setOpacity(85);
    bg->setContentSize({
        m_size.width * 2 - 40.f,
        m_size.height * 2 - 140.f
    });
    bg->setPosition(winSize / 2);
    m_mainLayer->addChild(bg);

    // picker

    m_picker = CCControlColourPicker::colourPicker();
    m_picker->setPosition(
        winSize.width / 2 - 45.f,
        winSize.height / 2 + (isRGBA ? 25.f : 0.f)
    );
    m_picker->setDelegate(this);
    m_mainLayer->addChild(m_picker);

    // color difference

    auto oldColorSpr = CCSprite::createWithSpriteFrameName("whiteSquare60_001.png");
    oldColorSpr->setPosition({
        winSize.width / 2 - 165.f,
        winSize.height / 2 + 15.f
    });
    oldColorSpr->setColor(to3B(m_color));
    m_mainLayer->addChild(oldColorSpr);

    m_newColorSpr = CCSprite::createWithSpriteFrameName("whiteSquare60_001.png");
    m_newColorSpr->setPosition({
        winSize.width / 2 - 165.f,
        winSize.height / 2 - 15.f
    });
    m_newColorSpr->setColor(to3B(m_color));
    m_mainLayer->addChild(m_newColorSpr);

    auto resetBtnSpr = ButtonSprite::create(
        CCSprite::createWithSpriteFrameName("reset-gold.png"_spr),
        0x20, true, 0.f, "GJ_button_01.png", 1.25f
    );
    resetBtnSpr->setScale(.6f);

    m_resetBtn = CCMenuItemSpriteExtra::create(
        resetBtnSpr, this, menu_selector(ColorPickPopup::onReset)
    );
    m_resetBtn->setPosition({ -165.f, -50.f });
    m_buttonMenu->addChild(m_resetBtn);

    // r

    auto rText = CCLabelBMFont::create("R", "goldFont.fnt");
    rText->setPosition(
        winSize.width / 2 + 75.f,
        winSize.height / 2 + (isRGBA ? 60.f : 35.f)
    );
    rText->setScale(.55f);
    m_mainLayer->addChild(rText);
    
    m_rInput = InputNode::create(50.f, "R");
    m_rInput->setPosition(75.f, (isRGBA ? 40.f : 15.f));
    m_rInput->setScale(.7f);
    m_rInput->getInput()->setTag(TAG_R_INPUT);
    m_rInput->getInput()->setDelegate(this);
    m_buttonMenu->addChild(m_rInput);

    // g

    auto gText = CCLabelBMFont::create("G", "goldFont.fnt");
    gText->setPosition(
        winSize.width / 2 + 115.f,
        winSize.height / 2 + (isRGBA ? 60.f : 35.f)
    );
    gText->setScale(.55f);
    m_mainLayer->addChild(gText);
    
    m_gInput = InputNode::create(50.f, "G");
    m_gInput->setPosition(115.f, (isRGBA ? 40.f : 15.f));
    m_gInput->setScale(.7f);
    m_gInput->getInput()->setTag(TAG_G_INPUT);
    m_gInput->getInput()->setDelegate(this);
    m_buttonMenu->addChild(m_gInput);

    // b

    auto bText = CCLabelBMFont::create("B", "goldFont.fnt");
    bText->setPosition(
        winSize.width / 2 + 155.f,
        winSize.height / 2 + (isRGBA ? 60.f : 35.f)
    );
    bText->setScale(.55f);
    m_mainLayer->addChild(bText);
    
    m_bInput = InputNode::create(50.f, "B");
    m_bInput->setPosition(155.f, (isRGBA ? 40.f : 15.f));
    m_bInput->setScale(.7f);
    m_bInput->getInput()->setTag(TAG_B_INPUT);
    m_bInput->getInput()->setDelegate(this);
    m_buttonMenu->addChild(m_bInput);

    // hex

    auto hexText = CCLabelBMFont::create("Hex", "goldFont.fnt");
    hexText->setPosition(
        winSize.width / 2 + 115.f,
        winSize.height / 2 + (isRGBA ? 20.f : -5.f)
    );
    hexText->setScale(.55f);
    m_mainLayer->addChild(hexText);
    
    m_hexInput = InputNode::create(165.f, "Hex");
    m_hexInput->setPosition(115.f, (isRGBA ? 0.f : -25.f));
    m_hexInput->setScale(.7f);
    m_hexInput->getInput()->setTag(TAG_HEX_INPUT);
    m_hexInput->getInput()->setDelegate(this);
    m_buttonMenu->addChild(m_hexInput);

    if (isRGBA) {
        auto opacityText = CCLabelBMFont::create("Opacity", "goldFont.fnt");
        opacityText->setPosition(
            winSize.width / 2 - 30.f,
            winSize.height / 2 - 75.f
        );
        opacityText->setScale(.55f);
        m_mainLayer->addChild(opacityText);

        m_opacitySlider = Slider::create(
            this,
            menu_selector(ColorPickPopup::onOpacitySlider),
            .75f
        );
        m_opacitySlider->setPosition(
            winSize.width / 2 - 30.f,
            winSize.height / 2 - 95.f
        );
        m_opacitySlider->setValue(color.a / 255.f);
        m_opacitySlider->updateBar();
        m_mainLayer->addChild(m_opacitySlider);

        m_opacityInput = InputNode::create(60.f, "0.00");
        m_opacityInput->setPosition(85.f, -95.f);
        m_opacityInput->setScale(.7f);
        m_opacityInput->getInput()->setTag(TAG_OPACITY_INPUT);
        m_opacityInput->getInput()->setDelegate(this);
        m_buttonMenu->addChild(m_opacityInput);
    }

    this->updateState();

    auto okBtnSpr = ButtonSprite::create("OK");
    okBtnSpr->setScale(.7f);

    auto okBtn = CCMenuItemSpriteExtra::create(
        okBtnSpr, this, menu_selector(ColorPickPopup::onClose)
    );
    okBtn->setPosition(.0f, -m_size.height / 2 + 20.f);
    m_buttonMenu->addChild(okBtn);

    return true;
}

void ColorPickPopup::updateState(CCNode* except) {
    #define IF_NOT_EXCEPT(inp, value)\
        if (inp->getInput() != except) {\
            inp->getInput()->setDelegate(nullptr);\
            inp->setString(value);\
            inp->getInput()->setDelegate(this);\
        }

    IF_NOT_EXCEPT(m_rInput, numToString<int>(m_color.r));
    IF_NOT_EXCEPT(m_gInput, numToString<int>(m_color.g));
    IF_NOT_EXCEPT(m_bInput, numToString<int>(m_color.b));
    IF_NOT_EXCEPT(m_hexInput, cc3bToHexString(to3B(m_color)));
    if (m_opacityInput) {
        IF_NOT_EXCEPT(
            m_opacityInput,
            numToString(m_color.a / 255.f, 2)
        );
    }
    if (m_opacitySlider) {
        m_opacitySlider->setValue(m_color.a / 255.f);
        m_opacitySlider->updateBar();
    }
    if (m_picker != except) {
        m_picker->setDelegate(nullptr);
        m_picker->setColorValue(to3B(m_color));
        m_picker->setDelegate(this);
    }
    m_resetBtn->setVisible(m_originalColor != m_color);
    m_newColorSpr->setColor(to3B(m_color));
    if (m_delegate) m_delegate->updateColor(m_color);
}

void ColorPickPopup::onOpacitySlider(CCObject* sender) {
    m_color.a = static_cast<GLubyte>(
        static_cast<SliderThumb*>(sender)->getValue() * 255.f
    );
    this->updateState();
}

void ColorPickPopup::onReset(CCObject*) {
    m_color = m_originalColor;
    this->updateState();
}

void ColorPickPopup::textChanged(CCTextInputNode* input) {
    if (input->getString() && strlen(input->getString())) {
        switch (input->getTag()) {
            case TAG_HEX_INPUT: {
                if (auto color = cc3bFromHexString(input->getString())) {
                    m_color.r = color.value().r;
                    m_color.g = color.value().g;
                    m_color.b = color.value().b;
                }
            } break;
            
            case TAG_OPACITY_INPUT: {
                m_color.a = parseFloat(input->getString());
            } break;
            
            case TAG_R_INPUT: m_color.r = parseInt(input->getString()); break;
            case TAG_G_INPUT: m_color.g = parseInt(input->getString()); break;
            case TAG_B_INPUT: m_color.b = parseInt(input->getString()); break;

            default: break;
        }
    }
    this->updateState(input);
}

void ColorPickPopup::colorValueChanged(ccColor3B color) {
    m_color.r = color.r;
    m_color.g = color.g;
    m_color.b = color.b;
    this->updateState(m_picker);
}

void ColorPickPopup::setDelegate(ColorPickPopupDelegate* delegate) {
    m_delegate = delegate;
}

void ColorPickPopup::setColorTarget(cocos2d::CCSprite* spr) {
    m_picker->setColorTarget(spr);
}

ColorPickPopup* ColorPickPopup::create(ccColor4B const& color, bool isRGBA) {
    auto ret = new ColorPickPopup();
    if (ret && ret->init(
        400.f, (isRGBA ? 290.f : 240.f), color, isRGBA
    )) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ColorPickPopup* ColorPickPopup::create(bool isRGBA) {
    return ColorPickPopup::create({ 255, 255, 255, 255 }, isRGBA);
}

ColorPickPopup* ColorPickPopup::create(ccColor3B const& color) {
    return ColorPickPopup::create(to4B(color), false);
}

ColorPickPopup* ColorPickPopup::create(ccColor4B const& color) {
    return ColorPickPopup::create(color, true);
}
