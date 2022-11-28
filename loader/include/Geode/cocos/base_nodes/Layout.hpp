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
class Layout {
public:
    /**
     * Automatically apply the layout's positioning on a set of nodes
     * @param nodes Nodes to position 
     * @param availableSize Give hints to the layout about how much space is 
     * available. Note that the layout may still overflow
     */
    virtual void apply(CCArray* nodes, CCSize const& availableSize) = 0;
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
    Alignment m_alignment = Alignment::Center;
    std::optional<float> m_alignVertically;
    float m_gap;

public:
    void apply(CCArray* nodes, CCSize const& availableSize) override;

    /**
     * Create a new RowLayout. Note that this class is not automatically 
     * managed by default, so you must assign it to a CCNode or manually 
     * manage the memory yourself.
     * @param gap Space between nodes
     * @param alignVertically Whether to align the nodes vertically, and if so, 
     * what Y position to align them at
     * @returns Created RowLayout
     */
    static RowLayout* create(
        float gap = 5.f,
        std::optional<float> alignVertically = std::nullopt
    );

    RowLayout* setAlignment(Alignment align);
    RowLayout* setGap(float gap);
    RowLayout* setAlignVertically(std::optional<float> align);
};

/**
 * Simple layout for arranging nodes in a column (vertical line)
 */
class GEODE_DLL ColumnLayout : public Layout {
protected:
    Alignment m_alignment = Alignment::Center;
    std::optional<float> m_alignHorizontally;
    float m_gap;

public:
    void apply(CCArray* nodes, CCSize const& availableSize) override;

    static ColumnLayout* create(
        float gap = 5.f,
        std::optional<float> alignHorizontally = std::nullopt
    );

    ColumnLayout* setAlignment(Alignment align);
    ColumnLayout* setGap(float gap);
    ColumnLayout* setAlignHorizontally(std::optional<float> align);
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
    void apply(CCArray* nodes, CCSize const& availableSize) override;

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
