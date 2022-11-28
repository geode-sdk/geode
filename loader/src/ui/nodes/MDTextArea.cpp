#include <Geode/binding/ProfilePage.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/MDTextArea.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/utils/string.hpp>
#include <md4c.h>

USE_GEODE_NAMESPACE();

static constexpr float g_fontScale = .5f;
static constexpr float g_paragraphPadding = 7.f;
static constexpr float g_indent = 7.f;
static constexpr float g_codeBlockIndent = 8.f;
static constexpr ccColor3B g_linkColor = cc3x(0x7ff4f4);

TextRenderer::Font g_mdFont = [](int style) -> TextRenderer::Label {
    if ((style & TextStyleBold) && (style & TextStyleItalic)) {
        return CCLabelBMFont::create("", "mdFontBI.fnt"_spr);
    }
    if ((style & TextStyleBold)) {
        return CCLabelBMFont::create("", "mdFontB.fnt"_spr);
    }
    if ((style & TextStyleItalic)) {
        return CCLabelBMFont::create("", "mdFontI.fnt"_spr);
    }
    return CCLabelBMFont::create("", "mdFont.fnt"_spr);
};

TextRenderer::Font g_mdMonoFont = [](int style) -> TextRenderer::Label {
    return CCLabelBMFont::create("", "mdFontMono.fnt"_spr);
};

class MDContentLayer : public CCContentLayer {
protected:
    CCMenu* m_content;

public:
    static MDContentLayer* create(CCMenu* content, float width, float height) {
        auto ret = new MDContentLayer();
        if (ret && ret->initWithColor({ 0, 255, 0, 0 }, width, height)) {
            ret->m_content = content;
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    void setPosition(CCPoint const& pos) override {
        // cringe CCContentLayer expect its children to
        // all be TableViewCells
        CCLayerColor::setPosition(pos);

        // so that's why based MDContentLayer expects itself
        // to have a CCMenu :-)
        if (m_content) {
            for (auto child : CCArrayExt<CCNode>(m_content->getChildren())) {
                auto y = this->getPositionY() + child->getPositionY();
                child->setVisible(
                    !((m_content->getContentSize().height < y) ||
                      (y < -child->getContentSize().height))
                );
            }
        }
    }
};

Result<ccColor3B> colorForIdentifier(std::string const& tag) {
    if (utils::string::contains(tag, ' ')) {
        auto hexStr = utils::string::split(utils::string::normalize(tag), " ").at(1);
        try {
            auto hex = std::stoi(hexStr, nullptr, 16);
            return Ok(cc3x(hex));
        }
        catch (...) {
            return Err("Invalid hex");
        }
    }
    else {
        auto colorText = tag.substr(1);
        if (!colorText.size()) {
            return Err("No color specified");
        }
        else if (colorText.size() > 1) {
            return Err("Color tag " + tag + " unexpectedly long, either do <cx> or <c hex>");
        }
        else {
            switch (colorText.front()) {
                case 'b': return Ok(cc3x(0x4a52e1)); break;
                case 'g': return Ok(cc3x(0x40e348)); break;
                case 'l': return Ok(cc3x(0x60abef)); break;
                case 'j': return Ok(cc3x(0x32c8ff)); break;
                case 'y': return Ok(cc3x(0xffff00)); break;
                case 'o': return Ok(cc3x(0xffa54b)); break;
                case 'r': return Ok(cc3x(0xff5a5a)); break;
                case 'p': return Ok(cc3x(0xff00ff)); break;
                default: return Err("Unknown color " + colorText);
            }
        }
    }
    return Err("Unknown error");
}

bool MDTextArea::init(std::string const& str, CCSize const& size) {
    if (!CCLayer::init()) return false;

    m_text = str;
    m_size = size;
    this->setContentSize(size);
    m_renderer = TextRenderer::create();
    CC_SAFE_RETAIN(m_renderer);

    m_bgSprite = CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    m_bgSprite->setScale(.5f);
    m_bgSprite->setColor({ 0, 0, 0 });
    m_bgSprite->setOpacity(75);
    m_bgSprite->setContentSize(size * 2 + CCSize { 25.f, 25.f });
    m_bgSprite->setPosition(size / 2);
    this->addChild(m_bgSprite);

    m_scrollLayer = ScrollLayer::create({ 0, 0, m_size.width, m_size.height }, true);

    m_content = CCMenu::create();
    m_content->setZOrder(2);
    m_scrollLayer->m_contentLayer->addChild(m_content);

    CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
    m_scrollLayer->registerWithTouchDispatcher();

    this->addChild(m_scrollLayer);

    this->updateLabel();

    return true;
}

MDTextArea::~MDTextArea() {
    CC_SAFE_RELEASE(m_renderer);
}

class BreakLine : public CCNode {
protected:
    void draw() override {
        // some nodes sometimes set the blend func to
        // something else without resetting it back
        ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ccDrawSolidRect({ 0, 0 }, this->getContentSize(), { 1.f, 1.f, 1.f, .2f });
        CCNode::draw();
    }

public:
    static BreakLine* create(float width) {
        auto ret = new BreakLine;
        if (ret && ret->init()) {
            ret->autorelease();
            ret->setContentSize({ width, 1.f });
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

void MDTextArea::onLink(CCObject* pSender) {
    auto href = as<CCString*>(as<CCNode*>(pSender)->getUserObject());
    auto layer = FLAlertLayer::create(
        this, "Hold Up!",
        "Links are spooky! Are you sure you want to go to <cy>" + std::string(href->getCString()) +
            "</c>?",
        "Cancel", "Yes", 360.f
    );
    layer->setUserObject(href);
    layer->show();
}

void MDTextArea::onGDProfile(CCObject* pSender) {
    auto href = as<CCString*>(as<CCNode*>(pSender)->getUserObject());
    auto profile = std::string(href->getCString());
    profile = profile.substr(profile.find(":") + 1);
    try {
        ProfilePage::create(std::stoi(profile), false)->show();
    }
    catch (...) {
        FLAlertLayer::create(
            "Error",
            "Invalid profile ID: <cr>" + profile +
                "</c>. This is "
                "probably the modder's fault, report the bug to them.",
            "OK"
        )
            ->show();
    }
}

void MDTextArea::FLAlert_Clicked(FLAlertLayer* layer, bool btn) {
    if (btn) {
        web::openLinkInBrowser(as<CCString*>(layer->getUserObject())->getCString());
    }
}

struct MDParser {
    static std::string s_lastLink;
    static std::string s_lastImage;
    static bool s_isOrderedList;
    static bool s_isCodeBlock;
    static float s_codeStart;
    static size_t s_orderedListNum;
    static std::vector<TextRenderer::Label> s_codeSpans;

    static int parseText(MD_TEXTTYPE type, MD_CHAR const* rawText, MD_SIZE size, void* mdtextarea) {
        auto textarea = static_cast<MDTextArea*>(mdtextarea);
        auto renderer = textarea->m_renderer;
        auto text = std::string(rawText, size);
        switch (type) {
            case MD_TEXTTYPE::MD_TEXT_CODE:
                {
                    auto rendered = renderer->renderString(text);
                    if (!s_isCodeBlock) {
                        // code span BGs need to be rendered after all
                        // rendering is done since the position of the
                        // rendered labels may change after alignments
                        // are adjusted
                        ranges::push(s_codeSpans, rendered);
                    }
                }
                break;

            case MD_TEXTTYPE::MD_TEXT_BR:
                {
                    renderer->breakLine();
                }
                break;

            case MD_TEXTTYPE::MD_TEXT_SOFTBR:
                {
                    renderer->breakLine();
                }
                break;

            case MD_TEXTTYPE::MD_TEXT_NORMAL:
                {
                    if (s_lastLink.size()) {
                        renderer->pushColor(g_linkColor);
                        renderer->pushDecoFlags(TextDecorationUnderline);
                        auto rendered = renderer->renderStringInteractive(
                            text, textarea,
                            utils::string::startsWith(s_lastLink, "user:")
                                ? menu_selector(MDTextArea::onGDProfile)
                                : menu_selector(MDTextArea::onLink)
                        );
                        for (auto const& label : rendered) {
                            label.m_node->setUserObject(CCString::create(s_lastLink));
                        }
                        renderer->popDecoFlags();
                        renderer->popColor();
                    }
                    else if (s_lastImage.size()) {
                        bool isFrame = false;
                        if (utils::string::startsWith(s_lastImage, "frame:")) {
                            s_lastImage = s_lastImage.substr(s_lastImage.find(":") + 1);
                            isFrame = true;
                        }
                        CCSprite* spr = nullptr;
                        if (isFrame) {
                            spr = CCSprite::createWithSpriteFrameName(s_lastImage.c_str());
                        }
                        else {
                            spr = CCSprite::create(s_lastImage.c_str());
                        }
                        if (spr) {
                            renderer->renderNode(spr);
                        }
                        else {
                            renderer->renderString(text);
                        }
                        s_lastImage = "";
                    }
                    else {
                        renderer->renderString(text);
                    }
                }
                break;

            case MD_TEXTTYPE::MD_TEXT_HTML:
                {
                    if (text.size() > 2) {
                        auto tag = utils::string::trim(text.substr(1, text.size() - 2));
                        auto isClosing = tag.front() == '/';
                        if (isClosing) tag = tag.substr(1);
                        if (tag.front() != 'c') {
                            log::warn("Unknown tag {}", text);
                            renderer->renderString(text);
                        }
                        else {
                            if (isClosing) {
                                renderer->popColor();
                            }
                            else {
                                auto color = colorForIdentifier(tag);
                                if (color) {
                                    renderer->pushColor(color.unwrap());
                                }
                                else {
                                    log::warn("Error parsing color: {}", color.unwrapErr());
                                }
                            }
                        }
                    }
                    else {
                        log::warn("Too short tag {}", text);
                        renderer->renderString(text);
                    }
                }
                break;

            default:
                {
                    log::warn("Unhandled text type {}", type);
                }
                break;
        }
        return 0;
    }

    static int enterBlock(MD_BLOCKTYPE type, void* detail, void* mdtextarea) {
        auto textarea = static_cast<MDTextArea*>(mdtextarea);
        auto renderer = textarea->m_renderer;
        switch (type) {
            case MD_BLOCKTYPE::MD_BLOCK_DOC:
                {
                }
                break;

            case MD_BLOCKTYPE::MD_BLOCK_H:
                {
                    auto hdetail = static_cast<MD_BLOCK_H_DETAIL*>(detail);
                    renderer->pushStyleFlags(TextStyleBold);
                    switch (hdetail->level) {
                        case 1: renderer->pushScale(g_fontScale * 2.f); break;
                        case 2: renderer->pushScale(g_fontScale * 1.5f); break;
                        case 3: renderer->pushScale(g_fontScale * 1.17f); break;
                        case 4: renderer->pushScale(g_fontScale); break;
                        case 5: renderer->pushScale(g_fontScale * .83f); break;
                        default:
                        case 6: renderer->pushScale(g_fontScale * .67f); break;
                    }
                    // switch (hdetail->level) {
                    //     case 3: renderer->pushCaps(TextCapitalization::AllUpper); break;
                    // }
                }
                break;

            case MD_BLOCKTYPE::MD_BLOCK_P:
                {
                }
                break;

            case MD_BLOCKTYPE::MD_BLOCK_UL:
            case MD_BLOCKTYPE::MD_BLOCK_OL:
                {
                    renderer->pushIndent(g_indent);
                    s_isOrderedList = type == MD_BLOCKTYPE::MD_BLOCK_OL;
                    s_orderedListNum = 0;
                }
                break;

            case MD_BLOCKTYPE::MD_BLOCK_HR:
                {
                    renderer->breakLine(g_paragraphPadding / 2);
                    renderer->renderNode(BreakLine::create(textarea->m_size.width));
                    renderer->breakLine(g_paragraphPadding);
                }
                break;

            case MD_BLOCKTYPE::MD_BLOCK_LI:
                {
                    renderer->pushOpacity(renderer->getCurrentOpacity() / 2);
                    auto lidetail = static_cast<MD_BLOCK_LI_DETAIL*>(detail);
                    if (s_isOrderedList) {
                        s_orderedListNum++;
                        renderer->renderString(std::to_string(s_orderedListNum) + ". ");
                    }
                    else {
                        renderer->renderString("• ");
                    }
                    renderer->popOpacity();
                }
                break;

            case MD_BLOCKTYPE::MD_BLOCK_CODE:
                {
                    s_isCodeBlock = true;
                    s_codeStart = renderer->getCursorPos().y;
                    renderer->pushFont(g_mdMonoFont);
                    renderer->pushIndent(g_codeBlockIndent);
                    renderer->pushWrapOffset(g_codeBlockIndent);
                }
                break;

            default:
                {
                    log::warn("Unhandled block enter type {}", type);
                }
                break;
        }
        return 0;
    }

    static int leaveBlock(MD_BLOCKTYPE type, void* detail, void* mdtextarea) {
        auto textarea = static_cast<MDTextArea*>(mdtextarea);
        auto renderer = textarea->m_renderer;
        switch (type) {
            case MD_BLOCKTYPE::MD_BLOCK_DOC:
                {
                }
                break;

            case MD_BLOCKTYPE::MD_BLOCK_H:
                {
                    auto hdetail = static_cast<MD_BLOCK_H_DETAIL*>(detail);
                    renderer->breakLine();
                    if (hdetail->level == 1) {
                        renderer->breakLine(g_paragraphPadding / 2);
                        renderer->renderNode(BreakLine::create(textarea->m_size.width));
                    }
                    renderer->breakLine(g_paragraphPadding);
                    renderer->popScale();
                    renderer->popStyleFlags();
                    // switch (hdetail->level) {
                    //     case 3: renderer->popCaps(); break;
                    // }
                }
                break;

            case MD_BLOCKTYPE::MD_BLOCK_P:
                {
                    renderer->breakLine();
                    renderer->breakLine(g_paragraphPadding);
                }
                break;

            case MD_BLOCKTYPE::MD_BLOCK_OL:
            case MD_BLOCKTYPE::MD_BLOCK_UL:
                {
                    renderer->popIndent();
                }
                break;

            case MD_BLOCKTYPE::MD_BLOCK_CODE:
                {
                    auto codeEnd = renderer->getCursorPos().y;

                    auto pad = g_codeBlockIndent / 1.5f;

                    CCSize size { textarea->m_size.width - renderer->getCurrentIndent() -
                                      renderer->getCurrentWrapOffset() + pad * 2,
                                  s_codeStart - codeEnd + pad * 2 };

                    auto bg =
                        CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
                    bg->setScale(.25f);
                    bg->setColor({ 0, 0, 0 });
                    bg->setOpacity(75);
                    bg->setContentSize(size * 4);
                    bg->setPosition(
                        size.width / 2 + renderer->getCurrentIndent() - pad,
                        // mmm i love magic numbers
                        // the -2.f is to offset the the box
                        // to fit the Ubuntu font very neatly.
                        // idk if it works the same for other
                        // fonts
                        s_codeStart - 2.f + pad - size.height / 2
                    );
                    bg->setAnchorPoint({ .5f, .5f });
                    bg->setZOrder(-1);
                    textarea->m_content->addChild(bg);

                    renderer->popWrapOffset();
                    renderer->popIndent();
                    renderer->popFont();

                    renderer->breakLine();
                }
                break;

            case MD_BLOCKTYPE::MD_BLOCK_LI:
                {
                }
                break;

            case MD_BLOCKTYPE::MD_BLOCK_HR:
                {
                }
                break;

            default:
                {
                    log::warn("Unhandled block leave type {}", type);
                }
                break;
        }
        return 0;
    }

    static int enterSpan(MD_SPANTYPE type, void* detail, void* mdtextarea) {
        auto renderer = static_cast<MDTextArea*>(mdtextarea)->m_renderer;
        switch (type) {
            case MD_SPANTYPE::MD_SPAN_STRONG:
                {
                    renderer->pushStyleFlags(TextStyleBold);
                }
                break;

            case MD_SPANTYPE::MD_SPAN_EM:
                {
                    renderer->pushStyleFlags(TextStyleItalic);
                }
                break;

            case MD_SPANTYPE::MD_SPAN_DEL:
                {
                    renderer->pushDecoFlags(TextDecorationStrikethrough);
                }
                break;

            case MD_SPANTYPE::MD_SPAN_U:
                {
                    renderer->pushDecoFlags(TextDecorationUnderline);
                }
                break;

            case MD_SPANTYPE::MD_SPAN_IMG:
                {
                    auto adetail = static_cast<MD_SPAN_IMG_DETAIL*>(detail);
                    s_lastImage = std::string(adetail->src.text, adetail->src.size);
                }
                break;

            case MD_SPANTYPE::MD_SPAN_A:
                {
                    auto adetail = static_cast<MD_SPAN_A_DETAIL*>(detail);
                    s_lastLink = std::string(adetail->href.text, adetail->href.size);
                }
                break;

            case MD_SPANTYPE::MD_SPAN_CODE:
                {
                    s_isCodeBlock = false;
                    renderer->pushFont(g_mdMonoFont);
                }
                break;

            default:
                {
                    log::warn("Unhandled span enter type {}", type);
                }
                break;
        }
        return 0;
    }

    static int leaveSpan(MD_SPANTYPE type, void* detail, void* mdtextarea) {
        auto renderer = static_cast<MDTextArea*>(mdtextarea)->m_renderer;
        switch (type) {
            case MD_SPANTYPE::MD_SPAN_STRONG:
                {
                    renderer->popStyleFlags();
                }
                break;

            case MD_SPANTYPE::MD_SPAN_EM:
                {
                    renderer->popStyleFlags();
                }
                break;

            case MD_SPANTYPE::MD_SPAN_DEL:
                {
                    renderer->popDecoFlags();
                }
                break;

            case MD_SPANTYPE::MD_SPAN_U:
                {
                    renderer->popDecoFlags();
                }
                break;

            case MD_SPANTYPE::MD_SPAN_A:
                {
                    s_lastLink = "";
                }
                break;

            case MD_SPANTYPE::MD_SPAN_IMG:
                {
                    s_lastImage = "";
                }
                break;

            case MD_SPANTYPE::MD_SPAN_CODE:
                {
                    renderer->popFont();
                }
                break;

            default:
                {
                    log::warn("Unhandled span leave type {}", type);
                }
                break;
        }
        return 0;
    }
};

std::string MDParser::s_lastLink = "";
std::string MDParser::s_lastImage = "";
bool MDParser::s_isOrderedList = false;
size_t MDParser::s_orderedListNum = 0;
bool MDParser::s_isCodeBlock = false;
float MDParser::s_codeStart = 0;
decltype(MDParser::s_codeSpans) MDParser::s_codeSpans = {};

void MDTextArea::updateLabel() {
    m_renderer->begin(m_content, CCPointZero, m_size);

    m_renderer->pushFont(g_mdFont);
    m_renderer->pushScale(.5f);
    m_renderer->pushVerticalAlign(TextAlignment::End);
    m_renderer->pushHorizontalAlign(TextAlignment::Begin);

    MD_PARSER parser;

    parser.abi_version = 0;
    parser.flags = MD_FLAG_UNDERLINE | MD_FLAG_STRIKETHROUGH | MD_FLAG_PERMISSIVEURLAUTOLINKS |
        MD_FLAG_PERMISSIVEWWWAUTOLINKS;

    parser.text = &MDParser::parseText;
    parser.enter_block = &MDParser::enterBlock;
    parser.leave_block = &MDParser::leaveBlock;
    parser.enter_span = &MDParser::enterSpan;
    parser.leave_span = &MDParser::leaveSpan;
    parser.debug_log = nullptr;
    parser.syntax = nullptr;

    MDParser::s_codeSpans = {};

    if (md_parse(m_text.c_str(), m_text.size(), &parser, this)) {
        m_renderer->renderString("Error parsing Markdown");
    }

    for (auto& render : MDParser::s_codeSpans) {
        auto bg = CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
        bg->setScale(.125f);
        bg->setColor({ 0, 0, 0 });
        bg->setOpacity(75);
        bg->setContentSize(render.m_node->getScaledContentSize() * 8 + CCSize { 20.f, .0f });
        bg->setPosition(
            render.m_node->getPositionX() - 2.5f * (.5f - render.m_node->getAnchorPoint().x),
            render.m_node->getPositionY() - .5f
        );
        bg->setAnchorPoint(render.m_node->getAnchorPoint());
        bg->setZOrder(-1);
        m_content->addChild(bg);
        // i know what you're thinking.
        // my brother in christ, what the hell is this?
        // where did this magical + 1.5f come from?
        // the reason is that if you remove them, code
        // spans are slightly offset and it triggers my
        // OCD.
        render.m_node->setPositionY(render.m_node->getPositionY() + 1.5f);
    }

    m_renderer->end();

    m_scrollLayer->m_contentLayer->setContentSize(m_content->getContentSize());
    m_scrollLayer->moveToTop();
}

CCScrollLayerExt* MDTextArea::getScrollLayer() const {
    return m_scrollLayer;
}

void MDTextArea::setString(char const* text) {
    this->m_text = text;
    this->updateLabel();
}

char const* MDTextArea::getString() {
    return this->m_text.c_str();
}

MDTextArea* MDTextArea::create(std::string const& str, CCSize const& size) {
    auto ret = new MDTextArea;
    if (ret && ret->init(str, size)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
