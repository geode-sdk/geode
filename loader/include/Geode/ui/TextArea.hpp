#pragma once

#include <Geode/DefaultInclude.hpp>
#include <cocos2d.h>

namespace geode {
    /**
     * A class which provides a textarea with proper alignment and some extra features like:
     * 
     * - Max lines
     * - Changing all aspects after creation
     * - Custom text alignment
     * - Automatic line wrapping and cutoff
     * - Line padding
     * 
     * Contact me on Discord (\@smjs) if you have any questions, suggestions or bugs.
    */
    class GEODE_DLL SimpleTextArea : public cocos2d::CCNode {
        cocos2d::CCMenu* m_container;
        std::string m_font;
        std::string m_text;
        std::vector<cocos2d::CCLabelBMFont*> m_lines;
        cocos2d::CCTextAlignment m_alignment;
        size_t m_maxLines;
        float m_scale;
        float m_lineHeight;
        float m_linePadding;
        bool m_artificialWidth;
    public:
        static SimpleTextArea* create(const std::string& font, const std::string& text, const float scale);
        static SimpleTextArea* create(const std::string& font, const std::string& text, const float scale, const float width);

        void setFont(const std::string& font);
        std::string getFont();
        void setAlignment(const cocos2d::CCTextAlignment alignment);
        cocos2d::CCTextAlignment getAlignment();
        void setText(const std::string& text);
        std::string getText();
        void setMaxLines(const size_t maxLines);
        size_t getMaxLines();
        void setWidth(const float width);
        float getWidth();
        void setScale(const float scale);
        float getScale();
        void setLinePadding(const float padding);
        float getLinePadding();
        std::vector<cocos2d::CCLabelBMFont*> getLines();
        float getHeight();
        float getLineHeight();
    private:
        static SimpleTextArea* create(const std::string& font, const std::string& text, const float scale, const float width, const bool artificialWidth);

        SimpleTextArea(const std::string& font, const std::string& text, const float scale, const float width, const bool artificialWidth);
        cocos2d::CCLabelBMFont* createLabel(const std::string& text, const float top);
        cocos2d::CCLabelBMFont* moveOverflow(cocos2d::CCLabelBMFont* line, const char c, const float top);
        float calculateOffset(cocos2d::CCLabelBMFont* label);
        void updateLines();
        void updateContents();
    };
}