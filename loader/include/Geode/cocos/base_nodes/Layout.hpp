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

class GEODE_DLL AxisLayoutOptions : public LayoutOptions {
protected:
    std::optional<float> m_maxScale = 1.f;
    std::optional<float> m_length = std::nullopt;

public:
    static AxisLayoutOptions* create();

    std::optional<float> getMaxScale();
    std::optional<float> getLength();

    /**
     * Set the maximum scale this node can be if it's contained in an 
     * auto-scaled layout. Default is 1
     */
    AxisLayoutOptions* setMaxScale(float scale);

    /**
     * Disables auto-scaling for this node, even if the layout the node is 
     * contained in has it enabled. To enable auto-scaling, provide a max 
     * scale for the node in setMaxScale
    */
    AxisLayoutOptions* disableAutoScale();

    /**
     * Set an absolute length for this node. If nullopt, the length will be 
     * dynamically calculated based on content size
     */
    AxisLayoutOptions* setLength(std::optional<float> length);
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
    /**
     * Create a new AxisLayout. Note that this class is not automatically 
     * managed by default, so you must assign it to a CCNode or manually 
     * manage the memory yourself. See the chainable setters on AxisLayout for 
     * what options you can customize for the layout
     * @param axis The direction of the layout
     * @note For convenience, you can use the RowLayout and ColumnLayout 
     * classes, which are just thin wrappers over AxisLayout
     * @returns Created AxisLayout
     */
    static AxisLayout* create(Axis axis = Axis::Row);

    void apply(CCNode* on) override;

    Axis getAxis() const;
    AxisAlignment getAxisAlignment() const;
    AxisAlignment getCrossAxisAlignment() const;
    float getGap() const;
    bool getAxisReverse() const;
    bool getCrossAxisReverse() const;
    bool getAutoScale() const;
    bool getGrowCrossAxis() const;
    bool getCrossAxisOverflow() const;

    AxisLayout* setAxis(Axis axis);
    /**
     * Sets where to align the target node's children on the main axis (X for 
     * Row, Y for Column)
     */
    AxisLayout* setAxisAlignment(AxisAlignment align);
    /**
     * Sets where to align the target node's children on the cross-axis (Y for 
     * Row, X for Column)
     */
    AxisLayout* setCrossAxisAlignment(AxisAlignment align);
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
