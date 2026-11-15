#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/cocos/robtop/sprite_nodes/CCFontSprite.h>
#include <Geode/utils/function.hpp>
#include <Geode/ui/Label.hpp>
#include <memory>

namespace geode {
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
     *
     * Contact me on Discord (\@smjs) if you have any questions, suggestions or bugs.
    */

    class GEODE_DLL SimpleTextArea : public cocos2d::CCNode {
    public:
        static SimpleTextArea* create(std::string text, std::string font = "chatFont.fnt", float scale = 1.0f);
        static SimpleTextArea* create(std::string text, std::string font, float scale, float width);

        void setFont(std::string font);
        std::string getFont() const;

        void setColor(const cocos2d::ccColor4B& color);
        cocos2d::ccColor4B getColor() const;

        void setAlignment(cocos2d::CCTextAlignment alignment);
        cocos2d::CCTextAlignment getAlignment() const;

        /// Set the text wrapping mode.
        /// You can disable text wrapping with `setWrappingMode(NO_WRAP)`
        void setWrappingMode(WrappingMode mode);
        WrappingMode getWrappingMode() const;

        void setText(std::string text);
        std::string getText() const;

        /// Set the max amount of lines to be displayed. Anything else with get cut off.
        /// You can disable this feature by setting the value to '0'
        void setMaxLines(size_t maxLines);
        size_t getMaxLines() const;

        void setWidth(float width);
        float getWidth() const;

        void setScale(float scale) override;
        float getScale() override;

        /// Sets extra padding in between each line
        void setLinePadding(float padding);
        float getLinePadding() const;

        std::vector<geode::Label*> getLines() const;
        float getHeight() const;
        float getLineHeight() const;
    protected:
        SimpleTextArea();
        ~SimpleTextArea() override;

        struct Impl;
        std::unique_ptr<Impl> m_impl;

        bool init(std::string font, std::string text, float scale, float width, bool artificialWidth);
    };
}
