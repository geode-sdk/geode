#pragma once
#include <Geode/Result.hpp>
#include <ccTypes.h>

namespace geode::utils::md {
    enum class Alignment : uint8_t {
        None,
        Left,
        Center,
        Right,
    };

    struct Inline;
    struct Block;

    struct InlineText { std::string text; };
    struct InlineCode { std::string code; };
    struct InlineSoftBreak {};
    struct InlineHardBreak {};
    struct InlineEmphasis { std::vector<Inline> children; };
    struct InlineStrong { std::vector<Inline> children; };
    struct InlineStrikethrough { std::vector<Inline> children; };
    struct InlineUnderline { std::vector<Inline> children; };

    enum class LinkKind : uint8_t {
        URL,      // http(s) or unknown scheme
        GDUser,   // user:<accountID>
        GDLevel,  // level:<levelID>
        GeodeMod, // mod:<id>
    };

    struct InlineLink {
        std::string href;
        LinkKind linkKind = LinkKind::URL;
        bool isAutoLink = false;
        std::vector<Inline> children;
    };

    struct InlineImage {
        std::string src;
        std::string alt;
        bool isSpriteFrame = false;
        std::optional<float> scale;
        std::optional<float> width;
        std::optional<float> height;
    };

    struct InlineColorTag {
        cocos2d::ccColor3B color{255, 255, 255};
        std::vector<Inline> children;
    };

    using InlineData = std::variant<
        InlineText,
        InlineCode,
        InlineSoftBreak,
        InlineHardBreak,
        InlineEmphasis,
        InlineStrong,
        InlineStrikethrough,
        InlineUnderline,
        InlineLink,
        InlineImage,
        InlineColorTag
    >;

    enum class InlineKind : uint8_t {
        Text,
        Code,
        SoftBreak,
        HardBreak,
        Emphasis,
        Strong,
        Strikethrough,
        Underline,
        Link,
        Image,
        ColorTag,
    };

    struct Inline {
        InlineData data;

        InlineKind kind() const noexcept {
            return static_cast<InlineKind>(data.index());
        }
    };

    struct TableCell {
        std::vector<Inline> inlines;
        Alignment align = Alignment::None;
    };

    struct TableRow {
        std::vector<TableCell> cells;
        bool isHeader = false;
    };

    struct BlockTable { std::vector<TableRow> rows; };
    struct BlockDocument { std::vector<Block> children; };
    struct BlockParagraph { std::vector<Inline> inlines; };
    struct BlockHeading { int level = 1; std::vector<Inline> inlines; };
    struct BlockCodeBlock { std::string lang; std::string content; };
    struct BlockHorizontalRule {};

    struct BlockList {
        int start = 1;
        bool ordered = false;
        bool tight = false;
        std::vector<Block> items;
    };

    struct BlockListItem {
        bool isTask = false;
        bool isChecked = false;
        std::vector<Block> children;
    };

    struct BlockBlockQuote { std::vector<Block> children; };

    using BlockData = std::variant<
        BlockDocument,
        BlockParagraph,
        BlockHeading,
        BlockCodeBlock,
        BlockHorizontalRule,
        BlockList,
        BlockListItem,
        BlockBlockQuote,
        BlockTable
    >;

    enum class BlockKind : uint8_t {
        Document,
        Paragraph,
        Heading,
        CodeBlock,
        HorizontalRule,
        List,
        ListItem,
        BlockQuote,
        Table,
    };

    struct Block {
        BlockData data;

        BlockKind kind() const noexcept {
            return static_cast<BlockKind>(data.index());
        }
    };

    GEODE_DLL Result<Block> parse(std::string_view text);
}