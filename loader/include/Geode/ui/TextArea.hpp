#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/function.hpp>
#include <memory>
#include <cocos2d.h>

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
        std::string getFont();
        void setColor(const cocos2d::ccColor4B& color);
        cocos2d::ccColor4B getColor();
        void setAlignment(cocos2d::CCTextAlignment alignment);
        cocos2d::CCTextAlignment getAlignment();
        void setWrappingMode(WrappingMode mode);
        WrappingMode getWrappingMode();
        virtual void setText(std::string text);
        std::string getText();
        void setMaxLines(size_t maxLines);
        size_t getMaxLines();
        void setWidth(float width);
        float getWidth();
        void setScale(float scale) override;
        float getScale() override;
        void setLinePadding(float padding);
        float getLinePadding();
        std::vector<cocos2d::CCLabelBMFont*> getLines();
        float getHeight();
        float getLineHeight();

        class Impl {
        public:
            bool m_shouldUpdate = false;
            bool m_artificialWidth = false;
            cocos2d::CCMenu* m_container = nullptr;
            std::string m_font;
            std::string m_text;
            std::vector<cocos2d::CCLabelBMFont*> m_lines;
            cocos2d::ccColor4B m_color = { 0xFF, 0xFF, 0xFF, 0xFF };
            cocos2d::CCTextAlignment m_alignment = cocos2d::kCCTextAlignmentLeft;
            WrappingMode m_wrappingMode = WORD_WRAP;
            size_t m_maxLines = 0;
            float m_scale = 1.f;
            float m_lineHeight = 0.f;
            float m_linePadding = 0.f;

            SimpleTextArea* m_self = nullptr;

            Impl(SimpleTextArea* self) : m_self(self) {}

            cocos2d::CCLabelBMFont* createLabel(char const* text, float top);
            float calculateOffset(cocos2d::CCLabelBMFont* label);
            virtual void charIteration(geode::FunctionRef<cocos2d::CCLabelBMFont*(cocos2d::CCLabelBMFont* line, char c, float top)> overflowHandling);
            void updateLinesNoWrap();
            void updateLinesWordWrap(bool spaceWrap);
            void updateLinesCutoffWrap();
            void updateContainer();
        };
    protected:
        SimpleTextArea();
        ~SimpleTextArea() override;

        bool init(std::string font, std::string text, float scale, float width, const bool artificialWidth);

        std::unique_ptr<Impl> m_impl;
    
    private:
        static SimpleTextArea* create(std::string font, std::string text, float scale, float width, const bool artificialWidth);
    };
}
