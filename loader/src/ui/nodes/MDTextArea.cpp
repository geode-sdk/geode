#include <Geode/binding/GJSearchObject.hpp>
#include <Geode/binding/LevelBrowserLayer.hpp>
#include <Geode/binding/ProfilePage.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/BreakLine.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/MDTextArea.hpp>
#include <numeric>
#include <simdutf/implementation.h>
#include <ui/mods/list/ModItem.hpp>

using namespace geode::prelude;

// Config

static BitmapFont* loadFont(ZStringView fnt) {
    auto ret = BitmapFont::load(fnt);
    if (!ret) return BitmapFont::load("chatFont.fnt");
    return ret;
}

struct MDConfig::Impl {
    BitmapFont* fontRegular = loadFont("mdFont.fnt"_spr);
    BitmapFont* fontBold = loadFont("mdFontB.fnt"_spr);
    BitmapFont* fontItalic = loadFont("mdFontI.fnt"_spr);
    BitmapFont* fontBoldItalic = loadFont("mdFontBI.fnt"_spr);
    BitmapFont* fontMono = loadFont("mdFontMono.fnt"_spr);

    std::string listBulletMarker = "\u2022 ";
    std::string listOrderedSeparator = ". ";

    std::array<float, 6> headingScale = { 2.0f, 1.5f, 1.17f, 1.0f, 0.83f, 0.67f };

    float fontScale = 0.5f;
    float paragraphSpacing = 7.f;
    float listIndent = 7.f;
    float codeBlockIndent = 8.f;
    float codeBlockPadding = 5.33f;
    float codeBlockBgScale = 0.25f;
    float codeSpanBgScale = 0.25f;
    float codeSpanBgExtraWidth = 2.5f;
    float codeSpanBgOffsetY = 0.f;
    float codeSpanTextOffsetY = 0.f;
    float hrPaddingBefore = 3.5f;
    float hrPaddingAfter = 0.f;
    float imageDefaultScale = 1.0f;
    float imageMinScale = 0.1f;
    float imageMaxDim = 999.f;
    float blockQuoteIndent = 10.f;
    float blockQuoteRuleWidth = 2.f;
    float tableCellPadding = 4.f;
    float tableBorderWidth = 1.f;

    ccColor3B linkColor{118, 163, 214};
    ccColor3B linkActiveColor{25, 60, 99};
    ccColor3B codeBlockBgColor{0, 0, 0};
    ccColor3B codeSpanBgColor{0, 0, 0};
    ccColor3B blockQuoteRuleColor{200, 200, 200};
    ccColor3B tableBorderColor{100, 100, 100};
    ccColor3B tableHeaderBgColor{40, 40, 40};

    GLubyte listMarkerOpacity = 127;
    GLubyte codeBlockBgOpacity = 75;
    GLubyte codeSpanBgOpacity = 75;
    GLubyte tableHeaderBgOpacity = 128;

    BitmapFont* resolveFont(bool bold, bool italic, bool mono) const noexcept {
        if (mono) {
            return fontMono ? fontMono : fontRegular;
        }

        if (bold && italic) {
            if (fontBoldItalic) return fontBoldItalic;
            if (fontBold) return fontBold;
            if (fontItalic) return fontItalic;
            return fontRegular;
        }

        if (bold) {
            return fontBold ? fontBold : fontRegular;
        }

        if (italic) {
            return fontItalic ? fontItalic : fontRegular;
        }

        return fontRegular;
    }
};

MDConfig::MDConfig() : m_impl(std::make_shared<Impl>()) {}
MDConfig::~MDConfig() = default;
MDConfig::MDConfig(MDConfig const&) = default;
MDConfig::MDConfig(MDConfig&&) noexcept = default;
MDConfig& MDConfig::operator=(MDConfig const&) = default;
MDConfig& MDConfig::operator=(MDConfig&&) noexcept = default;

MDConfig MDConfig::clone() const {
    MDConfig out;
    out.m_impl = std::make_shared<Impl>(*m_impl);
    return out;
}

void MDConfig::setFontRegular(BitmapFont* font) { m_impl->fontRegular = font; }
BitmapFont* MDConfig::getFontRegular() const noexcept { return m_impl->fontRegular; }

void MDConfig::setFontBold(BitmapFont* font) { m_impl->fontBold = font; }
BitmapFont* MDConfig::getFontBold() const noexcept { return m_impl->fontBold; }

void MDConfig::setFontItalic(BitmapFont* font) { m_impl->fontItalic = font; }
BitmapFont* MDConfig::getFontItalic() const noexcept { return m_impl->fontItalic; }

void MDConfig::setFontBoldItalic(BitmapFont* font) { m_impl->fontBoldItalic = font; }
BitmapFont* MDConfig::getFontBoldItalic() const noexcept { return m_impl->fontBoldItalic; }

void MDConfig::setFontMono(BitmapFont* font) { m_impl->fontMono = font; }
BitmapFont* MDConfig::getFontMono() const noexcept { return m_impl->fontMono; }

BitmapFont* MDConfig::resolveFont(bool bold, bool italic, bool mono) const noexcept {
    return m_impl->resolveFont(bold, italic, mono);
}

void MDConfig::setFontScale(float scale) { m_impl->fontScale = scale; }
float MDConfig::getFontScale() const noexcept { return m_impl->fontScale; }

void MDConfig::setHeadingScale(std::array<float, 6> const& scales) { m_impl->headingScale = scales; }
std::array<float, 6> const& MDConfig::getHeadingScale() const noexcept { return m_impl->headingScale; }

void MDConfig::setHeadingScale(uint8_t level, float scale) {
    if (level >= 6) return;
    m_impl->headingScale[level] = scale;
}

float MDConfig::getHeadingScale(uint8_t level) const noexcept {
    if (level >= 6) return 1.f;
    return m_impl->headingScale[level];
}

void MDConfig::setLinkColor(ccColor3B color) { m_impl->linkColor = color; }
ccColor3B MDConfig::getLinkColor() const noexcept { return m_impl->linkColor; }

void MDConfig::setLinkActiveColor(ccColor3B color) { m_impl->linkActiveColor = color; }
ccColor3B MDConfig::getLinkActiveColor() const noexcept { return m_impl->linkActiveColor; }

void MDConfig::setParagraphSpacing(float spacing) { m_impl->paragraphSpacing = spacing; }
float MDConfig::getParagraphSpacing() const noexcept { return m_impl->paragraphSpacing; }

void MDConfig::setListIndent(float indent) { m_impl->listIndent = indent; }
float MDConfig::getListIndent() const noexcept { return m_impl->listIndent; }

void MDConfig::setListBulletMarker(std::string marker) { m_impl->listBulletMarker = std::move(marker); }
std::string const& MDConfig::getListBulletMarker() const noexcept { return m_impl->listBulletMarker; }

void MDConfig::setListOrderedSeparator(std::string separator) { m_impl->listOrderedSeparator = std::move(separator); }
std::string const& MDConfig::getListOrderedSeparator() const noexcept { return m_impl->listOrderedSeparator; }

void MDConfig::setListMarkerOpacity(GLubyte opacity) { m_impl->listMarkerOpacity = opacity; }
GLubyte MDConfig::getListMarkerOpacity() const noexcept { return m_impl->listMarkerOpacity; }

void MDConfig::setCodeBlockIndent(float indent) { m_impl->codeBlockIndent = indent; }
float MDConfig::getCodeBlockIndent() const noexcept { return m_impl->codeBlockIndent; }

void MDConfig::setCodeBlockPadding(float padding) { m_impl->codeBlockPadding = padding; }
float MDConfig::getCodeBlockPadding() const noexcept { return m_impl->codeBlockPadding; }

void MDConfig::setCodeBlockBgScale(float scale) { m_impl->codeBlockBgScale = scale; }
float MDConfig::getCodeBlockBgScale() const noexcept { return m_impl->codeBlockBgScale; }

void MDConfig::setCodeBlockBgOpacity(GLubyte opacity) { m_impl->codeBlockBgOpacity = opacity; }
GLubyte MDConfig::getCodeBlockBgOpacity() const noexcept { return m_impl->codeBlockBgOpacity; }

void MDConfig::setCodeBlockBgColor(ccColor3B color) { m_impl->codeBlockBgColor = color; }
ccColor3B MDConfig::getCodeBlockBgColor() const noexcept { return m_impl->codeBlockBgColor; }

void MDConfig::setCodeSpanBgScale(float scale) { m_impl->codeSpanBgScale = scale; }
float MDConfig::getCodeSpanBgScale() const noexcept { return m_impl->codeSpanBgScale; }

void MDConfig::setCodeSpanBgExtraWidth(float width) { m_impl->codeSpanBgExtraWidth = width; }
float MDConfig::getCodeSpanBgExtraWidth() const noexcept { return m_impl->codeSpanBgExtraWidth; }

void MDConfig::setCodeSpanBgOffsetY(float offset) { m_impl->codeSpanBgOffsetY = offset; }
float MDConfig::getCodeSpanBgOffsetY() const noexcept { return m_impl->codeSpanBgOffsetY; }

void MDConfig::setCodeSpanTextOffsetY(float offset) { m_impl->codeSpanTextOffsetY = offset; }
float MDConfig::getCodeSpanTextOffsetY() const noexcept { return m_impl->codeSpanTextOffsetY; }

void MDConfig::setCodeSpanBgOpacity(GLubyte opacity) { m_impl->codeSpanBgOpacity = opacity; }
GLubyte MDConfig::getCodeSpanBgOpacity() const noexcept { return m_impl->codeSpanBgOpacity; }

void MDConfig::setCodeSpanBgColor(ccColor3B color) { m_impl->codeSpanBgColor = color; }
ccColor3B MDConfig::getCodeSpanBgColor() const noexcept { return m_impl->codeSpanBgColor; }

void MDConfig::setHrPaddingBefore(float padding) { m_impl->hrPaddingBefore = padding; }
float MDConfig::getHrPaddingBefore() const noexcept { return m_impl->hrPaddingBefore; }

void MDConfig::setHrPaddingAfter(float padding) { m_impl->hrPaddingAfter = padding; }
float MDConfig::getHrPaddingAfter() const noexcept { return m_impl->hrPaddingAfter; }

void MDConfig::setImageDefaultScale(float value) { m_impl->imageDefaultScale = value; }
float MDConfig::getImageDefaultScale() const noexcept { return m_impl->imageDefaultScale; }

void MDConfig::setImageMinScale(float value) { m_impl->imageMinScale = value; }
float MDConfig::getImageMinScale() const noexcept { return m_impl->imageMinScale; }

void MDConfig::setImageMaxDim(float value) { m_impl->imageMaxDim = value; }
float MDConfig::getImageMaxDim() const noexcept { return m_impl->imageMaxDim; }

void MDConfig::setBlockQuoteIndent(float indent) { m_impl->blockQuoteIndent = indent; }
float MDConfig::getBlockQuoteIndent() const noexcept { return m_impl->blockQuoteIndent; }

void MDConfig::setBlockQuoteRuleWidth(float width) { m_impl->blockQuoteRuleWidth = width; }
float MDConfig::getBlockQuoteRuleWidth() const noexcept { return m_impl->blockQuoteRuleWidth; }

void MDConfig::setBlockQuoteRuleColor(ccColor3B color) { m_impl->blockQuoteRuleColor = color; }
ccColor3B MDConfig::getBlockQuoteRuleColor() const noexcept { return m_impl->blockQuoteRuleColor; }

void MDConfig::setTableCellPadding(float padding) { m_impl->tableCellPadding = padding; }
float MDConfig::getTableCellPadding() const noexcept { return m_impl->tableCellPadding; }

void MDConfig::setTableBorderWidth(float width) { m_impl->tableBorderWidth = width; }
float MDConfig::getTableBorderWidth() const noexcept { return m_impl->tableBorderWidth; }

void MDConfig::setTableHeaderBgOpacity(GLubyte opacity) { m_impl->tableHeaderBgOpacity = opacity; }
GLubyte MDConfig::getTableHeaderBgOpacity() const noexcept { return m_impl->tableHeaderBgOpacity; }

void MDConfig::setTableBorderColor(ccColor3B color) { m_impl->tableBorderColor = color; }
ccColor3B MDConfig::getTableBorderColor() const noexcept { return m_impl->tableBorderColor; }

void MDConfig::setTableHeaderBgColor(ccColor3B color) { m_impl->tableHeaderBgColor = color; }
ccColor3B MDConfig::getTableHeaderBgColor() const noexcept { return m_impl->tableHeaderBgColor; }

// Rendering

struct InlineContext {
    bool bold = false;
    bool italic = false;
    bool mono = false;
    int decoration = TextDecorationNone;
    std::optional<ccColor3B> color;
};

struct LinkRegion {
    Label* label;
    uint32_t codepointBegin;
    uint32_t codepointEnd;
    std::string href;
    md::LinkKind linkKind;
};

static float renderBlocksInto(
    std::vector<md::Block> const& blocks,
    MDConfig const& config,
    float x, float w, float y, CCNode* parent,
    std::vector<LinkRegion>* links, float gap
);

struct CodeSpanRegion { uint32_t begin, end; };

struct RunGroup {
    std::vector<Label::Run> runs;
    std::vector<CodeSpanRegion> codeSpans;
    std::vector<LinkRegion>* links = nullptr;
    uint32_t cp = 0;
    float labelScale = 1.0f;
    float availWidth = 0.f;
};

static void pushRun(RunGroup& r, std::string text, BitmapFont* font, InlineContext const& ctx) {
    r.cp += simdutf::utf32_length_from_utf8(text);
    r.runs.push_back({std::move(text), font, ctx.color, ctx.decoration});
}

static CCSprite* loadImageSprite(std::string const& src, bool frame) {
    // TODO ~~5.x~~ 6.x: remote images
    if (src.starts_with("http://") || src.starts_with("https://")) return nullptr;

    auto spr = frame
        ? CCSprite::createWithSpriteFrameName(src.c_str())
        : CCSprite::create(src.c_str());

    if (!spr || spr->isUsingFallback()) return nullptr;
    return spr;
}

static void applyImageSize(CCSprite* spr, md::InlineImage const& img, MDConfig const& cfg) {
    spr->setScale(img.scale.value_or(cfg.getImageDefaultScale()));

    float mx = cfg.getImageMaxDim();
    float mn = cfg.getImageMinScale();

    if (img.width && !img.height) {
        limitNodeWidth(spr, *img.width, mx, mn);
    } else if (img.height && !img.width) {
        limitNodeHeight(spr, *img.height, mx, mn);
    } else if (img.width && img.height) {
        limitNodeSize(spr, {*img.width, *img.height}, mx, mn);
    }
}

static void collectRuns(
    std::span<md::Inline const> inlines,
    MDConfig const& config,
    InlineContext const& ctx,
    RunGroup& r
) {
    for (auto const& node : inlines) {
        std::visit([&](auto const& data) {
            using T = std::decay_t<decltype(data)>;

            if constexpr (std::is_same_v<T, md::InlineText>) {
                pushRun(r, data.text, config.resolveFont(ctx.bold, ctx.italic, ctx.mono), ctx);
            } else if constexpr (std::is_same_v<T, md::InlineCode>) {
                if (data.code.empty()) return;
                uint32_t begin = r.cp;
                pushRun(r, data.code, config.resolveFont(false, false, true), ctx);
                r.codeSpans.push_back({begin, r.cp});
            } else if constexpr (std::is_same_v<T, md::InlineSoftBreak>) {
                pushRun(r, " ", config.resolveFont(ctx.bold, ctx.italic, ctx.mono), ctx);
            } else if constexpr (std::is_same_v<T, md::InlineHardBreak>) {
                pushRun(r, "\n", config.resolveFont(ctx.bold, ctx.italic, ctx.mono), ctx);
            } else if constexpr (std::is_same_v<T, md::InlineEmphasis>) {
                auto n = ctx; n.italic = true;
                collectRuns(data.children, config, n, r);
            } else if constexpr (std::is_same_v<T, md::InlineStrong>) {
                auto n = ctx; n.bold = true;
                collectRuns(data.children, config, n, r);
            } else if constexpr (std::is_same_v<T, md::InlineStrikethrough>) {
                auto n = ctx; n.decoration |= TextDecorationStrikethrough;
                collectRuns(data.children, config, n, r);
            } else if constexpr (std::is_same_v<T, md::InlineUnderline>) {
                auto n = ctx; n.decoration |= TextDecorationUnderline;
                collectRuns(data.children, config, n, r);
            } else if constexpr (std::is_same_v<T, md::InlineColorTag>) {
                auto n = ctx; n.color = data.color;
                collectRuns(data.children, config, n, r);
            } else if constexpr (std::is_same_v<T, md::InlineLink>) {
                if (data.isAutoLink && data.linkKind == md::LinkKind::GeodeMod && r.availWidth > 0.f) {
                    auto* item = AnyModItem::create(data.href.substr(data.href.find(':') + 1));
                    if (item) {
                        item->updateDisplay(r.availWidth - 1.f, ModListDisplay::SmallList);
                        item->setScale(1.f / r.labelScale);
                        item->setAnchorPoint({0.f, 1.f});

                        Label::Run run;
                        run.node = [ref = Ref<CCNode>(item)]() -> CCNode* { return ref; };

                        r.runs.push_back(std::move(run));
                        r.cp += 1;
                        return;
                    }
                }

                auto n = ctx;
                n.color = config.getLinkColor();
                n.decoration |= TextDecorationUnderline;
                uint32_t begin = r.cp;
                collectRuns(data.children, config, n, r);
                if (r.links && r.cp > begin) {
                    r.links->push_back({
                        nullptr,
                        begin,
                        r.cp,
                        data.href,
                        data.linkKind
                    });
                }
            } else if constexpr (std::is_same_v<T, md::InlineImage>) {
                auto* spr = loadImageSprite(data.src, data.isSpriteFrame);
                if (!spr) {
                    if (!data.alt.empty()) {
                        pushRun(r, data.alt, config.resolveFont(ctx.bold, ctx.italic, ctx.mono), ctx);
                    }
                    return;
                }

                applyImageSize(spr, data, config);
                float finalScale = spr->getScale() / r.labelScale;
                spr->setScale(finalScale);
                spr->setAnchorPoint({0.f, 1.f});

                Label::Run run;
                run.node = [spr = Ref(spr)]() -> CCNode* { return spr; };

                r.cp += 1;
                r.runs.push_back(std::move(run));
            }
        }, node.data);
    }
}

static void addCodeSpanBgs(
    Label* label,
    std::vector<CodeSpanRegion> const& spans,
    MDConfig const& cfg, CCNode* parent
) {
    if (spans.empty()) return;
    float s = label->getScale();
    float H = label->getContentHeight();
    float lx = label->getPositionX();
    float ly = label->getPositionY();
    float padX = cfg.getCodeSpanBgExtraWidth() * 0.5f;
    float dy = cfg.getCodeSpanBgOffsetY() - cfg.getCodeSpanTextOffsetY();

    for (auto const& span : spans) {
        for (auto const& r : label->getRangeRects(span.begin, span.end)) {
            auto* bg = NineSlice::create("square02b_001.png");
            bg->setScaleMultiplier(cfg.getCodeSpanBgScale());
            bg->setColor(cfg.getCodeSpanBgColor());
            bg->setOpacity(cfg.getCodeSpanBgOpacity());
            bg->setAnchorPoint({0.f, 0.f});
            bg->setContentSize({r.size.width * s + padX * 2.f, r.size.height * s});
            bg->setPosition({lx + r.origin.x * s - padX, ly + (r.origin.y - H) * s + dy});
            parent->addChild(bg, -1);
        }
    }
}

struct InlineLabel {
    Label* label = nullptr;
    std::vector<CodeSpanRegion> codeSpans;
    size_t linkBegin = 0;
    size_t linkEnd = 0;
};

static InlineLabel buildInlineLabel(
    std::span<md::Inline const> inlines,
    MDConfig const& config,
    InlineContext const& ctx,
    float scale, float maxWidth,
    std::vector<LinkRegion>* links
) {
    InlineLabel out;
    RunGroup group;
    group.links = links;
    group.labelScale = scale;
    group.availWidth = maxWidth;

    out.linkBegin = links ? links->size() : 0;
    collectRuns(inlines, config, ctx, group);
    out.linkEnd = links ? links->size() : 0;

    if (group.runs.empty()) return out;

    auto* label = Label::create(config.getFontRegular());
    if (!label) return out;
    label->setRuns(std::move(group.runs));
    label->setScale(scale);
    label->setAnchorPoint({0.f, 1.f});
    out.label = label;
    out.codeSpans = std::move(group.codeSpans);
    return out;
}

static void placeInlineLabel(
    InlineLabel& il,
    MDConfig const& config,
    float x, float y, CCNode* parent,
    std::vector<LinkRegion>* links
) {
    il.label->setPosition({x, y});
    parent->addChild(il.label);

    if (links) {
        for (size_t i = il.linkBegin; i < il.linkEnd; ++i) {
            (*links)[i].label = il.label;
        }
    }

    addCodeSpanBgs(il.label, il.codeSpans, config, parent);
}

static float renderParagraphInto(
    md::BlockParagraph const& data,
    MDConfig const& config,
    float x, float w, float y, CCNode* parent,
    std::vector<LinkRegion>* links
) {
    float scale = config.getFontScale();

    auto il = buildInlineLabel(data.inlines, config, {}, scale, w, links);
    if (!il.label) return y;

    il.label->setMaxWidth(w / scale);
    placeInlineLabel(il, config, x, y, parent, links);

    return y - il.label->getScaledContentHeight();
}

static float renderHeadingInto(
    md::BlockHeading const& data,
    MDConfig const& config,
    float x, float w, float y, CCNode* parent,
    std::vector<LinkRegion>* links
) {
    InlineContext ctx;
    ctx.bold = true;

    int level = std::clamp(data.level, 1, 6);
    float scale = config.getFontScale() * config.getHeadingScale(level - 1);

    auto il = buildInlineLabel(data.inlines, config, ctx, scale, w, links);
    if (!il.label) return y;

    il.label->setMaxWidth(w / scale);
    placeInlineLabel(il, config, x, y, parent, links);

    float bottom = y - il.label->getScaledContentHeight();

    if (level == 1) {
        float lineY = bottom - config.getHrPaddingBefore();
        auto* line = BreakLine::create(w);
        line->setAnchorPoint({0.f, 1.f});
        line->setPosition({x, lineY});
        parent->addChild(line);
        bottom = lineY - line->getScaledContentHeight();
    }

    return bottom;
}

static float renderHRInto(
    MDConfig const& config,
    float x, float w,
    float y, CCNode* parent
) {
    float lineY = y - config.getHrPaddingBefore();
    auto* line = BreakLine::create(w);
    line->setAnchorPoint({0.f, 1.f});
    line->setPosition({x, lineY});
    parent->addChild(line);
    return lineY - line->getScaledContentHeight() - config.getHrPaddingAfter();
}

static float renderBlockQuoteInto(
    md::BlockBlockQuote const& data,
    MDConfig const& config,
    float x, float w, float y, CCNode* parent,
    std::vector<LinkRegion>* links
) {
    float indent = config.getBlockQuoteIndent();
    float bottom = renderBlocksInto(
        data.children, config, x + indent, w - indent, y,
        parent, links, config.getParagraphSpacing()
    );
    float h = y - bottom;
    if (h <= 0.f) return y;

    auto c = config.getBlockQuoteRuleColor();
    auto* rule = CCLayerColor::create({c.r, c.g, c.b, 255}, config.getBlockQuoteRuleWidth(), h);
    rule->ignoreAnchorPointForPosition(false);
    rule->setAnchorPoint({0.f, 1.f});
    rule->setPosition({x, y});
    parent->addChild(rule, -1);
    return bottom;
}

static float renderListInto(
    md::BlockList const& data,
    MDConfig const& config,
    float x, float w, float y, CCNode* parent,
    std::vector<LinkRegion>* links
) {
    float spacing = data.tight ? 0.f : config.getParagraphSpacing();
    int num = data.start;

    for (size_t i = 0; i < data.items.size(); ++i) {
        auto const& item = data.items[i];
        auto const& li = std::get<md::BlockListItem>(item.data);

        bool onlyNestedLists = !li.children.empty() && std::ranges::all_of(li.children, [](md::Block const& b) {
            return b.kind() == md::BlockKind::List;
        });

        float contentX;
        if (!onlyNestedLists) {
            if (li.isTask) {
                if (data.ordered) ++num;

                auto checkbox = CCSprite::createWithSpriteFrameName(
                    li.isChecked ? "GJ_checkOn_001.png" : "GJ_checkOff_001.png"
                );

                auto scale = 0.65f * config.getFontScale();
                checkbox->setScale(scale);
                CCPoint extra{scale * 31.f, -scale * 32.f};
                checkbox->setPosition(extra * 0.5f + CCPoint{x, y});
                parent->addChild(checkbox);

                contentX = x + extra.x * 1.25f;
            } else {
                std::string marker;
                if (data.ordered) marker = fmt::format("{}{}", num++, config.getListOrderedSeparator());
                else marker = config.getListBulletMarker();

                auto* markerLabel = Label::create(config.getFontRegular());
                if (!markerLabel) return y;
                markerLabel->setScale(config.getFontScale());
                markerLabel->setAnchorPoint({0.f, 1.f});
                markerLabel->setText(std::move(marker));
                markerLabel->setOpacity(config.getListMarkerOpacity());
                markerLabel->setPosition({x, y});
                parent->addChild(markerLabel);

                contentX = x + markerLabel->getScaledContentWidth();
            }
        } else {
            contentX = x + config.getListIndent();
        }

        float contentWidth = w - (contentX - x);
        float bottom = renderBlocksInto(
            li.children, config, contentX, contentWidth,
            y, parent, links, spacing
        );

        y = i + 1 < data.items.size() ? bottom - spacing : bottom;
    }

    return y;
}

static float renderCodeBlockInto(
    md::BlockCodeBlock const& data,
    MDConfig const& config,
    float x, float w, float y,
    CCNode* parent
) {
    float pad = config.getCodeBlockPadding();
    float inset = std::max(0.f, config.getCodeBlockIndent() - pad);
    float bgX = x + inset;
    float bgW = w - inset * 2.f;

    float scale = config.getFontScale();
    float bgScale = config.getCodeBlockBgScale();

    auto* label = Label::create(config.getFontMono());
    if (!label) return y;
    label->setMaxWidth((bgW - pad * 2.f) / scale);
    label->setText(data.content);
    label->setScale(scale);
    label->setAnchorPoint({0.f, 1.f});

    float textH = label->getScaledContentSize().height;
    float bgH = textH + pad * 2.f;

    auto* bg = NineSlice::create("square02b_001.png");
    bg->setScaleMultiplier(bgScale);
    bg->setColor(config.getCodeBlockBgColor());
    bg->setOpacity(config.getCodeBlockBgOpacity());
    bg->setAnchorPoint({0.f, 1.f});
    bg->setPosition({bgX, y});
    bg->setContentSize({bgW, bgH});
    parent->addChild(bg);

    label->setPosition({bgX + pad, y - pad});
    parent->addChild(label);

    return y - bgH;
}

static void solveColumnWidths(std::vector<float> const& nat, float avail, std::vector<float>& out) {
    out = nat;
    if (std::accumulate(nat.begin(), nat.end(), 0.f) <= avail) return;

    std::vector<bool> fixed(nat.size(), false);
    float remaining = avail;
    size_t freeCols = nat.size();

    for (bool changed = true; changed && freeCols;) {
        changed = false;
        float share = remaining / freeCols;
        for (size_t i = 0; i < nat.size(); ++i) {
            if (!fixed[i] && nat[i] <= share) {
                fixed[i] = true;
                out[i] = nat[i];
                remaining -= nat[i];
                --freeCols;
                changed = true;
            }
        }
    }

    if (freeCols) {
        for (size_t i = 0; i < nat.size(); ++i) {
            if (!fixed[i]) out[i] = remaining / freeCols;
        }
    }
}

static Label::Alignment toLabelAlign(md::Alignment a) {
    switch (a) {
        case md::Alignment::Center: return Label::Alignment::Center;
        case md::Alignment::Right: return Label::Alignment::Right;
        default: return Label::Alignment::Left;
    }
}

static float renderTableInto(
    md::BlockTable const& data,
    MDConfig const& cfg,
    float x, float w, float y, CCNode* parent,
    std::vector<LinkRegion>* links
) {
    size_t rows = data.rows.size();
    size_t cols = 0;
    for (auto const& r : data.rows) {
        cols = std::max(cols, r.cells.size());
    }

    if (!rows || !cols) return y;

    float pad = cfg.getTableCellPadding();
    float bw = cfg.getTableBorderWidth();
    float scale = cfg.getFontScale();

    std::vector<std::vector<InlineLabel>> cells(rows);
    std::vector<float> natural(cols, 0.f);
    for (size_t r = 0; r < rows; ++r) {
        InlineContext ctx;
        ctx.bold = data.rows[r].isHeader;

        for (size_t c = 0; c < data.rows[r].cells.size(); ++c) {
            auto il = buildInlineLabel(data.rows[r].cells[c].inlines, cfg, ctx, scale, 0.f, links);
            if (il.label) {
                natural[c] = std::max(natural[c], il.label->getScaledContentWidth());
            }
            cells[r].push_back(std::move(il));
        }
    }

    std::vector<float> colW;
    solveColumnWidths(natural, std::max(0.f, w - bw * (cols + 1) - pad * 2.f * cols), colW);
    std::vector<float> colX(cols + 1);
    colX[0] = x + bw;
    for (size_t c = 0; c < cols; ++c) {
        colX[c + 1] = colX[c] + colW[c] + pad * 2.f + bw;
    }

    float tableW = colX[cols] - x;
    float minLine = cfg.getFontRegular() ? cfg.getFontRegular()->getCommonHeightScaled() * scale : 0.f;
    std::vector<float> rowTop(rows + 1);
    rowTop[0] = y - bw;

    for (size_t r = 0; r < rows; ++r) {
        float h = minLine;
        for (size_t c = 0; c < cells[r].size(); ++c) {
            auto* l = cells[r][c].label;
            if (!l) continue;
            l->setBreakWords(true);
            l->setMaxWidth(colW[c] / scale + 0.01f);
            l->setAlignment(toLabelAlign(data.rows[r].cells[c].align));
            h = std::max(h, l->getScaledContentHeight());
        }
        rowTop[r + 1] = rowTop[r] - (h + pad * 2.f) - bw;
    }

    float bottom = rowTop[rows];
    auto rect = [&](float rx, float ry, float rw, float rh, ccColor4B col, int z) {
        auto* n = CCLayerColor::create(col, rw, rh);
        n->ignoreAnchorPointForPosition(false);
        n->setAnchorPoint({0.f, 1.f});
        n->setPosition({rx, ry});
        parent->addChild(n, z);
    };

    auto hb = cfg.getTableHeaderBgColor();
    auto bc = cfg.getTableBorderColor();

    for (size_t r = 0; r < rows; ++r) {
        if (data.rows[r].isHeader) rect(
            x, rowTop[r], tableW, rowTop[r] - rowTop[r + 1] - bw,
            {hb.r, hb.g, hb.b, cfg.getTableHeaderBgOpacity()}, -2
        );
    }

    for (size_t k = 0; k <= rows; ++k) rect(
        x, rowTop[k] + bw, tableW, bw,
        {bc.r, bc.g, bc.b, 255}, -1
    );

    for (size_t j = 0; j <= cols; ++j) rect(
        colX[j] - bw, y, bw, y - bottom,
        {bc.r, bc.g, bc.b, 255}, -1
    );

    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cells[r].size(); ++c) {
            auto& il = cells[r][c];
            if (!il.label) continue;

            float off = 0.f;
            float lw = il.label->getScaledContentWidth();
            switch (data.rows[r].cells[c].align) {
                case md::Alignment::Center:
                    off = (colW[c] - lw) * 0.5f;
                    break;
                case md::Alignment::Right:
                    off = colW[c] - lw;
                    break;
                default: break;
            }

            placeInlineLabel(il, cfg, colX[c] + pad + off, rowTop[r] - pad, parent, links);
        }
    }

    return bottom;
}

static float renderBlockInto(
    md::Block const& block,
    MDConfig const& config,
    float x, float w, float y, CCNode* parent,
    std::vector<LinkRegion>* links
) {
    return std::visit([&](auto const& data) -> float {
        using T = std::decay_t<decltype(data)>;

        if constexpr (std::is_same_v<T, md::BlockDocument>) {
            return renderBlocksInto(
                data.children, config, x, w, y, parent, links,
                config.getParagraphSpacing()
            );
        } else if constexpr (std::is_same_v<T, md::BlockParagraph>) {
            return renderParagraphInto(data, config, x, w, y, parent, links);
        } else if constexpr (std::is_same_v<T, md::BlockHeading>) {
            return renderHeadingInto(data, config, x, w, y, parent, links);
        } else if constexpr (std::is_same_v<T, md::BlockHorizontalRule>) {
            return renderHRInto(config, x, w, y, parent);
        } else if constexpr (std::is_same_v<T, md::BlockBlockQuote>) {
            return renderBlockQuoteInto(data, config, x, w, y, parent, links);
        } else if constexpr (std::is_same_v<T, md::BlockList>) {
            return renderListInto(data, config, x, w, y, parent, links);
        } else if constexpr (std::is_same_v<T, md::BlockCodeBlock>) {
            return renderCodeBlockInto(data, config, x, w, y, parent);
        } else if constexpr (std::is_same_v<T, md::BlockTable>) {
            return renderTableInto(data, config, x, w, y, parent, links);
        } else {
            return y;
        }
    }, block.data);
}

static float renderBlocksInto(
    std::vector<md::Block> const& blocks,
    MDConfig const& config,
    float x, float w, float y, CCNode* parent,
    std::vector<LinkRegion>* links, float gap
) {
    for (size_t i = 0; i < blocks.size(); ++i) {
        y = renderBlockInto(blocks[i], config, x, w, y, parent, links);
        if (i + 1 < blocks.size()) y -= gap;
    }
    return y;
}

static float renderMarkdown(
    md::Block const& root,
    MDConfig const& config,
    float contentWidth,
    float padding,
    CCNode* parent,
    std::vector<LinkRegion>* links
) {
    auto bottom = renderBlocksInto(
        std::get<md::BlockDocument>(root.data).children,
        config, padding, contentWidth, -padding * 0.5f, parent, links,
        config.getParagraphSpacing()
    );

    float height = -bottom + padding * 0.5f;
    for (auto child : parent->getChildrenExt<CCNode>()) {
        child->setPositionY(child->getPositionY() + height);
    }

    return height;
}

// MDTextArea

class MDTextArea::Impl {
public:
    std::string m_text;
    MDConfig m_config;
    Ref<ScrollLayer> m_scrollLayer;
    Ref<NineSlice> m_background;
    std::vector<LinkRegion> m_links;

    struct ActivePress {
        std::vector<std::pair<uint32_t, ccColor4B>> saved;
        size_t link = -1;
        CCPoint start;
        bool inside = false;
        bool dragged = false;
    } m_press;
};

MDTextArea::MDTextArea() : m_impl(std::make_unique<Impl>()) {}
MDTextArea::~MDTextArea() = default;

MDTextArea* MDTextArea::create(std::string str, float maxWidth) {
    return create(std::move(str), maxWidth, MDConfig{});
}

MDTextArea* MDTextArea::create(std::string str, float maxWidth, MDConfig config) {
    auto ret = new MDTextArea;
    if (ret->init(std::move(str), maxWidth, std::move(config))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

MDTextArea* MDTextArea::create(std::string str, CCSize size) {
    return create(std::move(str), size, MDConfig{});
}

MDTextArea* MDTextArea::create(std::string str, CCSize size, MDConfig config) {
    auto ret = new MDTextArea;
    if (ret->initClipped(std::move(str), size, std::move(config))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void MDTextArea::updateLabel() {
    auto parsed = md::parse(m_impl->m_text);
    if (!parsed) {
        return;
    }

    CCNode* container = this;
    float fullWidth = this->getContentWidth();
    float contentWidth = fullWidth;
    float padding = 0.f;

    if (m_impl->m_scrollLayer) {
        container = m_impl->m_scrollLayer->m_contentLayer;
        padding = 6.f;
        contentWidth -= padding * 2.f;
    }

    m_impl->m_press = {};
    container->removeAllChildren();

    m_impl->m_links.clear();
    float h = renderMarkdown(
        parsed.unwrap(), m_impl->m_config,
        contentWidth, padding,
        container, &m_impl->m_links
    );

    container->setContentSize({fullWidth, h});

    if (m_impl->m_scrollLayer) {
        m_impl->m_scrollLayer->moveToTop();
    }
}

void MDTextArea::setText(std::string text) {
    m_impl->m_text = std::move(text);
    this->updateLabel();
}

ZStringView MDTextArea::getText() const {
    return m_impl->m_text;
}

void MDTextArea::setString(char const* text) {
    this->setText(text);
}

char const* MDTextArea::getString() {
    return m_impl->m_text.c_str();
}

ScrollLayer* MDTextArea::getScrollLayer() const {
    return m_impl->m_scrollLayer;
}

static std::optional<size_t> linkAt(std::vector<LinkRegion> const& links, CCPoint worldPos) {
    for (size_t i = 0; i < links.size(); ++i) {
        auto& l = links[i];
        if (!l.label) continue;

        auto cp = l.label->getCodepointAt(l.label->convertToNodeSpace(worldPos));
        if (cp && *cp >= l.codepointBegin && *cp < l.codepointEnd) {
            return i;
        }
    }

    return std::nullopt;
}

static void setLinkPressed(
    LinkRegion const& l,
    MDConfig const& config,
    std::vector<std::pair<uint32_t, ccColor4B>>& saved,
    bool pressed
) {
    auto* label = l.label;
    if (!label) return;

    if (pressed) {
        saved.clear();
        uint32_t charIdx = 0;
        auto color = config.getLinkActiveColor();
        for (auto const& g : label->getGlyphInfos()) {
            if (g.kind != Label::GlyphKind::Glyph) continue;
            if (g.stringIndex >= l.codepointBegin && g.stringIndex < l.codepointEnd) {
                auto orig = label->getCharColor(charIdx);
                saved.push_back({charIdx, orig});
                label->setCharColor(charIdx, {color.r, color.g, color.b});
            }
            ++charIdx;
        }
    } else {
        for (auto const& [idx, col] : saved) {
            label->setCharColor(idx, col);
        }
        saved.clear();
    }
}

void MDTextArea::scrollToTop() {
    if (m_impl->m_scrollLayer) {
        m_impl->m_scrollLayer->scrollToTop();
    }
}

bool MDTextArea::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    if (!nodeIsVisible(this)) return false;
    auto pos = touch->getLocation();

    if (m_impl->m_scrollLayer) {
        auto local = this->convertToNodeSpace(pos);
        if (!CCRect{0.f, 0.f, this->getContentWidth(), this->getContentHeight()}.containsPoint(local)) {
            return false;
        }
    }

    auto idx = linkAt(m_impl->m_links, pos);
    if (!idx) return false;

    auto& p = m_impl->m_press;
    p = {};
    p.link = *idx;
    p.start = pos;
    p.inside = true;
    setLinkPressed(m_impl->m_links[*idx], m_impl->m_config, p.saved, true);

    return true;
}

void MDTextArea::ccTouchMoved(CCTouch* touch, CCEvent* event) {
    auto& p = m_impl->m_press;
    if (p.link == -1) return;
    auto pos = touch->getLocation();

    if (!p.dragged && ccpDistance(pos, p.start) > 10.f) p.dragged = true;

    bool inside = !p.dragged && linkAt(m_impl->m_links, pos) == p.link;
    if (inside != p.inside) {
        p.inside = inside;
        setLinkPressed(m_impl->m_links[p.link], m_impl->m_config, p.saved, inside);
    }
}

void MDTextArea::ccTouchEnded(CCTouch* touch, CCEvent* event) {
    auto& p = m_impl->m_press;
    if (p.link == -1) return;

    auto link = m_impl->m_links[p.link];
    bool fire = p.inside && !p.dragged;
    if (p.inside) setLinkPressed(link, m_impl->m_config, p.saved, false);
    p = {};

    if (fire) this->activateLink(link.linkKind, link.href);
}

void MDTextArea::ccTouchCancelled(CCTouch* touch, CCEvent* event) {
    auto& p = m_impl->m_press;
    if (p.link != -1 && p.inside) {
        setLinkPressed(m_impl->m_links[p.link], m_impl->m_config, p.saved, false);
    }
    p = {};
}

void MDTextArea::registerWithTouchDispatcher() {
    CCTouchDispatcher::get()->addTargetedDelegate(
        this, kCCMenuHandlerPriority,
        m_impl->m_scrollLayer == nullptr
    );
}

bool MDTextArea::init(std::string str, float maxWidth, MDConfig config) {
    if (!CCLayer::init()) return false;

    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint({ .5f, .5f });
    this->setContentSize({maxWidth, 0});

    m_impl->m_text = std::move(str);
    m_impl->m_config = std::move(config);

    this->updateLabel();
    this->setTouchEnabled(true);

    return true;
}

bool MDTextArea::initClipped(std::string str, CCSize size, MDConfig config) {
    if (!CCLayer::init()) return false;

    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint({ .5f, .5f });
    this->setContentSize(size);

    m_impl->m_text = std::move(str);
    m_impl->m_config = std::move(config);

    m_impl->m_background = NineSlice::create("square02b_001.png");
    m_impl->m_background->setID("background");
    m_impl->m_background->setScaleMultiplier(0.5f);
    m_impl->m_background->setColor({ 0, 0, 0 });
    m_impl->m_background->setOpacity(75);
    m_impl->m_background->setContentSize(size);
    this->addChildAtPosition(m_impl->m_background, Anchor::Center);

    m_impl->m_scrollLayer = ScrollLayer::create(size);
    this->addChildAtPosition(m_impl->m_scrollLayer, Anchor::Center);

    this->updateLabel();
    this->setTouchEnabled(true);

    return true;
}

void MDTextArea::activateLink(md::LinkKind kind, std::string href) {
    if (kind == md::LinkKind::URL) {
        // TODO: anchor links
        if (href.starts_with('#')) return;

        auto content = fmt::format("Links are spooky! Are you sure you want to go to <cy>{}</c>?", href);
        createQuickPopup(
            "Hold Up!",
            std::move(content),
            "Cancel", "Yes", 360.f,
            [href = std::move(href)](FLAlertLayer*, bool btn2) {
                if (btn2) web::openLinkInBrowser(href);
            },
            true, true
        );

        return;
    }

    auto pos = href.find(':');
    if (pos == std::string::npos) {
        return;
    }

    auto arg = std::string_view{href}.substr(pos + 1);
    if (kind == md::LinkKind::GeodeMod) {
        (void) openInfoPopup(std::string(arg));
        return;
    }

    auto res = numFromString<int>(arg);
    if (!res) {
        createQuickPopup(
            "Error",
            fmt::format(
                "Invalid {} ID: <cr>{}</c>. This is probably the mod developer's fault, report the bug to them.",
                kind == md::LinkKind::GDLevel ? "level" : "profile",
                arg
            ),
            "Ok"
        );
        return;
    }

    switch (kind) {
        case md::LinkKind::GDLevel: {
            auto searchObject = GJSearchObject::create(SearchType::Type19, fmt::format("{}&gameVersion=22", res.unwrap()));
            switchToScene(LevelBrowserLayer::scene(searchObject));
            break;
        }
        case md::LinkKind::GDUser: {
            ProfilePage::create(res.unwrap(), false)->show();
            break;
        }
        default: break;
    }
}
