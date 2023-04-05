#pragma once

#include "CCNode.h"

NS_CC_BEGIN

#pragma warning(push)
#pragma warning(disable: 4275)

/**
 * A node for controlling spacing in Layouts. When a Layout is applied, if 
 * space is left over, the remaining space is divided among all SpacerNodes in 
 * the Layout. The space each node gets is the proprotion between its growth 
 * factor and the sum of all the SpacerNodes' growth factors in the Layout
 * @example
 * node->addChild(SpacerNode::create(1));
 * node->addChild(SpacerNode::create(2));
 * node->addChild(SpacerNode::create(1));
 * node->updateLayout();
 * // Total SpacerNode growth sum is 1 + 2 + 1 = 4
 * // So s1 and s3 get 1/4 of the remaining space and s2 gets 2/4
 * @note If you want to specify a minimum width for a SpacerNode, add 
 * AxisLayoutOptions for it and use setLength
 */
class GEODE_DLL SpacerNode : public CCNode {
protected:
    size_t m_grow;

    bool init(size_t grow);

public:
    /**
     * Create a new spacer node. When the layout is applied, 
     * if there is space left over the remaining space is distributed among 
     * all spacer nodes in proportion to the sum of all the spacers' grow 
     * factors (akin to CSS flew grow)
     * @param grow The grow factor for this node. Default is 1
    */
    static SpacerNode* create(size_t grow = 1);

    /**
     * Set the grow factor for this spacer node. When the layout is applied, 
     * if there is space left over the remaining space is distributed among 
     * all spacer nodes in proportion to the sum of all the spacers' grow 
     * factors (akin to CSS flew grow)
     * @param grow The new grow factor for this node. Default is 1
     * @note Make sure to call updateLayout on the spacer's parent afterwards
    */
    void setGrow(size_t grow);

    /**
     * Get the grow factor for this spacer node
     */
    size_t getGrow() const;
};

#pragma warning(pop)

NS_CC_END
