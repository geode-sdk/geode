#pragma once

#include "Label.hpp"
#include "ScrollLayer.hpp"

#include <Geode/utils/markdown.hpp>

struct MDParser;
class CCScrollLayerExt;

namespace geode {
    /// Configuration for MDTextArea, which allows setting custom sizes/fonts/colors/etc.
    class GEODE_DLL MDConfig {
    public:
        MDConfig();
        ~MDConfig();

        MDConfig(MDConfig const&);
        MDConfig(MDConfig&&) noexcept;
        MDConfig& operator=(MDConfig const&);
        MDConfig& operator=(MDConfig&&) noexcept;

        /// Create a deep copy of this config
        MDConfig clone() const;

        void setFontRegular(BitmapFont* font);
        BitmapFont* getFontRegular() const noexcept;
        void setFontBold(BitmapFont* font);
        BitmapFont* getFontBold() const noexcept;
        void setFontItalic(BitmapFont* font);
        BitmapFont* getFontItalic() const noexcept;
        void setFontBoldItalic(BitmapFont* font);
        BitmapFont* getFontBoldItalic() const noexcept;
        void setFontMono(BitmapFont* font);
        BitmapFont* getFontMono() const noexcept;

        BitmapFont* resolveFont(bool bold, bool italic, bool mono) const noexcept;

        void setFontScale(float scale);
        float getFontScale() const noexcept;

        std::array<float, 6> const& getHeadingScale() const noexcept;
        void setHeadingScale(std::array<float, 6> const& scales);

        void setHeadingScale(uint8_t level, float scale);
        float getHeadingScale(uint8_t level) const noexcept;

        void setLinkColor(cocos2d::ccColor3B color);
        cocos2d::ccColor3B getLinkColor() const noexcept;

        void setLinkActiveColor(cocos2d::ccColor3B color);
        cocos2d::ccColor3B getLinkActiveColor() const noexcept;

        void setParagraphSpacing(float spacing);
        float getParagraphSpacing() const noexcept;

        void setListIndent(float indent);
        float getListIndent() const noexcept;

        void setListBulletMarker(std::string marker);
        std::string const& getListBulletMarker() const noexcept;
        void setListOrderedSeparator(std::string separator);
        std::string const& getListOrderedSeparator() const noexcept;

        void setListMarkerOpacity(GLubyte opacity);
        GLubyte getListMarkerOpacity() const noexcept;

        void setCodeBlockIndent(float indent);
        float getCodeBlockIndent() const noexcept;
        void setCodeBlockPadding(float padding);
        float getCodeBlockPadding() const noexcept;
        void setCodeBlockBgScale(float scale);
        float getCodeBlockBgScale() const noexcept;
        void setCodeBlockBgOpacity(GLubyte opacity);
        GLubyte getCodeBlockBgOpacity() const noexcept;
        void setCodeBlockBgColor(cocos2d::ccColor3B color);
        cocos2d::ccColor3B getCodeBlockBgColor() const noexcept;

        void setCodeSpanBgScale(float scale);
        float getCodeSpanBgScale() const noexcept;
        void setCodeSpanBgExtraWidth(float width);
        float getCodeSpanBgExtraWidth() const noexcept;
        void setCodeSpanBgOffsetY(float offset);
        float getCodeSpanBgOffsetY() const noexcept;
        void setCodeSpanTextOffsetY(float offset);
        float getCodeSpanTextOffsetY() const noexcept;
        void setCodeSpanBgOpacity(GLubyte opacity);
        GLubyte getCodeSpanBgOpacity() const noexcept;
        void setCodeSpanBgColor(cocos2d::ccColor3B color);
        cocos2d::ccColor3B getCodeSpanBgColor() const noexcept;

        void setHrPaddingBefore(float padding);
        float getHrPaddingBefore() const noexcept;
        void setHrPaddingAfter(float padding);
        float getHrPaddingAfter() const noexcept;

        void setImageDefaultScale(float value);
        float getImageDefaultScale() const noexcept;
        void setImageMinScale(float value);
        float getImageMinScale() const noexcept;
        void setImageMaxDim(float value);
        float getImageMaxDim() const noexcept;

        void setBlockQuoteIndent(float indent);
        float getBlockQuoteIndent() const noexcept;
        void setBlockQuoteRuleWidth(float width);
        float getBlockQuoteRuleWidth() const noexcept;
        void setBlockQuoteRuleColor(cocos2d::ccColor3B color);
        cocos2d::ccColor3B getBlockQuoteRuleColor() const noexcept;

        void setTableCellPadding(float padding);
        float getTableCellPadding() const noexcept;
        void setTableBorderWidth(float width);
        float getTableBorderWidth() const noexcept;
        void setTableHeaderBgOpacity(GLubyte opacity);
        GLubyte getTableHeaderBgOpacity() const noexcept;
        void setTableBorderColor(cocos2d::ccColor3B color);
        cocos2d::ccColor3B getTableBorderColor() const noexcept;
        void setTableHeaderBgColor(cocos2d::ccColor3B color);
        cocos2d::ccColor3B getTableHeaderBgColor() const noexcept;

    private:
        struct Impl;
        std::shared_ptr<Impl> m_impl;
    };

    /// TextArea for static Markdown content. Supports the
    /// following features:
    ///  - Links
    ///  - Images (sprites & spritesheets)
    ///  - Headings
    ///  - Paragraphs
    ///  - Code blocks
    ///  - Code spans
    ///  - TextArea color tags (<cr>, <cy>, etc.)
    ///  - Custom hex code color tags (e.g. <c-123>, <c-123abc>, ...)
    ///  - Strikethrough
    ///  - Underline
    ///  - Bold & italic
    ///  - Horizontal rules
    ///  - Lists (ordered, numbered, task lists)
    ///  - Blockquotes
    ///  - Tables
    ///
    /// Note that links also have some special protocols.
    /// Use `user:<accountID>` to link to a GD
    /// account; `level:<id>` to link to a GD level and
    /// `mod:<id>` to link to another Geode mod.
    class GEODE_DLL MDTextArea : public cocos2d::CCLayer, public cocos2d::CCLabelProtocol {
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

    protected:
        MDTextArea();
        ~MDTextArea() override;

        bool init(std::string str, float maxWidth, MDConfig config);
        bool initClipped(std::string str, cocos2d::CCSize size, MDConfig config);

        void activateLink(utils::md::LinkKind kind, std::string href);

    public:

        /// Create a Markdown text area without scroll container.
        /// @param str String to render
        /// @param maxWidth Width of the textarea
        static MDTextArea* create(std::string str, float maxWidth);

        /// Create a Markdown text area without scroll container.
        /// @param str String to render
        /// @param maxWidth Width of the textarea
        /// @param config Custom configuration for the textarea
        static MDTextArea* create(std::string str, float maxWidth, MDConfig config);

        /// Create a Markdown text area inside a scroll container.
        /// @param str String to render
        /// @param size Size of the textarea
        static MDTextArea* create(std::string str, cocos2d::CCSize size);

        /// Create a Markdown text area inside a scroll container.
        /// @param str String to render
        /// @param size Size of the textarea
        /// @param config Custom configuration for the textarea
        static MDTextArea* create(std::string str, cocos2d::CCSize size, MDConfig config);

        /// Re-draws all contents of the textarea.
        void updateLabel();

        void setText(std::string text);
        ZStringView getText() const;

        void setString(char const* text) override;
        char const* getString() override;

        ScrollLayer* getScrollLayer() const;
        void scrollToTop();

        bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;

        void registerWithTouchDispatcher() override;
    };
}
