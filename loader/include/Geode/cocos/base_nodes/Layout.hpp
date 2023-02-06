#pragma once

#include "../include/ccMacros.h"
#include "../cocoa/CCAffineTransform.h"
#include "../cocoa/CCArray.h"
#include <Geode/platform/platform.hpp>
#include <optional>
#include <unordered_map>

NS_CC_BEGIN

class CCNode;

/**
 * Layouts automatically handle the positioning of nodes. Use CCNode::setLayout 
 * to apply a layout to a node, and then use CCNode::updateLayout to apply 
 * the layout's positioning. Geode comes with a few default layouts like 
 * RowLayout, ColumnLayout, and GridLayout, but if you need a different kind 
 * of layout you can inherit from the Layout class.
 */
class GEODE_DLL Layout {
protected:
    static CCArray* getNodesToPosition(CCNode* forNode);

public:
    /**
     * Automatically apply the layout's positioning on a set of nodes
     * @param on Node to apply the layout on. Position's the node's children 
     * according to the layout. The content size of the node should be 
     * respected as a boundary the layout shouldn't overflow. The node may be 
     * rescaled to better fit its contents. Layouts should respect nodes that 
     * have their PositionHint marked as absolute
     */
    virtual void apply(CCNode* on) = 0;

    virtual ~Layout() = default;
};

class GEODE_DLL LayoutOptions {
public:
    virtual ~LayoutOptions() = default;
};

/**
 * Determines how a node should be positioned within its parent, if that 
 * parent has an automatically positioning layout
 */
enum class PositionHint {
    // The container can determine the best position 
    // for this node
    Default,
    // The container's layout should not affect the 
    // position of this node
    Absolute,
};

enum class Axis {
    Row,
    Column,
};

/**
 * Specifies the alignment of something
 */
enum class AxisAlignment {
    // Align items to the start
    // |ooo......|
    Start,
    // All items are centered
    // |...ooo...|
    Center,
    // Align items to the end
    // |......ooo|
    End,
    // Each item gets the same portion from the layout (disregards gap)
    // |.o..o..o.|
    Even,
};

/**
 * Layout for arranging nodes along an axis. Used to implement row, column, and 
 * grid layouts
 */
class GEODE_DLL AxisLayout : public Layout {
protected:
    Axis m_axis;
    AxisAlignment m_axisAlignment = AxisAlignment::Center;
    AxisAlignment m_crossAlignment = AxisAlignment::Center;
    float m_gap = 5.f;
    bool m_autoScale = true;
    bool m_axisReverse = false;
    bool m_crossReverse = false;
    bool m_allowCrossAxisOverflow = true;
    bool m_growCrossAxis = false;

    struct Row;

    Row* fitInRow(CCNode* on, CCArray* nodes, float scale, float squish) const;
    void tryFitLayout(CCNode* on, CCArray* nodes, float scale, float squish) const;

    AxisLayout(Axis);

public:
    void apply(CCNode* on) override;

    /**
     * Sets where to align the target node's children on the cross-axis (Y for 
     * Row, X for Column)
     */
    AxisLayout* setCrossAxisAlignment(AxisAlignment align);
    /**
     * Sets where to align the target node's children on the main axis (X for 
     * Row, Y for Column)
     */
    AxisLayout* setAxisAlignment(AxisAlignment align);
    /**
     * The spacing between the children of the node this layout applies to. 
     * Measured as the space between their edges, not centres. Does not apply 
     * on the main / cross axis if their alignment is AxisAlignment::Even
     */
    AxisLayout* setGap(float gap);
    /**
     * Whether to reverse the direction of the children in this layout or not
     */
    AxisLayout* setAxisReverse(bool reverse);
    /**
     * Whether to reverse the direction of the rows on the cross-axis or not
     */
    AxisLayout* setCrossAxisReverse(bool reverse);
    /**
     * If enabled, then the layout may scale the target's children if they are 
     * about to overflow. Assumes that all the childrens' intended scale is 1
     */
    AxisLayout* setAutoScale(bool enable);
    /**
     * If true, if the main axis overflows extra nodes will be placed on new 
     * rows/columns on the cross-axis
     */
    AxisLayout* setGrowCrossAxis(bool expand);
    /**
     * If true, the cross-axis content size of the target node will be 
     * automatically adjusted to fit the children
     */
    AxisLayout* setCrossAxisOverflow(bool allow);
};

/**
 * Simple layout for arranging nodes in a row (horizontal line)
 */
class GEODE_DLL RowLayout : public AxisLayout {
protected:
    RowLayout();

public:
    /**
     * Create a new RowLayout. Note that this class is not automatically 
     * managed by default, so you must assign it to a CCNode or manually 
     * manage the memory yourself. See the chainable setters on RowLayout for 
     * what options you can customize for the layout
     * @returns Created RowLayout
     */
    static RowLayout* create();
};

/**
 * Simple layout for arranging nodes in a column (vertical line)
 */
class GEODE_DLL ColumnLayout : public AxisLayout {
protected:
    ColumnLayout();

public:
    /**
     * Create a new ColumnLayout. Note that this class is not automatically 
     * managed by default, so you must assign it to a CCNode or manually 
     * manage the memory yourself. See the chainable setters on RowLayout for 
     * what options you can customize for the layout
     * @returns Created ColumnLayout
     */
    static ColumnLayout* create();
};

NS_CC_END
