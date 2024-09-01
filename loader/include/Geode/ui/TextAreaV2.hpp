#pragma once

#include <cocos2d.h>
#include "PaddingNode.hpp"

namespace geode {
    class GEODE_DLL TextAreaV2 : public PaddingNode {
    public:
        enum class WrappingMode {
            // Doesn't wrap the text and completely ignores bounds
            NoWrap,
            // Wraps the text on the last special character (spaces included) before the width is exceeded
            WordWrap,
            // Wraps the text on the last space before the width is exceeded
            SpaceWrap,
            // Wraps the text on the exact character that exceeds the width
            CutoffWrap
        };

        enum class Alignment {
            // Aligns the text to the left
            Left,
            // Aligns the text in the center
            Center,
            // Aligns the text to the right
            Right
        };

        struct Line {
            std::string text;
            std::string overflow;
            cocos2d::CCLabelBMFont* label;
            int lineNumber;
            float currentHeight;
            bool isLastLine;
        };

        static TextAreaV2* create(const std::string& text, const std::string& font, const float scale = 1, const float width = -1, const float height = -1, const bool deferUpdates = false);

        /**
        * Sets the font of the text area
        */
        void setFont(const std::string& font);
        /**
        * Gets the font of the text area
        */
        std::string getFont() const;
        /**
        * Sets the text of the text area
        */
        void setText(const std::string& text);
        /**
        * Gets the text of the text area
        */
        std::string getText() const;
        /**
        * Sets the color of the text in the text area
        */
        void setTextColor(const cocos2d::ccColor4B& color);
        /**
        * Gets the color of the text in the text area
        */
        cocos2d::ccColor4B getTextColor() const;
        /**
        * Sets horizontal the alignment of the text in the text area
        */
        void setAlignment(const Alignment alignment);
        /**
        * Gets the horizontal alignment of the text in the text area
        */
        Alignment getAlignment() const;
        /**
        * Sets the wrapping mode of the text in the text area
        */
        void setWrappingMode(const WrappingMode mode);
        /**
        * Gets the wrapping mode of the text in the text area
        */
        WrappingMode getWrappingMode() const;
        /**
        * Sets the maximum number of lines in the text area
        *
        * @note If this is set to a value smaller than 0, it will stop keeping track of the lines and leave either all the lines or as many as the max height allows
        */
        void setMaxLines(const int maxLines);
        /**
        * Gets the maximum number of lines in the text area
        */
        int getMaxLines() const;
        /**
        * Sets the scale of the text in the text area
        */
        void setTextScale(const float scale);
        /**
        * Gets the scale of the text in the text area
        */
        float getTextScale() const;
        /**
        * Sets the padding between lines in the text area
        */
        void setLinePadding(const float padding);
        /**
        * Gets the padding between lines in the text area
        */
        float getLinePadding() const;
        /**
        * Sets a minimum bound for the width of the text area
        *
        * @note If this is set to a value smaller than 0, it will stop rescaling the container and leave it at the max width
        */
        void setMinWidth(const float width);
        /**
        * Resets the minimum width to prevent it from rescaling the container
        */
        void resetMinWidth();
        /**
        * Gets the min width of the text area
        */
        float getMinWidth() const;
        /**
        * Sets the max width of the text area to define the wrapping bounds
        *
        * @note If this is set to a value smaller than 0, it will stop rescaling the container and leave it as big as the text or the min width
        */
        void setMaxWidth(const float width);
        /**
        * Resets the maximum width to prevent it from rescaling the container
        */
        void resetMaxWidth();
        /**
        * Gets the max width of the text area
        */
        float getMaxWidth() const;
        /**
        * Sets a minimum bound for the height of the text area
        *
        * @note If this is set to a value smaller than 0, it will stop rescaling the container and leave it at the max height
        */
        void setMinHeight(const float height);
        /**
        * Resets the minimum height to prevent it from rescaling the container
        */
        void resetMinHeight();
        /**
        * Gets the min height of the text area
        */
        float getMinHeight() const;
        /**
        * Sets the max height of the text area to define the maximum number of lines through a height limit
        *
        * @note If this is set to a value smaller than 0, it will stop rescaling the container and leave it as big as the lines or the min height
        */
        void setMaxHeight(const float height);
        /**
        * Resets the maximum height to prevent it from rescaling the container
        */
        void resetMaxHeight();
        /**
        * Gets the max height of the text area
        */
        float getMaxHeight() const;
        /**
        * Resets the minimum bounds for the width and height
        */
        void resetMinBounds();
        /**
        * Resets the maximum bounds for the width and height
        */
        void resetMaxBounds();
        /**
        * Sets whether the text should be hyphenated
        */
        void setHyphenate(const bool hyphenate);
        /**
        * Gets whether the text should be hyphenated
        */
        bool getHyphenate() const;
        /**
        * Sets whether the text area should show an ellipsis when the text overflows
        *
        * @note This will only show an ellipsis if the last line doesn't end with an ellipsis already
        */
        void setEllipsis(const bool ellipsis);
        /**
        * Gets whether the text area should show an ellipsis when the text overflows
        */
        bool getEllipsis() const;
        /**
        * Sets whether the text area should defer updates until the next frame
        *
        * @warning If set to true, all nodes won't immediately update and will only update on the next frame. This can cause side effects if not accounted for
        */
        void setDeferUpdates(const bool defer);
        /**
        * Gets whether the text area should defer updates until the next frame
        */
        bool getDeferUpdates() const;
        /**
        * Gets the height of a line including padding
        */
        float getLineHeight() const;
        /**
        * Gets the labels of the text area
        */
        std::vector<Line> getLines() const;
    protected:
        std::string m_font;
        std::string m_text;
        cocos2d::ccColor4B m_textColor;
        Alignment m_alignment;
        WrappingMode m_wrappingMode;
        int m_maxLines;
        float m_textScale;
        float m_linePadding;
        float m_minWidth;
        float m_maxWidth;
        float m_minHeight;
        float m_maxHeight;
        bool m_hyphenate;
        bool m_ellipsis;
        bool m_deferUpdates;
        bool m_deferred;
        std::vector<Line> m_lines;

        TextAreaV2(const std::string& text, const std::string& font, const float scale, const float width, const float height, const bool deferUpdates);
        bool init() override;
        void update();
        void updatePadding() override;
        void updateContainer(const float dt = 0);
        void updateLineAlignment(cocos2d::CCLabelBMFont* line);
        void addEllipsis(Line& line);
        bool isWidthOverflowing(const cocos2d::CCLabelBMFont* line);
        size_t getOverflowAmount(cocos2d::CCLabelBMFont* line, const size_t lineSize);
        Line createLine(const std::string& text, Line& previousLine);
        std::vector<Line> createLines();
        std::vector<Line> createNotWrap(const Line& reference);
        std::vector<Line> createCutoffWrap(const std::string& text, Line& reference);
        std::vector<Line> createDelimitedWrap(const std::string& text, Line& reference, const std::string& delimiters);
        std::vector<Line> wrapper(const std::string& text, Line& reference, const std::function<std::vector<Line>(Line& currentLine)>& onOverflow);
    };
}