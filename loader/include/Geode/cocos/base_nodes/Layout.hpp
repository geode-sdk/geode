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

/**
 * Specifies the alignment of something
 */
enum class Alignment {
    Begin,
    Center,
    End,
};

/**
 * Simple layout for arranging nodes in a row (horizontal line)
 */
class GEODE_DLL RowLayout : public Layout {
protected:
    std::optional<Alignment> m_vAlignment = Alignment::Center;
    float m_gap;
    std::optional<float> m_maxAutoScale = std::nullopt;
    bool m_reverse = false;
    bool m_fitInside = false;

public:
    void apply(CCNode* on) override;

    /**
     * Create a new RowLayout. Note that this class is not automatically 
     * managed by default, so you must assign it to a CCNode or manually 
     * manage the memory yourself. See the chainable setters on RowLayout for 
     * what options you can customize for the layout
     * @returns Created RowLayout
     */
    static RowLayout* create();

    /**
     * Sets where to align nodes on the Y-axis. If nullopt, the 
     * nodes' Y-position will not be affected, and the height of the node this 
     * layout applies to isn't altered. If an alignment is given, the height 
     * of the node this layout applies to is shrunk to fit the height of the 
     * nodes and no more. Any nodes that don't fit inside this space are 
     * aligned based on the value
     * @param align Value
     * @returns The same RowLayout this was applied on
     */
    RowLayout* setVAlignment(std::optional<Alignment> align);
    /**
     * The spacing between the children of the node this layout applies to. 
     * Measured as the space between their edges, not centres
     */
    RowLayout* setGap(float gap);
    /**
     * Whether to reverse the direction of the children in this layout or not
     */
    RowLayout* setReverse(bool reverse);
    /**
     * If a value is provided, then the node this layout applies to may be 
     * automatically rescaled to fit its contents better. By default the value 
     * is nullopt, which means that the layout doesn't affect the node's scale 
     * in any way, and any nodes that might overflow will be squished using 
     * other methods. If the value is set, the layout assumes that the scaled 
     * content size of the target node is what the content should be fit 
     * inside of, and scales to fit that space. If the value is nullopt, the 
     * unscaled content size is used instead
     */
    RowLayout* setMaxAutoScale(std::optional<float> scale);
    /**
     * If true, the children of the node this layout is applied to will be 
     * contained entirely within the bounds of the node's content size. If 
     * false, the children's positions will be within the bounds, but they may 
     * visually overflow depending on their anchor point
     */
    RowLayout* setFitInside(bool fit);
};

/**
 * Simple layout for arranging nodes in a column (vertical line)
 */
class GEODE_DLL ColumnLayout : public Layout {
protected:
    std::optional<Alignment> m_hAlignment = Alignment::Center;
    float m_gap;
    std::optional<float> m_maxAutoScale = std::nullopt;
    bool m_reverse = false;
    bool m_fitInside = false;

public:
    void apply(CCNode* on) override;

    /**
     * Create a new ColumnLayout. Note that this class is not automatically 
     * managed by default, so you must assign it to a CCNode or manually 
     * manage the memory yourself. See the chainable setters on RowLayout for 
     * what options you can customize for the layout
     * @returns Created ColumnLayout
     */
    static ColumnLayout* create();

    /**
     * Sets where to align nodes on the X-axis. If nullopt, the 
     * nodes' X-position will not be affected, and the width of the node this 
     * layout applies to isn't altered. If an alignment is given, the width 
     * of the node this layout applies to is shrunk to fit the width of the 
     * nodes and no more. Any nodes that don't fit inside this space are 
     * aligned based on the value
     * @param align Value
     * @returns The same RowLayout this was applied on
     */
    ColumnLayout* setHAlignment(std::optional<Alignment> align);
    /**
     * The spacing between the children of the node this layout applies to. 
     * Measured as the space between their edges, not centres
     */
    ColumnLayout* setGap(float gap);
    /**
     * Whether to reverse the direction of the children in this layout or not
     */
    ColumnLayout* setReverse(bool reverse);
    /**
     * If a value is provided, then the node this layout applies to may be 
     * automatically rescaled to fit its contents better. By default the value 
     * is nullopt, which means that the layout doesn't affect the node's scale 
     * in any way, and any nodes that might overflow will be squished using 
     * other methods
     */
    ColumnLayout* setMaxAutoScale(std::optional<float> scale);
    /**
     * If true, the children of the node this layout is applied to will be 
     * contained entirely within the bounds of the node's content size. If 
     * false, the children's positions will be within the bounds, but they may 
     * visually overflow depending on their anchor point
     */
    ColumnLayout* setFitInside(bool fit);
};

/**
 * Grid direction; which direction the grid should add its next row to if the 
 * current row is full
 */
enum class GridDirection {
    // Downward
    Column,
    // Upward
    ReverseColumn,
    // Right
    Row,
    // Left
    ReverseRow,
};

/**
 * Grid alignment; same as normal Alignment but also features the "Stretch" 
 * option which will stretch the row out to be the same size as the others
*/
enum class GridAlignment {
    Begin,
    Center,
    Stretch,
    End,
};

class GEODE_DLL GridLayout : public Layout {
protected:
    GridDirection m_direction = GridDirection::Column;
    GridAlignment m_alignment = GridAlignment::Center;
    std::optional<size_t> m_rowSize;

public:
    void apply(CCNode* on) override;

    static GridLayout* create(
        std::optional<size_t> rowSize,
        GridAlignment alignment = GridAlignment::Center,
        GridDirection direction = GridDirection::Column
    );

    GridLayout* setDirection(GridDirection direction);
    GridLayout* setAlignment(GridAlignment alignment);
    GridLayout* setRowSize(std::optional<size_t> rowSize);
};

NS_CC_END
