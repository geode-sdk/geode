#pragma once

#include <cocos2d.h>

namespace geode {
    enum class TextAlignment {
        Begin,
        Center,
        End,
    };

    enum class TextCapitalization {
        Normal,
        AllUpper,
        AllLower,
    };

    // enum only as these are flags
    enum TextStyle {
        TextStyleRegular = 0b0,
        TextStyleBold    = 0b1,
        TextStyleItalic  = 0b10,
    };

    // enum only as these are flags
    enum TextDecoration {
        TextDecorationNone         = 0b0,
        TextDecorationUnderline    = 0b1,
        TextDecorationStrikethrough= 0b10,
    };

    class TextDecorationWrapper;
    class TextLinkedButtonWrapper;

    /**
     * Utility class for creating rich text content. 
     * Use to incrementally render strings, and push 
     * variables to modify the renderer's state. Use 
     * `begin` to start rendering to a target and 
     * `end` to finish rendering.
     * 
     * Works for any type of label, although relies 
     * heavily on content sizes for labels and nodes.
     * 
     * Not too well-performant and the rendering is 
     * done linearly without so this is not suitable 
     * for dynamic content. For something like a 
     * static rich text -area though this can prove 
     * useful. Used in MDTextArea.
     */
    class GEODE_DLL TextRenderer : public cocos2d::CCObject {
    public:
        /**
         * Represents a label. As CCLabelBMFont and 
         * CCLabelTTF have different inheritance 
         * structures, this class can handle either 
         * one universally. All relevant vtables are 
         * stored in-class to avoid needing to 
         * `dynamic_cast` everything. This way of 
         * storing vtables also means that anything 
         * which satisfies these 3 vtables can be used, 
         * even if its true UX representation is 
         * actually not a label.
         */
        struct Label {
            /**
             * Label's CCNode vtable
             */
            cocos2d::CCNode* m_node;
            /**
             * Label's CCLabelProtocol vtable
             */
            cocos2d::CCLabelProtocol* m_labelProtocol;
            /**
             * Label's CCRGBAProtocol vtable
             */
            cocos2d::CCRGBAProtocol* m_rgbaProtocol;
            /**
             * Line height. If 0, the renderer will dynamically 
             * calculate line height based on content size.
             */
            float m_lineHeight;

            explicit inline Label() {
                m_node = nullptr;
                m_labelProtocol = nullptr;
                m_rgbaProtocol = nullptr;
                m_lineHeight = .0f;
            }

            template<class T>
            Label(T* label, float lineHeight = .0f) {
                static_assert(std::is_base_of_v<cocos2d::CCNode, T>, "Label must inherit from CCNode!");
                static_assert(std::is_base_of_v<cocos2d::CCLabelProtocol, T>, "Label must inherit from CCLabelProtocol!");
                static_assert(std::is_base_of_v<cocos2d::CCRGBAProtocol, T>, "Label must inherit from CCRGBAProtocol!");
                m_node = label;
                m_labelProtocol = label;
                m_rgbaProtocol = label;
                if (lineHeight) {
                    m_lineHeight = lineHeight;
                } else {
                    if constexpr (std::is_same_v<cocos2d::CCLabelBMFont, T>) {
                        m_lineHeight = label->getConfiguration()->m_nCommonHeight / cocos2d::CC_CONTENT_SCALE_FACTOR();
                    }
                }
            }
        };
        /**
         * Label generator function. The `int` parameter 
         * represents the current text style flags. Use 
         * to distinguish between bold, italic and 
         * regular text.
         */
        using Font = std::function<Label(int)>;

    protected:
        cocos2d::CCPoint m_origin = cocos2d::CCPointZero;
        cocos2d::CCSize m_size = cocos2d::CCSizeZero;
        cocos2d::CCPoint m_cursor = cocos2d::CCPointZero;
        cocos2d::CCNode* m_target = nullptr;
        std::vector<Font> m_fontStack;
        std::vector<float> m_scaleStack;
        std::vector<int> m_styleStack;
        std::vector<cocos2d::ccColor3B> m_colorStack;
        std::vector<GLubyte> m_opacityStack;
        std::vector<int> m_decorationStack;
        std::vector<TextCapitalization> m_capsStack;
        std::vector<Label> m_lastRendered;
        std::vector<float> m_indentationStack;
        std::vector<float> m_wrapOffsetStack;
        std::vector<TextAlignment> m_hAlignmentStack;
        std::vector<TextAlignment> m_vAlignmentStack;
        std::vector<cocos2d::CCNode*> m_renderedLine;
        cocos2d::CCNode* m_lastRenderedNode = nullptr;

        bool init();

        Label addWrappers(
            Label const& label,
            bool isButton,
            cocos2d::CCObject* target,
            cocos2d::SEL_MenuHandler callback
        );
        bool render(std::string const& word, cocos2d::CCNode* to, cocos2d::CCLabelProtocol* label);
        float adjustLineAlignment();

    public:
        /**
         * Create a TextRenderer
         * @returns Created TextRenderer
         */
        static TextRenderer* create();
        virtual ~TextRenderer();

        /**
         * Initialize renderer
         * @param target Target node to render to. If nullptr, 
         * a new CCNode will be created.
         * @param pos Position to render to
         * @param size Size of the render area. Needed for 
         * text wrapping & alignment
         */
        void begin(
            cocos2d::CCNode* target,
            cocos2d::CCPoint const& pos = cocos2d::CCPointZero,
            cocos2d::CCSize const& size = cocos2d::CCSizeZero
        );
        /**
         * Finish rendering and clean up renderer
         * @param fitToContent Resize the target's content 
         * size to match the rendered content
         * @param horizontalAlign Horizontal alignment of 
         * the rendered text
         * @param verticalAlign Vertical alignment of 
         * the rendered text
         * @returns Target that was rendered onto
         */
        cocos2d::CCNode* end(
            bool fitToContent = true,
            TextAlignment horizontalAlign = TextAlignment::Begin,
            TextAlignment verticalAlign = TextAlignment::Begin
        );

        /**
         * Render a string with specific settings, bypassing 
         * current variable stacks.
         * @param str String to render
         * @param font Font function to use
         * @param scale Scale of label
         * @param color Label color
         * @param opacity Label opacity
         * @param style Label style (TextStyle enum)
         * @param deco Label decorations (TextDecoration enum)
         * @param caps String capitalization
         * @param addToTarget Whether to add the created label(s) 
         * onto the target
         * @param isButton If the label should be created as an 
         * interactive linked button
         * @param buttonTarget Target for the label if isButton is 
         * true, defaults to current renderer target
         * @param callback Callback for the label if isButton is 
         * true
         * @returns Vector of rendered labels. The label may be 
         * split on multiple lines if it exceeds bounds
         */
        std::vector<Label> renderStringEx(
            std::string const& str,
            Font font,
            float scale,
            cocos2d::ccColor3B color = { 255, 255, 255 },
            GLubyte opacity = 255,
            int style = TextStyleRegular,
            int deco = TextDecorationNone,
            TextCapitalization caps = TextCapitalization::Normal,
            bool addToTarget = true,
            bool isButton = false,
            cocos2d::CCObject* buttonTarget = nullptr,
            cocos2d::SEL_MenuHandler callback = nullptr
        );
        /**
         * Render a string to target. Uses current variable stacks 
         * for styling and parameters
         * @param str String to render
         * @returns Vector of rendered labels. The label may be 
         * split on multiple lines if it exceeds bounds
         */
        std::vector<Label> renderString(std::string const& str);
        /**
         * Render a string to target as a button. Note that the 
         * target should be a CCMenu for the button to do 
         * anything. Uses current variable stacks  for styling 
         * and parameters
         * @param str String to render
         * @param buttonTarget Target for the label if isButton is 
         * true, defaults to current renderer target
         * @param callback Callback for the label if isButton is 
         * true
         * @returns Vector of rendered labels. The label may be 
         * split on multiple lines if it exceeds bounds
         */
        std::vector<Label> renderStringInteractive(
            std::string const& str,
            cocos2d::CCObject* buttonTarget,
            cocos2d::SEL_MenuHandler callback
        );
        /**
         * Render a node to the current target, use for adding 
         * images & other content in the middle of text
         * @param node Node to render
         * @returns Rendered node
         */
        cocos2d::CCNode* renderNode(cocos2d::CCNode* node);
        /**
         * Start next line
         * @param y Y offset amount from previous line. If 0, 
         * will dynamically figure out based on content size
         */
        void breakLine(float y = .0f);

        /**
         * Helper for pushing a CCLabelBMFont. Make 
         * sure the const char* outlives the renderer.
         */
        void pushBMFont(const char* bmFont);
        void pushFont(Font const& font);
        void popFont();
        Font getCurrentFont() const;

        void pushScale(float scale);
        void popScale();
        float getCurrentScale() const;

        void pushStyleFlags(int style);
        void popStyleFlags();
        int getCurrentStyle() const;

        void pushColor(cocos2d::ccColor3B const& color);
        void popColor();
        cocos2d::ccColor3B getCurrentColor() const;

        void pushOpacity(GLubyte opacity);
        void popOpacity();
        GLubyte getCurrentOpacity() const;

        void pushDecoFlags(int deco);
        void popDecoFlags();
        int getCurrentDeco() const;

        void pushCaps(TextCapitalization caps);
        void popCaps();
        TextCapitalization getCurrentCaps() const;

        void pushIndent(float indent);
        void popIndent();
        float getCurrentIndent() const;

        void pushWrapOffset(float wrapOffset);
        void popWrapOffset();
        float getCurrentWrapOffset() const;

        void pushVerticalAlign(TextAlignment align);
        void popVerticalAlign();
        TextAlignment getCurrentVerticalAlign() const;

        void pushHorizontalAlign(TextAlignment align);
        void popHorizontalAlign();
        TextAlignment getCurrentHorizontalAlign() const;

        void moveCursor(cocos2d::CCPoint const& pos);
        cocos2d::CCPoint getCursorPos();
    };

    /**
     * Wrapper node for adding decorations (strikethrough, 
     * underline) to an arbitary label. Is not agnostic of 
     * font and as such will always render simple lines
     */
    class TextDecorationWrapper : public cocos2d::CCNodeRGBA, public cocos2d::CCLabelProtocol {
    protected:
        int m_deco;
        TextRenderer::Label m_label;

        bool init(
            TextRenderer::Label const& label,
            int decoration,
            cocos2d::ccColor3B const& color,
            GLubyte opacity
        );

        void draw() override;

    public:
        static TextDecorationWrapper* create(
            TextRenderer::Label const& label,
            int decoration,
            cocos2d::ccColor3B const& color,
            GLubyte opacity
        );
        static TextDecorationWrapper* wrap(
            TextRenderer::Label const& label,
            int decoration,
            cocos2d::ccColor3B const& color,
            GLubyte opacity
        );

        void setColor(cocos2d::ccColor3B const& color) override;
        void setOpacity(GLubyte opacity) override;
        void updateDisplayedColor(cocos2d::ccColor3B const& color) override;
        void updateDisplayedOpacity(GLubyte opacity) override;

        void setString(const char* text) override;
        const char* getString() override;
    };

    /**
     * Wrapper node for making a label clickable. 
     * Note that this should always be the top 
     * wrapper above all other wrappers
     */
    class TextLinkedButtonWrapper :
        public cocos2d::CCMenuItemSprite,
        public cocos2d::CCLabelProtocol
    {
    protected:
        TextRenderer::Label m_label;
        GLubyte m_opacity;
        cocos2d::ccColor3B m_color;
        std::vector<TextLinkedButtonWrapper*> m_linked;

        bool init(
            TextRenderer::Label const& label,
            cocos2d::CCObject* target,
            cocos2d::SEL_MenuHandler handler
        );
    
    public:
        static TextLinkedButtonWrapper* create(
            TextRenderer::Label const& label,
            cocos2d::CCObject* target,
            cocos2d::SEL_MenuHandler handler
        );
        static TextLinkedButtonWrapper* wrap(
            TextRenderer::Label const& label,
            cocos2d::CCObject* target,
            cocos2d::SEL_MenuHandler handler
        );

        void link(TextLinkedButtonWrapper* other);

        void selectedWithoutPropagation(bool selected);
        void selected() override;
        void unselected() override;

        void setColor(cocos2d::ccColor3B const& color) override;
        void setOpacity(GLubyte opacity) override;
        void updateDisplayedColor(cocos2d::ccColor3B const& color) override;
        void updateDisplayedOpacity(GLubyte opacity) override;

        void setString(const char* text) override;
        const char* getString() override;
    };
}
