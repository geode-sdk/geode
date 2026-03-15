#pragma once

#include "ScrollLayer.hpp"
#include "TextRenderer.hpp"

#include <Geode/binding/FLAlertLayerProtocol.hpp>

struct MDParser;
class CCScrollLayerExt;

namespace geode {
    /**
     * TextArea for static markdown content. Supports the
     * following features:
     *  - Links
     *  - Images (sprites & spritesheets)
     *  - Headings
     *  - Paragraphs
     *  - Code blocks
     *  - Code spans
     *  - TextArea color tags (&lt;cr&gt;, &lt;cy&gt;, etc.)
     *  - Custom hex code color tags (e.g. &lt;c-123&gt;, &lt;c-123abc&gt;, ...)
     *  - Strikethrough
     *  - Underline
     *  - Bold & italic
     *  - Horizontal rules
     *  - Lists
     *
     * Note that links also have some special protocols.
     * Use `user:<accountID>` to link to a GD
     * account; `level:<id>` to link to a GD level and
     * `mod:<id>` to link to another Geode mod.
     *
     * ## Images
     *
     * Images can be loaded from files or sprite frames:
     *  - `![alt text](image.png)` - Load from file
     *  - `![alt text](frame:spriteFrameName)` - Load from sprite frame
     *
     * Images support the following query parameters:
     *  - `?scale=X` - Scale the sprite by X (default: 1.0)
     *  - `?width=X` - Limit width to X pixels
     *  - `?height=X` - Limit height to X pixels
     *
     * Examples:
     *  - `![](frame:mySprite_001.png?scale=0.5)`
     *  - `![](frame:mySprite_001.png?width=100&height=50)`
     *  - `![](myImage.png?scale=2.0&width=200)`
     */
    class GEODE_DLL MDTextArea :
        public cocos2d::CCLayer,
        public cocos2d::CCLabelProtocol,
        public FLAlertLayerProtocol {
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
        /**
         * Converts single newlines to soft linebreaks.
         */
        static std::string translateNewlines(std::string const& str);

        bool init(std::string str, cocos2d::CCSize const& size);
    protected:
        MDTextArea();
        virtual ~MDTextArea();
    private:
        void onLink(CCObject*);
        void onGDProfile(CCObject*);
        void onGDLevel(CCObject*);
        void onGeodeMod(CCObject*);
        void FLAlert_Clicked(FLAlertLayer*, bool btn) override;

        friend struct ::MDParser;

    public:
        /**
         * Create a markdown text area. See class
         * documentation for details on supported
         * features & notes.
         * @param str String to render
         * @param size Size of the textarea
         */
        static MDTextArea* create(std::string str, cocos2d::CCSize const& size);

        /**
         * Create a markdown text area. See class
         * documentation for details on supported
         * features & notes.
         * @param str String to render
         * @param size Size of the textarea
         * @param compatibilityMode Enables functionality that may be useful for wrapping a generic alert, such as newline support
         */
        static MDTextArea* create(std::string str, cocos2d::CCSize const& size, bool compatibilityMode);

        /**
         * Update the label's content; call
         * sparingly as rendering may be slow
         */
        void updateLabel();

        void setString(char const* text) override;
        char const* getString() override;

        CCScrollLayerExt* getScrollLayer() const;
    };
}
