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
     * - Color tag support (for custom tags use `RichTextArea`)
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
     */
    class GEODE_DLL RichTextArea : public SimpleTextArea {
    public:
        using ValidateArgs = geode::Function<bool(std::string_view)>;
        using CreateCallback = geode::Function<cocos2d::CCNode*(std::string_view, std::string_view)>;
        using CharIterator = geode::Function<void(cocos2d::CCFontSprite*, std::string_view)>;

        static RichTextArea* create(std::string text, std::string font = "chatFont.fnt", float scale = 1.f);
        static RichTextArea* create(std::string text, std::string font, float scale, float width);

        /**
         * Add a custom rich text key
         *
         * @param id The ID of the key
         * @param create Function to create a custom node for the key
         * @param updateText Whether to update the rich text (you can call `updateRichText` later)
         */
        void addRichKey(std::string id, CreateCallback create, bool updateText = true);

        /**
         * Add a custom rich text key
         *
         * @param id The ID of the key
         * @param requireArgs Whether to require arguments for this key
         * @param create Function to create a custom node for the key
         * @param updateText Whether to update the rich text (you can call `updateRichText` later)
         */
        void addRichKey(std::string id, bool requireArgs, CreateCallback create, bool updateText = true);

        /**
         * Add a custom rich text key
         *
         * @param id The ID of the key
         * @param validateArgs Function to validate the arguments given to this key
         * @param create Function to create a custom node for the key
         * @param updateText Whether to update the rich text (you can call `updateRichText` later)
         */
        void addRichKey(std::string id, ValidateArgs validate, CreateCallback create, bool updateText = true);

        /**
         * Add a custom rich text key
         *
         * @param id The ID of the key
         * @param requireArgs Whether to require arguments for this key
         * @param validateArgs Function to validate the arguments given to this key
         * @param create Function to create a custom node for the key
         * @param updateText Whether to update the rich text (you can call `updateRichText` later)
         */
        void addRichKey(std::string id, bool requireArgs, ValidateArgs validate, CreateCallback create, bool updateText = true);

        /**
         * Add a custom rich text key
         *
         * @param id The ID of the key
         * @param iterator Function to apply effects to each character in the key
         * @param updateText Whether to update the rich text (you can call `updateRichText` later)
         */
        void addRichKey(std::string id, CharIterator iterator, bool updateText = true);

        /**
         * Add a custom rich text key
         *
         * @param id The ID of the key
         * @param requireArgs Whether to require arguments for this key
         * @param iterator Function to apply effects to each character in the key
         * @param updateText Whether to update the rich text (you can call `updateRichText` later)
         */
        void addRichKey(std::string id, bool requireArgs, CharIterator iterator, bool updateText = true);

        /**
         * Add a custom rich text key
         *
         * @param id The ID of the key
         * @param validateArgs Function to validate the arguments given to this key
         * @param iterator Function to apply effects to each character in the key
         * @param updateText Whether to update the rich text (you can call `updateRichText` later)
         */
        void addRichKey(std::string id, ValidateArgs validate, CharIterator iterator, bool updateText = true);

        /**
         * Add a custom rich text key
         *
         * @param id The ID of the key
         * @param requireArgs Whether to require arguments for this key
         * @param validateArgs Function to validate the arguments given to this key
         * @param iterator Function to apply effects to each character in the key
         * @param updateText Whether to update the rich text (you can call `updateRichText` later)
         */
        void addRichKey(std::string id, bool requireArgs, ValidateArgs validate, CharIterator iterator, bool updateText = true);

        std::string getText() const override;

        void updateRichText();
    protected:
        RichTextArea() : SimpleTextArea() {}
        ~RichTextArea() = default;

        struct RichKey {
            std::string id;
            bool requireArgs;
            ValidateArgs validate;
            CreateCallback create;
            CharIterator iterator;
        };

        class RichImpl;
        RichImpl* m_richImpl;
        std::unique_ptr<SimpleTextAreaImpl> createImpl() override;

        bool init(std::string font, std::string text, float scale, float width, const bool artificialWidth);
    private:
        static RichTextArea* create(std::string font, std::string text, float scale, float width, const bool artificialWidth);
    };
}
