#pragma once

#include <Geode/cocos/include/ccTypes.h>
#include <Geode/cocos/sprite_nodes/CCSprite.h>
#include <Geode/cocos/include/CCProtocols.h>
#include <Geode/ui/SimpleAxisLayout.hpp>
#include <Geode/ui/NineSlice.hpp>
#include <Geode/ui/Label.hpp>

namespace geode {
    /*
     * ButtonSprite but cool
     */
    class GEODE_DLL ButtonBackgroundSprite : public cocos2d::CCNodeRGBA, public cocos2d::CCLabelProtocol {
        class Impl;
        std::unique_ptr<Impl> m_impl;
    protected:
        bool init(cocos2d::CCNode* node, std::optional<ZStringView> text, ZStringView font, ZStringView background = "GJ_button_01.png");

        ButtonBackgroundSprite();
        ~ButtonBackgroundSprite();

    public:
        enum class NodeAlignment : uint8_t {
            Top,
            Bottom,
            Left,
            Right
        };

        static ButtonBackgroundSprite* create(ZStringView text, ZStringView font, ZStringView background = "GJ_button_01.png");
        static ButtonBackgroundSprite* createWithNode(cocos2d::CCNode* node, ZStringView background = "GJ_button_01.png");
        static ButtonBackgroundSprite* createWithNodeLabel(cocos2d::CCNode* node, ZStringView text, ZStringView font, ZStringView background = "GJ_button_01.png");

        void setBG(ZStringView background, bool isFrame);
        NineSlice* getBG() const;

        /*
         * Sets the node on the button
         * If a label is also set, nodes scale will take priority
         * This function also sets max node scale to node->getScale()
         */
        void setNode(cocos2d::CCNode* node);
        /*
         * Remove the node on a button or do nothing if node is unset
         */
        void removeNode();
        cocos2d::CCNode* getNode() const;
        void setNodeAlignment(NodeAlignment alignment);
        NodeAlignment getNodeAlignment() const;

        void setString(char const* label) override;
        char const* getString() override;
        void setFont(ZStringView font);
        ZStringView getFont() const;
        /*
         * Remove the label on a button or do nothing if label is unset
         */
        void removeLabel();
        Label* getLabel() const;

        /*
         * Set (max) label scale
         */
        void setLabelScale(float scale);
        float getLabelScale() const;
        /*
         * Set (max) node scale
         */
        void setNodeScale(float scale);
        float getNodeScale() const;
        /*
         * Set label y offset relative to scale (default to 4.0f so its actually centered like in normal buttonsprite)
         */
        void setLabelOffset(float offset);
        float getLabelOffset() const;

        /*
         * Sets width of the button
         * The button is locked to this width and elements are scaled acordingly
         * Set to nullopt for width to be determined automatically
         */
        void setWidth(std::optional<float> width);
        std::optional<float> getWidth() const;
        void setMinWidth(float width);
        float getMinWidth() const;
        void setMaxWidth(float width);
        float getMaxWidth() const;
        /*
         * Sets height of the button
         * The button is locked to this height and elements are scaled acordingly
         * Set to nullopt for height to be determined automatically
         */
        void setHeight(std::optional<float> height);
        std::optional<float> getHeight() const;
        void setMinHeight(float height);
        float getMinHeight() const;
        void setMaxHeight(float height);
        float getMaxHeight() const;

        /**
         * Sets padding around edge and node/label
         */
        void setPadding(const Padding& padding);
        const Padding& getPadding() const;
        /**
         * Sets gap between node and label
         */
        void setGap(float gap);
        float getGap() const;
        /**
         * Like AxisLayout::setCrossLineAlignment
         */
        void setLineAlignment(CrossAxisAlignment alignment);
        CrossAxisAlignment getLineAlignment() const;

        void setColor(cocos2d::ccColor3B const& color) override;
        void setOpacity(GLubyte opacity) override;

        /**
         * Update content size and the layout node and label are in, called automatically in everything
         */
        void updateElements();
    };
}