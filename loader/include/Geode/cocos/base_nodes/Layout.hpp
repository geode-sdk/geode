#pragma once

#include "../include/ccMacros.h"
#include "../cocoa/CCAffineTransform.h"
#include "../cocoa/CCArray.h"
#include <Geode/platform/platform.hpp>
#include <optional>
#include <unordered_map>

NS_CC_BEGIN

class CCNode;

#pragma warning(push)
#pragma warning(disable: 4275)

/**
 * Layouts automatically handle the positioning of nodes. Use CCNode::setLayout 
 * to apply a layout to a node, and then use CCNode::updateLayout to apply 
 * the layout's positioning. Geode comes with a few default layouts like 
 * RowLayout, ColumnLayout, and GridLayout, but if you need a different kind 
 * of layout you can inherit from the Layout class.
 */
class GEODE_DLL Layout : public CCObject {
protected:
    static CCArray* getNodesToPosition(CCNode* forNode);

public:
    /**
     * Automatically apply the layout's positioning on a set of nodes
     * @param on Node to apply the layout on. Position's the node's children 
     * according to the layout. The content size of the node should be 
     * respected as a boundary the layout shouldn't overflow. The node may be 
     * rescaled to better fit its contents
     */
    virtual void apply(CCNode* on) = 0;

    virtual ~Layout() = default;
};

class GEODE_DLL LayoutOptions {
public:
    virtual ~LayoutOptions() = default;
};

/**
 * The direction of an AxisLayout
 */
enum class Axis {
    Row,
    Column,
};

/**
 * Specifies the alignment of something in an AxisLayout
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

constexpr float AXISLAYOUT_DEFAULT_MIN_SCALE = 0.65f;
constexpr int AXISLAYOUT_DEFAULT_PRIORITY = 0;

/**
 * Options for controlling the behaviour of individual nodes in an AxisLayout
 * @example
 * auto node = CCNode::create();
 * // this node will have 10 units of spacing between it and the next one
 * node->setLayoutOptions(
 *     AxisLayoutOptions::create()
 *         ->setNextGap(10.f)
 * );
 * someNodeWithALayout->addChild(node);
 */
class GEODE_DLL AxisLayoutOptions : public LayoutOptions {
protected:
    std::optional<bool> m_autoScale = std::nullopt;
    float m_maxScale = 1.f;
    float m_minScale = AXISLAYOUT_DEFAULT_MIN_SCALE;
    float m_relativeScale = 1.f;
    std::optional<float> m_length = std::nullopt;
    std::optional<float> m_nextGap = std::nullopt;
    std::optional<float> m_prevGap = std::nullopt;
    bool m_breakLine = false;
    bool m_sameLine = false;
    int m_scalePriority = AXISLAYOUT_DEFAULT_PRIORITY;

public:
    static AxisLayoutOptions* create();

    std::optional<bool> getAutoScale() const;
    float getMaxScale() const;
    float getMinScale() const;
    float getRelativeScale() const;
    std::optional<float> getLength() const;
    std::optional<float> getPrevGap() const;
    std::optional<float> getNextGap() const;
    bool getBreakLine() const;
    bool getSameLine() const;
    int getScalePriority() const;

    /**
     * Set the maximum scale this node can be if it's contained in an 
     * auto-scaled layout. Default is 1
     */
    AxisLayoutOptions* setMaxScale(float scale);

    /**
     * Set the minimum scale this node can be if it's contained in an 
     * auto-scaled layout. Default is AXISLAYOUT_DEFAULT_MIN_SCALE
     */
    AxisLayoutOptions* setMinScale(float scale);

    /**
     * Set the relative scale of this node compared to other nodes if it's 
     * contained in an auto-scaled layout. Default is 1
     */
    AxisLayoutOptions* setRelativeScale(float scale);

    /**
     * Set auto-scaling for this node, overriding the layout's auto-scale 
     * setting. If nullopt, the layout's auto-scale options will be used
    */
    AxisLayoutOptions* setAutoScale(std::optional<bool> enabled);

    /**
     * Set an absolute length for this node. If nullopt, the length will be 
     * dynamically calculated based on content size
     */
    AxisLayoutOptions* setLength(std::optional<float> length);

    /**
     * Override the default gap in the layout between this node and the 
     * previous one. If nullopt, the default gap of the layout will be used
     */
    AxisLayoutOptions* setPrevGap(std::optional<float> gap);

    /**
     * Override the default gap in the layout between this node and the next 
     * one. If nullopt, the default gap of the layout will be used
     */
    AxisLayoutOptions* setNextGap(std::optional<float> gap);

    /**
     * If enabled, the node will always cause a growable axis layout to break 
     * into a new line even if the current line could've fit the next node
     */
    AxisLayoutOptions* setBreakLine(bool enable);

    /**
     * If enabled, the node will be forced to be on the same line as the 
     * previous node even if doing this would overflow
     */
    AxisLayoutOptions* setSameLine(bool enable);

    /**
     * Set the scale priority of this node. Nodes with higher priority will be 
     * scaled down first before nodes with lower priority when an auto-scaled 
     * layout attempts to fit its contents. Default is 
     * AXISLAYOUT_DEFAULT_PRIORITY
     * @note For optimal performance, the priorities should all be close to 
     * each other with no gaps
     */
    AxisLayoutOptions* setScalePriority(int priority);
};

/**
 * A multi-purpose dynamic layout for arranging nodes along an axis. Can be 
 * used to arrange nodes in a single line, a grid, or a flex layout. The 
 * RowLayout and ColumnLayout classes function as simple thin wrappers over 
 * AxisLayout. The positioning of individual nodes in the layout can be 
 * further controlled using AxisLayoutOptions
 * @warning Calculating layouts can get increasingly expensive for large 
 * amounts of child nodes being fit into a small space - while this should 
 * never prove a real performance concern as most layouts only have a few 
 * hundred children at the very most, be aware that you probably shouldn't 
 * call CCNode::updateLayout every frame for a menu with thousands of children
 * @example
 * auto menu = CCMenu::create();
 * // The menu's children will be arranged horizontally, unless they overflow 
 * // the content size width in which case a new line will be inserted and 
 * // aligned to the left. The menu automatically will automatically grow in 
 * // height to fit all the rows
 * menu->setLayout(
 *     RowLayout::create()
 *         ->setGap(10.f)
 *         ->setGrowCrossAxis(true)
 *         ->setAxisAlignment(AxisAlignment::Start)
 * );
 * menu->setContentSize({ 200.f, 0.f });
 * menu->addChild(...);
 * menu->updateLayout();
 */
class GEODE_DLL AxisLayout : public Layout {
protected:
    Axis m_axis;
    AxisAlignment m_axisAlignment = AxisAlignment::Center;
    AxisAlignment m_crossAlignment = AxisAlignment::Center;
    AxisAlignment m_crossLineAlignment = AxisAlignment::Center;
    float m_gap = 5.f;
    bool m_autoScale = true;
    bool m_axisReverse = false;
    bool m_crossReverse = false;
    bool m_allowCrossAxisOverflow = true;
    bool m_growCrossAxis = false;

    struct Row;
    
    float minScaleForPrio(CCArray* nodes, int prio) const;
    float maxScaleForPrio(CCArray* nodes, int prio) const;
    bool shouldAutoScale(AxisLayoutOptions const* opts) const;
    bool canTryScalingDown(
        CCArray* nodes,
        int& prio, float& scale,
        float crossScaleDownFactor,
        std::pair<int, int> const& minMaxPrios
    ) const;
    float nextGap(AxisLayoutOptions const* now, AxisLayoutOptions const* next) const;
    Row* fitInRow(
        CCNode* on, CCArray* nodes,
        std::pair<int, int> const& minMaxPrios,
        bool doAutoScale,
        float scale, float squish, int prio
    ) const;
    void tryFitLayout(
        CCNode* on, CCArray* nodes,
        std::pair<int, int> const& minMaxPrios,
        bool doAutoScale,
        float scale, float squish, int prio,
        size_t depth
    ) const;

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
    AxisAlignment getCrossAxisLineAlignment() const;
    float getGap() const;
    bool getAxisReverse() const;
    bool getCrossAxisReverse() const;
    bool getAutoScale() const;
    bool getGrowCrossAxis() const;
    bool getCrossAxisOverflow() const;

    AxisLayout* setAxis(Axis axis);
    /**
     * Sets where to align the target node's children on the main axis (X-axis 
     * for Row, Y-axis for Column)
     */
    AxisLayout* setAxisAlignment(AxisAlignment align);
    /**
     * Sets where to align the target node's children on the cross-axis (Y-axis 
     * for Row, X-axis for Column)
     */
    AxisLayout* setCrossAxisAlignment(AxisAlignment align);
    /**
     * Sets where to align the target node's children on the cross-axis for 
     * each row (Y-axis for Row, X-axis for Column)
     */
    AxisLayout* setCrossAxisLineAlignment(AxisAlignment align);
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

#pragma warning(pop)

NS_CC_END
