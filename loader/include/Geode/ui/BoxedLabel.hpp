#pragma once

#include <Geode/ui/Label.hpp>
#include <Geode/ui/NineSlice.hpp>

namespace geode {
    class GEODE_DLL BoxedLabel final : public cocos2d::CCNode {
        class Impl;
        std::unique_ptr<Impl> m_impl;

    protected:
        BoxedLabel();
        ~BoxedLabel();

        void resize();

        bool init(std::string text, ZStringView font, float width);

    public:
        /// Create a label visually encased by a NineSlice with a width limit
        /// @param text Text to display on the label
        /// @param font Name of the label's font `.fnt` file
        /// @param width Text label width limit, set to 0 for dynamic width
        static BoxedLabel* create(std::string text, ZStringView font, float width = .0f);

        /// Change the label text
        /// @param text Text to display on the label
        void setText(std::string text);

        /// Change the label text
        /// @param font Name of the label's font `.fnt` file
        void setFont(ZStringView font);

        /// Change the scale of the text label and re-size the box with it
        /// @param width Scale for the label
        void setTextScale(float scale);

        /// Change the label's text alignment
        /// @param alignment Direction the label expands from
        void setTextAlignment(Label::Alignment alignment);

        /// Set the maximum width of the label
        void setMaxLabelWidth(float width);

        /// Change the padding between the edges of the box and the text label
        void setPadding(float padding);

        /// Change the color of the NineSlice surrounding the text label
        void setBGColor(cocos2d::ccColor3B const& color);

        /// Change the opacity of the NineSlice surrounding the text label
        void setBGOpacity(GLubyte opacity);

        /// Change the scale multiplier of the NineSlice surrounding the text label
        void setBGScale(float scale);

        /// Lock the box at the maximum width if set, instead of dynamically expanding with the label
        void lockBGWidth(bool lock);

        float getMaxLabelWidth() const noexcept;
        float getPadding() const noexcept;
        bool isBGWidthLocked() const noexcept;

        Label* getLabelNode() const noexcept;
        NineSlice* getBGSprite() const noexcept;
    };
};