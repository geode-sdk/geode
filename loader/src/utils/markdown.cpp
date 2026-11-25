#include <Geode/loader/Log.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/markdown.hpp>
#include <md4c.h>

namespace geode::utils::md {
    struct Frame {
        enum class Kind : uint8_t {
            Document,
            List,
            ListItem,
            BlockQuote,

            Paragraph,
            Heading,
            Emphasis,
            Strong,
            Strikethrough,
            Underline,
            Link,
            ColorTag,

            CodeBlock,
            CodeSpan,
            Image,

            Table,
            TableRow,
            TableCell,

            Sink,
        };

        std::vector<Inline> inlines;
        std::vector<Block> blocks;
        std::vector<TableRow> tableRows;
        std::vector<TableCell> tableCells;
        std::string rawText;

        Kind kind = Kind::Document;

        // ListItem
        bool isTask = false;
        bool isChecked = false;

        // List
        bool listOrdered = false;
        int listStart = 1;
        bool listTight = false;

        // ColorTag
        cocos2d::ccColor3B color{255, 255, 255};

        // Heading
        int headingLevel = 1;

        // Table
        bool inThead = false;
        bool isHeaderRow = false;
        Alignment cellAlignment = Alignment::None;

        bool imageIsSpriteFrame = false; // placed here to minimize padding

        // Link
        LinkKind linkKind = LinkKind::URL;
        bool isAutolink = false;
        std::string linkHref;

        // CodeBlock
        std::string codeLang;

        // Image
        std::string imageSrc;
        std::optional<float> imageScale;
        std::optional<float> imageWidth;
        std::optional<float> imageHeight;
    };

    struct MDAstBuilder {
        std::vector<Frame> stack;
        std::string error;

        MDAstBuilder() {
            stack.emplace_back(Frame{ .kind = Frame::Kind::Document });
        }

        Frame& pushFrame(Frame::Kind kind) {
            stack.emplace_back(Frame{ .kind = kind });
            return stack.back();
        }

        static void flushInlines(Frame& frame) {
            if (frame.inlines.empty()) return;
            frame.blocks.push_back(Block{BlockParagraph{std::move(frame.inlines)}});
        }

        void appendBlock(Block block) {
            if (stack.empty()) return;
            this->flushInlines(stack.back());
            stack.back().blocks.push_back(std::move(block));
        }

        void appendInline(Inline inl) {
            if (stack.empty()) return;
            stack.back().inlines.push_back(std::move(inl));
        }

        void closeColorTags() {
            while (!stack.empty() && stack.back().kind == Frame::Kind::ColorTag) {
                this->popFrame();
            }
        }

        static LinkKind classifyLink(std::string_view href) {
            if (href.starts_with("user:")) return LinkKind::GDUser;
            if (href.starts_with("level:")) return LinkKind::GDLevel;
            if (href.starts_with("mod:")) return LinkKind::GeodeMod;
            return LinkKind::URL;
        }

        // cheaper version of string::trim
        // TODO: replace it when we fix actual string::trim
        static std::string_view trimView(std::string_view str) {
            constexpr char WHITESPACE[] = " \f\n\r\t\v";
            size_t start = str.find_first_not_of(WHITESPACE);
            return start == -1 ? std::string_view{} : str.substr(start, str.find_last_not_of(WHITESPACE) + 1 - start);
        }

        static std::optional<cocos2d::ccColor3B> parseColorCode(std::string_view tag) {
            if (tag.empty() || tag[0] != 'c') return std::nullopt;
            auto sv = tag.substr(1);
            if (sv.empty()) return std::nullopt;

            if (sv[0] == '-') {
                if (auto c = cocos::cc3bFromHexString(sv.substr(1))) {
                    return c.unwrap();
                }
                return std::nullopt;
            }

            // Support the old form of <carbitaryletters hex>
            if (sv[0] == ' ') {
                auto trimmed = trimView(sv.substr(1));
                if (auto c = cocos::cc3bFromHexString(trimmed)) {
                    return c.unwrap();
                }
                return std::nullopt;
            }

            if (sv.size() == 1) {
                switch (sv[0]) {
                    case 'a': return cocos2d::ccc3(150, 50, 255);
                    case 'b': return cocos2d::ccc3(74, 82, 225);
                    case 'c': return cocos2d::ccc3(255, 255, 150);
                    case 'd': return cocos2d::ccc3(255, 150, 255);
                    case 'f': return cocos2d::ccc3(150, 255, 255);
                    case 'g': return cocos2d::ccc3(64, 227, 72);
                    case 'j': return cocos2d::ccc3(50, 200, 255);
                    case 'l': return cocos2d::ccc3(96, 171, 239);
                    case 'o': return cocos2d::ccc3(255, 165, 75);
                    case 'p': return cocos2d::ccc3(255, 0, 255);
                    case 'r': return cocos2d::ccc3(255, 90, 90);
                    case 's': return cocos2d::ccc3(255, 220, 65);
                    case 'y': return cocos2d::ccc3(255, 255, 0);
                    default: return cocos2d::ccc3(255, 0, 0);
                }
            }

            return std::nullopt;
        }

        static void parseImageSrcInto(Frame& f, std::string src) {
            auto q = src.find('?');
            if (q != std::string::npos) {
                std::string_view v{src};
                auto query = v.substr(q + 1);

                size_t pos = 0;
                while (pos < query.size()) {
                    auto amp = query.find('&', pos);
                    auto param = query.substr(pos, amp == std::string_view::npos ? std::string_view::npos : amp - pos);

                    auto eq = param.find('=');
                    if (eq != std::string::npos) {
                        auto key = param.substr(0, eq);
                        auto val = param.substr(eq + 1);

                        if (key == "scale") {
                            if (auto r = utils::numFromString<float>(val)) f.imageScale = r.unwrap();
                        } else if (key == "width") {
                            if (auto r = utils::numFromString<float>(val)) f.imageWidth = r.unwrap();
                        } else if (key == "height") {
                            if (auto r = utils::numFromString<float>(val)) f.imageHeight = r.unwrap();
                        }
                    }

                    if (amp == std::string::npos) break;
                    pos = amp + 1;
                }

                src = src.substr(0, q);
            }

            if (src.starts_with("frame:")) {
                f.imageIsSpriteFrame = true;
                src = src.substr(6);
            }

            f.imageSrc = std::move(src);
        }

        static Alignment mapAlignment(MD_ALIGN align) {
            switch (align) {
                case MD_ALIGN_LEFT: return Alignment::Left;
                case MD_ALIGN_CENTER: return Alignment::Center;
                case MD_ALIGN_RIGHT: return Alignment::Right;
                case MD_ALIGN_DEFAULT: default:
                    return Alignment::None;
            }
        }

        void popFrame() {
            Frame frame = std::move(stack.back());
            stack.pop_back();

            if (stack.empty()) {
                log::warn("popFrame underflow in markdown parse");
                return;
            }

            Frame& parent = stack.back();
            if (frame.kind == Frame::Kind::ListItem || frame.kind == Frame::Kind::BlockQuote) {
                this->flushInlines(frame);
            }

            switch (frame.kind) {
                case Frame::Kind::Paragraph:
                    this->appendBlock(Block{BlockParagraph{std::move(frame.inlines)}});
                    break;

                case Frame::Kind::Heading:
                    this->appendBlock(Block{BlockHeading{
                        .level = frame.headingLevel,
                        .inlines = std::move(frame.inlines),
                    }});
                    break;

                case Frame::Kind::CodeBlock:
                    if (frame.rawText.ends_with('\n')) frame.rawText.pop_back();
                    this->appendBlock(Block{BlockCodeBlock{
                        .lang = std::move(frame.codeLang),
                        .content = std::move(frame.rawText),
                    }});
                    break;

                case Frame::Kind::List:
                    this->appendBlock(Block{BlockList{
                        .start = frame.listStart,
                        .ordered = frame.listOrdered,
                        .tight = frame.listTight,
                        .items = std::move(frame.blocks),
                    }});
                    break;

                case Frame::Kind::ListItem:
                    this->appendBlock(Block{BlockListItem{
                        .isTask = frame.isTask,
                        .isChecked = frame.isChecked,
                        .children = std::move(frame.blocks),
                    }});
                    break;

                case Frame::Kind::BlockQuote:
                    this->appendBlock(Block{BlockBlockQuote{
                        .children = std::move(frame.blocks),
                    }});
                    break;

                case Frame::Kind::Emphasis:
                    parent.inlines.push_back(Inline{InlineEmphasis{std::move(frame.inlines)}});
                    break;

                case Frame::Kind::Strong:
                    parent.inlines.push_back(Inline{InlineStrong{std::move(frame.inlines)}});
                    break;

                case Frame::Kind::Strikethrough:
                    parent.inlines.push_back(Inline{InlineStrikethrough{std::move(frame.inlines)}});
                    break;

                case Frame::Kind::Underline:
                    parent.inlines.push_back(Inline{InlineUnderline{std::move(frame.inlines)}});
                    break;

                case Frame::Kind::Link:
                    parent.inlines.push_back(Inline{InlineLink{
                        .href = std::move(frame.linkHref),
                        .linkKind = frame.linkKind,
                        .isAutoLink = frame.isAutolink,
                        .children = std::move(frame.inlines),
                    }});
                    break;

                case Frame::Kind::ColorTag:
                    parent.inlines.push_back(Inline{InlineColorTag{
                        .color = frame.color,
                        .children = std::move(frame.inlines),
                    }});
                    break;

                case Frame::Kind::CodeSpan:
                    parent.inlines.push_back(Inline{InlineCode{std::move(frame.rawText)}});
                    break;

                case Frame::Kind::Image: {
                    std::string alt;
                    for (auto& n : frame.inlines) {
                        if (auto* t = std::get_if<InlineText>(&n.data)) {
                            alt.append(t->text);
                        }
                    }

                    parent.inlines.push_back(Inline{InlineImage{
                        .src = std::move(frame.imageSrc),
                        .alt = std::move(alt),
                        .isSpriteFrame = frame.imageIsSpriteFrame,
                        .scale = frame.imageScale,
                        .width = frame.imageWidth,
                        .height = frame.imageHeight,
                    }});
                    break;
                }

                case Frame::Kind::Table:
                    this->appendBlock(Block{ BlockTable{
                        .rows = std::move(frame.tableRows),
                    }});
                    break;

                case Frame::Kind::TableRow:
                    parent.tableRows.push_back(TableRow{
                        .cells = std::move(frame.tableCells),
                        .isHeader = frame.isHeaderRow,
                    });
                    break;

                case Frame::Kind::TableCell:
                    parent.tableCells.push_back(TableCell{
                        .inlines = std::move(frame.inlines),
                        .align = frame.cellAlignment,
                    });
                    break;

                default: break;
            }
        }

        static int onEnterBlock(MD_BLOCKTYPE type, void* detail, void* userdata) {
            auto* self = static_cast<MDAstBuilder*>(userdata);
            switch (type) {
                case MD_BLOCK_DOC:
                    return 0;

                case MD_BLOCK_P:
                    self->pushFrame(Frame::Kind::Paragraph);
                    return 0;

                case MD_BLOCK_H: {
                    auto* d = static_cast<MD_BLOCK_H_DETAIL*>(detail);
                    self->pushFrame(Frame::Kind::Heading).headingLevel = d->level;
                    return 0;
                }

                case MD_BLOCK_CODE: {
                    auto* d = static_cast<MD_BLOCK_CODE_DETAIL*>(detail);
                    auto& f = self->pushFrame(Frame::Kind::CodeBlock);
                    if (d->lang.size > 0) {
                        f.codeLang.assign(d->lang.text, d->lang.size);
                    }
                    return 0;
                }

                case MD_BLOCK_HR:
                    self->appendBlock(Block{BlockHorizontalRule{}});
                    return 0;

                case MD_BLOCK_UL: {
                    auto* d = static_cast<MD_BLOCK_UL_DETAIL*>(detail);
                    auto& f = self->pushFrame(Frame::Kind::List);
                    f.listOrdered = false;
                    f.listTight = d->is_tight != 0;
                    return 0;
                }

                case MD_BLOCK_OL: {
                    auto* d = static_cast<MD_BLOCK_OL_DETAIL*>(detail);
                    auto& f = self->pushFrame(Frame::Kind::List);
                    f.listOrdered = true;
                    f.listStart = static_cast<int>(d->start);
                    f.listTight = d->is_tight != 0;
                    return 0;
                }

                case MD_BLOCK_LI: {
                    auto* d = static_cast<MD_BLOCK_LI_DETAIL*>(detail);
                    auto& f = self->pushFrame(Frame::Kind::ListItem);
                    f.isTask = d->is_task != 0;
                    f.isChecked = f.isTask && (d->task_mark == 'x' || d->task_mark == 'X');
                    return 0;
                }

                case MD_BLOCK_QUOTE:
                    self->pushFrame(Frame::Kind::BlockQuote);
                    return 0;

                case MD_BLOCK_TABLE:
                    self->pushFrame(Frame::Kind::Table);
                    return 0;

                case MD_BLOCK_THEAD:
                    if (!self->stack.empty() && self->stack.back().kind == Frame::Kind::Table) {
                        self->stack.back().inThead = true;
                    }
                    return 0;

                case MD_BLOCK_TBODY:
                    if (!self->stack.empty() && self->stack.back().kind == Frame::Kind::Table) {
                        self->stack.back().inThead = false;
                    }
                    return 0;

                case MD_BLOCK_TR: {
                    auto& f = self->pushFrame(Frame::Kind::TableRow);
                    if (self->stack.size() >= 2) {
                        auto& parent = self->stack[self->stack.size() - 2];
                        if (parent.kind == Frame::Kind::Table) {
                            f.isHeaderRow = parent.inThead;
                        }
                    }
                    return 0;
                }

                case MD_BLOCK_TH:
                case MD_BLOCK_TD: {
                    auto* d = static_cast<MD_BLOCK_TD_DETAIL*>(detail);
                    auto& f = self->pushFrame(Frame::Kind::TableCell);
                    f.cellAlignment = MDAstBuilder::mapAlignment(d->align);
                    return 0;
                }

                default:
                    self->pushFrame(Frame::Kind::Sink);
                    return 0;
            }
        }

        static int onLeaveBlock(MD_BLOCKTYPE type, void* detail, void* userdata) {
            auto* self = static_cast<MDAstBuilder*>(userdata);
            switch (type) {
                case MD_BLOCK_DOC:
                case MD_BLOCK_HR:
                case MD_BLOCK_THEAD:
                case MD_BLOCK_TBODY:
                    return 0;

                default:
                    self->closeColorTags();
                    self->popFrame();
                    return 0;
            }
        }

        static int onEnterSpan(MD_SPANTYPE type, void* detail, void* userdata) {
            auto* self = static_cast<MDAstBuilder*>(userdata);
            switch (type) {
                case MD_SPAN_EM:
                    self->pushFrame(Frame::Kind::Emphasis);
                    return 0;

                case MD_SPAN_STRONG:
                    self->pushFrame(Frame::Kind::Strong);
                    return 0;

                case MD_SPAN_DEL:
                    self->pushFrame(Frame::Kind::Strikethrough);
                    return 0;

                case MD_SPAN_U:
                    self->pushFrame(Frame::Kind::Underline);
                    return 0;

                case MD_SPAN_A: {
                    auto* d = static_cast<MD_SPAN_A_DETAIL*>(detail);
                    auto& f = self->pushFrame(Frame::Kind::Link);
                    f.linkHref.assign(d->href.text, d->href.size);
                    f.isAutolink = d->is_autolink != 0;
                    f.linkKind = classifyLink(f.linkHref);
                    return 0;
                }

                case MD_SPAN_IMG: {
                    auto* d = static_cast<MD_SPAN_IMG_DETAIL*>(detail);
                    auto& f = self->pushFrame(Frame::Kind::Image);
                    parseImageSrcInto(f, std::string(d->src.text, d->src.size));
                    return 0;
                }

                case MD_SPAN_CODE:
                    self->pushFrame(Frame::Kind::CodeSpan);
                    return 0;

                default:
                    self->pushFrame(Frame::Kind::Sink);
                    return 0;
            }
        }

        static int onLeaveSpan(MD_SPANTYPE type, void* detail, void* userdata) {
            auto* self = static_cast<MDAstBuilder*>(userdata);
            self->closeColorTags();
            self->popFrame();
            return 0;
        }

        int handleHTML(std::string_view tag) {
            if (tag.size() < 3 || tag.front() != '<' || tag.back() != '>') {
                this->appendInline(Inline{InlineText{std::string(tag)}});
                return 0;
            }

            auto inner = tag.substr(1, tag.size() - 2);
            if (inner == "/c") {
                if (!stack.empty() && stack.back().kind == Frame::Kind::ColorTag) {
                    this->popFrame();
                }
                return 0;
            }

            if (!inner.empty() && inner[0] == 'c') {
                if (auto color = parseColorCode(inner)) {
                    this->pushFrame(Frame::Kind::ColorTag).color = *color;
                    return 0;
                }
            }

            this->appendInline(Inline{InlineText{ std::string(tag)}});
            return 0;
        }

        static int onText(MD_TEXTTYPE type, MD_CHAR const* text, MD_SIZE size, void* userdata) {
            auto* self = static_cast<MDAstBuilder*>(userdata);
            auto sv = std::string_view(text, size);

            auto isCodeContext = [&]() {
                auto k = self->stack.back().kind;
                return k == Frame::Kind::CodeBlock || k == Frame::Kind::CodeSpan;
            };

            switch (type) {
                case MD_TEXT_NORMAL:
                case MD_TEXT_CODE: {
                    if (isCodeContext()) {
                        self->stack.back().rawText.append(text, size);
                    } else {
                        self->appendInline(Inline{InlineText{std::string(sv)}});
                    }
                    return 0;
                }

                case MD_TEXT_ENTITY:
                    // TODO: decode entity
                    if (isCodeContext()) {
                        self->stack.back().rawText.append(text, size);
                    } else {
                        self->appendInline(Inline{InlineText{std::string(sv)}});
                    }
                    return 0;

                case MD_TEXT_BR:
                    self->appendInline(Inline{InlineHardBreak{}});
                    return 0;

                case MD_TEXT_SOFTBR:
                    self->appendInline(Inline{InlineSoftBreak{}});
                    return 0;

                case MD_TEXT_NULLCHAR:
                    if (isCodeContext()) {
                        self->stack.back().rawText.append(text, size);
                    } else {
                        self->appendInline(Inline{InlineText{std::string("\uFFFD")}});
                    }
                    return 0;

                case MD_TEXT_HTML:
                    return self->handleHTML(sv);

                default:
                    return 0;
            }
        }
    };

    Result<Block> parse(std::string_view text) {
        MDAstBuilder builder;

        MD_PARSER parser{};
        parser.abi_version = 0;
        parser.flags = MD_FLAG_UNDERLINE
            | MD_FLAG_STRIKETHROUGH
            | MD_FLAG_PERMISSIVEURLAUTOLINKS
            | MD_FLAG_PERMISSIVEWWWAUTOLINKS
            | MD_FLAG_TASKLISTS
            | MD_FLAG_TABLES;
        parser.enter_block = &MDAstBuilder::onEnterBlock;
        parser.leave_block = &MDAstBuilder::onLeaveBlock;
        parser.enter_span = &MDAstBuilder::onEnterSpan;
        parser.leave_span = &MDAstBuilder::onLeaveSpan;
        parser.text = &MDAstBuilder::onText;
        parser.debug_log = nullptr;
        parser.syntax = nullptr;

        if (md_parse(text.data(), static_cast<MD_SIZE>(text.size()), &parser, &builder)) {
            return Err("failed to parse markdown");
        }

        if (!builder.error.empty()) {
            return Err(std::move(builder.error));
        }

        if (builder.stack.size() != 1 || builder.stack.back().kind != Frame::Kind::Document) {
            return Err("parser stack did not unwind correctly");
        }

        return Ok(Block{BlockDocument{ std::move(builder.stack.back().blocks) }});
    }
}