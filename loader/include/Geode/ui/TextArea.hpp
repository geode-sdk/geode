#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/cocos/robtop/sprite_nodes/CCFontSprite.h>
#include <Geode/utils/function.hpp>
#include <Geode/ui/Label.hpp>
#include <memory>

namespace geode {
    class SimpleTextAreaImpl;

    enum WrappingMode {
        NO_WRAP,
        WORD_WRAP,
        SPACE_WRAP,
        CUTOFF_WRAP
    };

    /**
     * A class which provides a textarea with proper alignment and some extra features like:
     *
     * - Max lines
     * - Changing all aspects after creation
     * - Custom text alignment
     * - Configurable and automatic word wrapping
     * - Line padding
     * - Color tag support (for more/custom tags use `RichTextArea`)
     *
     * Contact me on Discord (\@smjs) if you have any questions, suggestions or bugs.
    */

    class GEODE_DLL SimpleTextArea : public cocos2d::CCNode {
    public:
        static SimpleTextArea* create(std::string text, std::string font = "chatFont.fnt", float scale = 1.0f);
        static SimpleTextArea* create(std::string text, std::string font, float scale, float width);

        void setFont(std::string font);
        std::string getFont();
        void setColor(const cocos2d::ccColor4B& color);
        cocos2d::ccColor4B getColor();
        void setAlignment(cocos2d::CCTextAlignment alignment);
        cocos2d::CCTextAlignment getAlignment();
        void setWrappingMode(WrappingMode mode);
        WrappingMode getWrappingMode();
        void setText(std::string text);
        virtual std::string getText() const;
        void setMaxLines(size_t maxLines);
        size_t getMaxLines();
        void setWidth(float width);
        float getWidth();
        void setScale(float scale) override;
        float getScale() override;
        void setLinePadding(float padding);
        float getLinePadding();
        std::vector<geode::Label*> getLines();
        float getHeight();
        float getLineHeight();

    protected:
        SimpleTextArea();
        ~SimpleTextArea() override;

        virtual std::unique_ptr<SimpleTextAreaImpl> createImpl();
        std::unique_ptr<SimpleTextAreaImpl> m_impl;

        bool init(std::string font, std::string text, float scale, float width, const bool artificialWidth);
    private:
        static SimpleTextArea* create(std::string font, std::string text, float scale, float width, const bool artificialWidth);
    };

    // abb2k and xblazegmd were here :)

    /**
     * A SimpleTextArea with rich text support
     *
     * You can add a custom tag with `addRichTag`:
     * @example ```cpp
     * richTextArea->addRichTag(
     *     "tagID",
     *     [](CCFontSprite* spr, std::string_view args) {
     *         // This function will manually modify every character
     *         // in the label. This class uses a combination of geode::Label and
     *         // CCLabelBMFont under the hood to make this possible
     *         spr->setFlipX(true);
     *     }
     * );
     * ```
     *
     * You can also use *any* node of your liking:
     * @example ```cpp
     * richTextArea->addRichTag(
     *     "tagID",
     *     [](std::string_view text, std::string_view args) {
     *         // If you want to add a button for example, you can
     *         // add one like so
     *         auto btn = Button::createWithLabel("Button!", "bigFont.fnt", []{
     *             log::info("My button was pressed!")
     *         });
     *         return btn;
     *     }
     * );
     * ```
     *
     * Tags can also take arguments with "-" or "=":
     * @example ```cpp
     * auto richTextArea = RichTextArea::create("Here's some <tagID-h>text!</tagID>");
     * richTextArea->addRichTag(
     *     "tagID",
     *     true, // This will make the tag *always* require an argument, anything else will throw an error
     *     [](std::string_view args) {
     *         // This function is meant to validate the argument's syntax
     *         // For this example we don't care so we just return true
     *         return true;
     *     },
     *     [](CCFontSprite* spr, std::string_view args) {
     *         if (args == "h") {
     *             spr->setFlipX(true);
     *         } else {
     *             spr->setFlipY(true);
     *         }
     *     }
     * );
     * ```
     */
    class GEODE_DLL RichTextArea : public SimpleTextArea {
    public:
        using ValidateArgs = geode::Function<bool(std::string_view)>;
        using CreateCallback = geode::Function<cocos2d::CCNode*(std::string_view, std::string_view)>;
        using CharIterator = geode::Function<void(cocos2d::CCFontSprite*, std::string_view)>;

        static RichTextArea* create(std::string text, std::string font = "chatFont.fnt", float scale = 1.f);
        static RichTextArea* create(std::string text, std::string font, float scale, float width);

        /**
         * Add a custom rich text tag
         *
         * @param id The ID of the tag
         * @param create Function to create a custom node for the tag
         * @param updateText Whether to update the rich text (you can call `updateRichText` later)
         */
        void addRichTag(std::string id, CreateCallback create, bool updateText = true);

        /**
         * Add a custom rich text tag
         *
         * @param id The ID of the tag
         * @param requireArgs Whether to require arguments for this tag
         * @param create Function to create a custom node for the tag
         * @param updateText Whether to update the rich text (you can call `updateRichText` later)
         */
        void addRichTag(std::string id, bool requireArgs, CreateCallback create, bool updateText = true);

        /**
         * Add a custom rich text tag
         *
         * @param id The ID of the tag
         * @param validateArgs Function to validate the arguments given to this tag
         * @param create Function to create a custom node for the tag
         * @param updateText Whether to update the rich text (you can call `updateRichText` later)
         */
        void addRichTag(std::string id, ValidateArgs validate, CreateCallback create, bool updateText = true);

        /**
         * Add a custom rich text tag
         *
         * @param id The ID of the tag
         * @param requireArgs Whether to require arguments for this tag
         * @param validateArgs Function to validate the arguments given to this tag
         * @param create Function to create a custom node for the tag
         * @param updateText Whether to update the rich text (you can call `updateRichText` later)
         */
        void addRichTag(std::string id, bool requireArgs, ValidateArgs validate, CreateCallback create, bool updateText = true);

        /**
         * Add a custom rich text tag
         *
         * @param id The ID of the tag
         * @param iterator Function to apply effects to each character in the tag
         * @param updateText Whether to update the rich text (you can call `updateRichText` later)
         */
        void addRichTag(std::string id, CharIterator iterator, bool updateText = true);

        /**
         * Add a custom rich text tag
         *
         * @param id The ID of the tag
         * @param requireArgs Whether to require arguments for this tag
         * @param iterator Function to apply effects to each character in the tag
         * @param updateText Whether to update the rich text (you can call `updateRichText` later)
         */
        void addRichTag(std::string id, bool requireArgs, CharIterator iterator, bool updateText = true);

        /**
         * Add a custom rich text tag
         *
         * @param id The ID of the tag
         * @param validateArgs Function to validate the arguments given to this tag
         * @param iterator Function to apply effects to each character in the tag
         * @param updateText Whether to update the rich text (you can call `updateRichText` later)
         */
        void addRichTag(std::string id, ValidateArgs validate, CharIterator iterator, bool updateText = true);

        /**
         * Add a custom rich text tag
         *
         * @param id The ID of the tag
         * @param requireArgs Whether to require arguments for this tag
         * @param validateArgs Function to validate the arguments given to this tag
         * @param iterator Function to apply effects to each character in the tag
         * @param updateText Whether to update the rich text (you can call `updateRichText` later)
         */
        void addRichTag(std::string id, bool requireArgs, ValidateArgs validate, CharIterator iterator, bool updateText = true);

        std::string getText() const override;

        void updateRichText();
    protected:
        RichTextArea() : SimpleTextArea() {}
        ~RichTextArea() = default;

        struct RichTag {
            std::string id;
            bool requireArgs;
            ValidateArgs validate;
            CreateCallback create;
            CharIterator iterator;
        };

        // I hate pImpl with inheritance
        class RichImpl;
        RichImpl* m_richImpl;
        std::unique_ptr<SimpleTextAreaImpl> createImpl() override;

        bool init(std::string font, std::string text, float scale, float width, const bool artificialWidth);
    private:
        static RichTextArea* create(std::string font, std::string text, float scale, float width, const bool artificialWidth);
    };
}
