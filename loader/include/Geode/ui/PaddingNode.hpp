#pragma once

#include <cocos2d.h>

namespace geode {
    class GEODE_DLL PaddingNode : public cocos2d::CCNode {
    public:
        enum class PaddingType {
            // The container will have its size adjusted to fit the padding
            Contained,
            // This node will have its size adjusted to fit the padding
            Uncontained
        };

        struct Padding {
            operator float() const;
            operator cocos2d::CCPoint() const;

            float top;
            float right;
            float bottom;
            float left;

            Padding();
            Padding(cocos2d::CCPoint padding);
            Padding(const float x, const float y);
            Padding(const float top, const float right, const float bottom, const float left);
        };

        static PaddingNode* create(cocos2d::CCNode* container, const Padding& padding = {}, const PaddingType type = PaddingType::Contained);

        /**
        * Sets the padding of the node
        */
        virtual void setPadding(const Padding& padding);
        /**
        * Gets the padding of the node
        */
        Padding getPadding() const;
        /**
        * Gets the average padding of the node
        */
        float getAveragePadding() const;
        /**
        * Gets the total padding of the node
        */
        float getTotalPadding() const;
        /**
        * Sets the padding on both the x-axis sides
        */
        virtual void setPaddingX(const float x);
        /**
        * Gets the average padding on the x-axis
        */
        float getPaddingX() const;
        /**
        * Gets the total padding on the x-axis
        */
        float getTotalPaddingX() const;
        /**
        * Sets the padding on both the y-axis sides
        */
        virtual void setPaddingY(const float y);
        /**
        * Gets the average padding on the y-axis
        */
        float getPaddingY() const;
        /**
        * Gets the total padding on the y-axis
        */
        float getTotalPaddingY() const;
        /**
        * Sets the padding on the top side
        */
        virtual void setPaddingTop(const float top);
        /**
        * Gets the padding on the top side
        */
        float getPaddingTop() const;
        /**
        * Sets the padding on the right side
        */
        virtual void setPaddingRight(const float right);
        /**
        * Gets the padding on the right side
        */
        float getPaddingRight() const;
        /**
        * Sets the padding on the bottom side
        */
        virtual void setPaddingBottom(const float bottom);
        /**
        * Gets the padding on the bottom side
        */
        float getPaddingBottom() const;
        /**
        * Sets the padding on the left side
        */
        virtual void setPaddingLeft(const float left);
        /**
        * Gets the padding on the left side
        */
        float getPaddingLeft() const;
        /**
        * Sets the padding type of the node
        */
        virtual void setPaddingType(const PaddingType type);
        /**
        * Gets the padding type of the node
        */
        PaddingType getPaddingType() const;
        /**
        * Sets the container of the node
        */
        virtual void setContainer(cocos2d::CCNode* content);
        /**
        * Gets the container of the node
        */
        cocos2d::CCNode* getContainer() const;
        /**
        * Gets the container size with the padding added
        */
        cocos2d::CCSize getPaddedContainerSize() const;
    protected:
        Padding m_padding;
        cocos2d::CCNode* m_container;
        PaddingType m_type;

        PaddingNode(cocos2d::CCNode* container, PaddingNode::Padding padding  = {}, const PaddingType type = PaddingType::Contained);
        bool init() override;
        virtual void updatePadding();
    };
}