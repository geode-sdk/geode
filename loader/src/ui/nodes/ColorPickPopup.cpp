#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/binding/SliderThumb.hpp>
#include <Geode/ui/ColorPickPopup.hpp>
#include <Geode/utils/cocos.hpp>
#include <charconv>
#include <clocale>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

class ColorPickPopup::Impl final {
public:
    cocos2d::ccColor4B m_color;
    cocos2d::ccColor4B m_originalColor;
    cocos2d::extension::CCControlColourPicker* m_picker;
    Slider* m_opacitySlider = nullptr;
    TextInput* m_rInput;
    TextInput* m_gInput;
    TextInput* m_bInput;
    TextInput* m_hexInput;
    TextInput* m_opacityInput = nullptr;
    ColorPickPopupDelegate* m_delegate = nullptr;
    cocos2d::CCSprite* m_newColorSpr;
    CCMenuItemSpriteExtra* m_resetBtn;
};

ColorPickPopup::ColorPickPopup() {
    m_impl = std::make_unique<Impl>();
}

ColorPickPopup::~ColorPickPopup() {}


bool ColorPickPopup::setup(ccColor4B const& color, bool isRGBA) {
    m_noElasticity = true;
    m_impl->m_color = color;
    m_impl->m_originalColor = color;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    this->setTitle("Select Color");

    auto bg = cocos2d::extension::CCScale9Sprite::create(
        "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
    );
    bg->setID("popup-bg");
    bg->setScale(.5f);
    bg->setColor({ 0, 0, 0 });
    bg->setOpacity(85);
    bg->setContentSize({ m_size.width * 2 - 40.f, m_size.height * 2 - 140.f });
    m_mainLayer->addChildAtPosition(bg, Anchor::Center, ccp(0, 0));

    auto opacityColumn = CCLayer::create();
    opacityColumn->setLayout(
        ColumnLayout::create()
            ->setGap(5.f)
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAutoGrowAxis(0)
    );
    opacityColumn->setID("opacity-column");
    m_mainLayer->addChildAtPosition(opacityColumn, Anchor::Center, ccp(0, 0));

    auto pickerRow = CCLayer::create();
    pickerRow->setLayout(
        RowLayout::create()
            ->setGap(10.f)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAutoGrowAxis(0)
    );
    pickerRow->setID("picker-row");
    opacityColumn->addChild(pickerRow);

    // color difference

    auto colorMenu = CCMenu::create();
    colorMenu->setLayout(
        ColumnLayout::create()
            ->setGap(0.f)
            ->setAxisReverse(true)
            ->setAutoScale(false)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAutoGrowAxis(0)
    );
    colorMenu->setID("color-menu");
    pickerRow->addChild(colorMenu);

    // picker

    m_impl->m_picker = CCControlColourPicker::colourPicker();
    m_impl->m_picker->setDelegate(this);
    m_impl->m_picker->setID("color-picker");

    auto pickerWrapper = CCNode::create();
    pickerWrapper->setContentSize(m_impl->m_picker->getContentSize());
    pickerWrapper->setID("picker-wrapper");
    pickerWrapper->addChildAtPosition(m_impl->m_picker, Anchor::Center, ccp(0, 0));
    pickerRow->addChild(pickerWrapper);

    auto oldColorSpr = CCSprite::createWithSpriteFrameName("whiteSquare60_001.png");
    oldColorSpr->setColor(to3B(m_impl->m_color));
    oldColorSpr->setID("old-color-spr");
    colorMenu->addChild(oldColorSpr);

    m_impl->m_newColorSpr = CCSprite::createWithSpriteFrameName("whiteSquare60_001.png");
    m_impl->m_newColorSpr->setColor(to3B(m_impl->m_color));
    m_impl->m_newColorSpr->setID("new-color-spr");
    colorMenu->addChild(m_impl->m_newColorSpr);

    auto resetBtnSpr = ButtonSprite::create(
        CCSprite::createWithSpriteFrameName("reset-gold.png"_spr), 0x20, true, 0.f,
        "GJ_button_01.png", 1.25f
    );
    resetBtnSpr->setScale(.6f);

    m_impl->m_resetBtn =
        CCMenuItemSpriteExtra::create(resetBtnSpr, this, menu_selector(ColorPickPopup::onReset));
    m_impl->m_resetBtn->setPosition({ -165.f, -50.f });
    m_impl->m_resetBtn->setLayoutOptions(
        AxisLayoutOptions::create()
            ->setPrevGap(10.f)
            ->setNextGap(10.f)
    );
    m_impl->m_resetBtn->setID("reset-btn");
    colorMenu->addChild(m_impl->m_resetBtn);



    auto inputColumn = CCLayer::create();
    inputColumn->setLayout(
        ColumnLayout::create()
            ->setGap(3.f)
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAutoGrowAxis(0)
    );
    inputColumn->setID("input-column");
    pickerRow->addChild(inputColumn);

    auto rgbRow = CCLayer::create();
    rgbRow->setLayout(
        RowLayout::create()
            ->setGap(5.f)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAutoGrowAxis(0)
    );
    rgbRow->setID("rgb-row");
    inputColumn->addChild(rgbRow);

    // r

    auto rColumn = CCLayer::create();
    rColumn->setLayout(
        ColumnLayout::create()
            ->setGap(3.f)
            ->setAxisReverse(true)
            ->setAutoScale(false)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAutoGrowAxis(0)
    );
    rColumn->setID("r-column");
    rgbRow->addChild(rColumn);

    auto rText = CCLabelBMFont::create("R", "goldFont.fnt");
    rText->setScale(.55f);
    rText->setID("r-text");
    rColumn->addChild(rText);

    m_impl->m_rInput = TextInput::create(50.f, "R");
    m_impl->m_rInput->setScale(.7f);
    m_impl->m_rInput->setDelegate(this, TAG_R_INPUT);
    m_impl->m_rInput->setID("r-input");
    rColumn->addChild(m_impl->m_rInput);

    rColumn->updateLayout();
    auto rRect = calculateChildCoverage(rColumn);
    rColumn->setContentSize(rRect.size);

    // g

    auto gColumn = CCLayer::create();
    gColumn->setLayout(
        ColumnLayout::create()
            ->setGap(3.f)
            ->setAxisReverse(true)
            ->setAutoScale(false)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAutoGrowAxis(0)
    );
    gColumn->setID("g-column");
    rgbRow->addChild(gColumn);

    auto gText = CCLabelBMFont::create("G", "goldFont.fnt");
    gText->setScale(.55f);
    gText->setID("g-text");
    gColumn->addChild(gText);

    m_impl->m_gInput = TextInput::create(50.f, "G");
    m_impl->m_gInput->setScale(.7f);
    m_impl->m_gInput->setDelegate(this, TAG_G_INPUT);
    m_impl->m_gInput->setID("g-input");
    gColumn->addChild(m_impl->m_gInput);

    gColumn->updateLayout();
    auto gRect = calculateChildCoverage(gColumn);
    gColumn->setContentSize(gRect.size);

    // b

    auto bColumn = CCLayer::create();
    bColumn->setLayout(
        ColumnLayout::create()
            ->setGap(3.f)
            ->setAxisReverse(true)
            ->setAutoScale(false)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAutoGrowAxis(0)
    );
    bColumn->setID("b-column");
    rgbRow->addChild(bColumn);

    auto bText = CCLabelBMFont::create("B", "goldFont.fnt");
    bText->setScale(.55f);
    bText->setID("b-text");
    bColumn->addChild(bText);

    m_impl->m_bInput = TextInput::create(50.f, "B");
    m_impl->m_bInput->setScale(.7f);
    m_impl->m_bInput->setDelegate(this, TAG_B_INPUT);
    m_impl->m_bInput->setID("b-input");
    bColumn->addChild(m_impl->m_bInput);

    bColumn->updateLayout();
    auto bRect = calculateChildCoverage(bColumn);
    bColumn->setContentSize(bRect.size);

    // hex

    auto hexColumn = CCLayer::create();
    hexColumn->setLayout(
        ColumnLayout::create()
            ->setGap(3.f)
            ->setAxisReverse(true)
            ->setAutoScale(false)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAutoGrowAxis(0)
    );
    hexColumn->setID("hex-column");
    inputColumn->addChild(hexColumn);

    auto hexText = CCLabelBMFont::create("Hex", "goldFont.fnt");
    hexText->setScale(.55f);
    hexText->setID("hex-text");
    hexColumn->addChild(hexText);

    m_impl->m_hexInput = TextInput::create(165.f, "Hex");
    m_impl->m_hexInput->setScale(.7f);
    m_impl->m_hexInput->setDelegate(this, TAG_HEX_INPUT);
    m_impl->m_hexInput->setID("hex-input");
    hexColumn->addChild(m_impl->m_hexInput);

    hexColumn->updateLayout();
    rgbRow->updateLayout();
    inputColumn->updateLayout();
    colorMenu->updateLayout();
    pickerRow->updateLayout();

    if (isRGBA) {
        auto opacitySection = CCLayer::create();
        opacitySection->setLayout(
            RowLayout::create()
                ->setGap(10.f)
                ->setAutoScale(false)
                ->setAxisAlignment(AxisAlignment::Start)
                ->setCrossAxisLineAlignment(AxisAlignment::Start)
                ->setAutoGrowAxis(0)
        );
        opacitySection->setID("opacity-section");
        opacityColumn->addChild(opacitySection);

        auto sliderColumn = CCLayer::create();
        sliderColumn->setLayout(
            ColumnLayout::create()
                ->setGap(7.f)
                ->setAxisReverse(true)
                ->setAutoScale(false)
                ->setAxisAlignment(AxisAlignment::Start)
                ->setAutoGrowAxis(0)
        );
        sliderColumn->setID("slider-column");
        opacitySection->addChild(sliderColumn);


        auto opacityText = CCLabelBMFont::create("Opacity", "goldFont.fnt");
        opacityText->setScale(.55f);
        opacityText->setID("opacity-text");
        sliderColumn->addChild(opacityText);

        m_impl->m_opacitySlider =
            Slider::create(this, menu_selector(ColorPickPopup::onOpacitySlider), .75f);
        m_impl->m_opacitySlider->setValue(color.a / 255.f);
        m_impl->m_opacitySlider->setID("opacity-slider");

        auto sliderWrapper = CCNode::create();
        sliderWrapper->setContentSize(ccp(m_impl->m_opacitySlider->m_width, m_impl->m_opacitySlider->m_height) * .75f);
        sliderWrapper->setID("slider-wrapper");
        sliderWrapper->addChildAtPosition(m_impl->m_opacitySlider, Anchor::Center, ccp(0, 0));
        sliderColumn->addChild(sliderWrapper);

        m_impl->m_opacityInput = TextInput::create(60.f, "Opacity");
        m_impl->m_opacityInput->setScale(.7f);
        m_impl->m_opacityInput->setDelegate(this, TAG_OPACITY_INPUT);
        m_impl->m_opacityInput->setID("opacity-input");
        opacitySection->addChild(m_impl->m_opacityInput);

        sliderColumn->updateLayout();
        opacitySection->updateLayout();
    }

    opacityColumn->updateLayout();

    this->updateState();

    auto okBtnSpr = ButtonSprite::create("OK");
    okBtnSpr->setScale(.7f);

    auto okBtn =
        CCMenuItemSpriteExtra::create(okBtnSpr, this, menu_selector(ColorPickPopup::onClose));
    // okBtn->setPosition(.0f, -m_size.height / 2 + 20.f);
    m_buttonMenu->addChildAtPosition(okBtn, Anchor::Bottom, ccp(0, 20.f));

    return true;
}

void ColorPickPopup::updateState(CCNode* except) {
    #define IF_NOT_EXCEPT(inp, value)                  \
        if (inp->getInputNode() != except) {           \
            inp->setString(value, false);              \
        }

    IF_NOT_EXCEPT(m_impl->m_rInput, numToString<int>(m_impl->m_color.r));
    IF_NOT_EXCEPT(m_impl->m_gInput, numToString<int>(m_impl->m_color.g));
    IF_NOT_EXCEPT(m_impl->m_bInput, numToString<int>(m_impl->m_color.b));
    IF_NOT_EXCEPT(m_impl->m_hexInput, cc3bToHexString(to3B(m_impl->m_color)));
    if (m_impl->m_opacityInput) {
        IF_NOT_EXCEPT(m_impl->m_opacityInput, numToString(m_impl->m_color.a / 255.f, 2));
    }
    if (m_impl->m_opacitySlider) {
        m_impl->m_opacitySlider->setValue(m_impl->m_color.a / 255.f);
    }
    if (m_impl->m_picker != except) {
        m_impl->m_picker->setDelegate(nullptr);
        m_impl->m_picker->setColorValue(to3B(m_impl->m_color));
        m_impl->m_picker->setDelegate(this);
    }
    m_impl->m_resetBtn->setVisible(m_impl->m_originalColor != m_impl->m_color);
    m_impl->m_newColorSpr->setColor(to3B(m_impl->m_color));
}

void ColorPickPopup::onOpacitySlider(CCObject* sender) {
    m_impl->m_color.a = static_cast<GLubyte>(static_cast<SliderThumb*>(sender)->getValue() * 255.f);
    this->updateState();
}

void ColorPickPopup::onReset(CCObject*) {
    m_impl->m_color = m_impl->m_originalColor;
    this->updateState();
}

void ColorPickPopup::onClose(CCObject* sender) {
    if (m_impl->m_delegate) {
        m_impl->m_delegate->updateColor(m_impl->m_color);
    }
    Popup::onClose(sender);
}

void ColorPickPopup::textChanged(CCTextInputNode* input) {
    if (input->getString().size()) {
        switch (input->getTag()) {
            case TAG_HEX_INPUT:
                {
                    if (auto color = cc3bFromHexString(input->getString(), true)) {
                        m_impl->m_color.r = color.unwrap().r;
                        m_impl->m_color.g = color.unwrap().g;
                        m_impl->m_color.b = color.unwrap().b;
                    }
                }
                break;

            case TAG_OPACITY_INPUT: {
                auto res = numFromString<float>(input->getString().c_str());
                if (res) m_impl->m_color.a = std::clamp(static_cast<int>(res.unwrap() * 255.f), 0, 255);
                break;
            }

            case TAG_R_INPUT: {
                auto res = numFromString<uint32_t>(input->getString().c_str());
                if (res) m_impl->m_color.r = std::clamp(res.unwrap(), 0u, 255u);
                break;
            }
            case TAG_G_INPUT: {
                auto res = numFromString<uint32_t>(input->getString().c_str());
                if (res) m_impl->m_color.g = std::clamp(res.unwrap(), 0u, 255u);
                break;
            }
            case TAG_B_INPUT: {
                auto res = numFromString<uint32_t>(input->getString().c_str());
                if (res) m_impl->m_color.b = std::clamp(res.unwrap(), 0u, 255u);
                break;
            }

            default: break;
        }
    }
    this->updateState(input);
}

void ColorPickPopup::colorValueChanged(ccColor3B color) {
    m_impl->m_color.r = color.r;
    m_impl->m_color.g = color.g;
    m_impl->m_color.b = color.b;
    this->updateState(m_impl->m_picker);
}

void ColorPickPopup::setDelegate(ColorPickPopupDelegate* delegate) {
    m_impl->m_delegate = delegate;
}

void ColorPickPopup::setColorTarget(cocos2d::CCSprite* spr) {
    m_impl->m_picker->setColorTarget(spr);
}

ColorPickPopup* ColorPickPopup::create(ccColor4B const& color, bool isRGBA) {
    auto ret = new ColorPickPopup();
    if (ret->initAnchored(400.f, (isRGBA ? 290.f : 240.f), color, isRGBA)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
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
