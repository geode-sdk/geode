#pragma once

#include "Layout.hpp"

namespace geode {
enum class AxisScaling {
    // Does not scale items
    None,
    // Scales items down if necessary to fit
    ScaleDown,
    // Scales items up/down to fit
    Scale,
    // Grows the layout if necessary to fit
    Grow,
    // Fits the layout to the items
    Fit,
    // Shrinks gaps if needed to fit, then scales down items
    ScaleDownGaps,
};

enum class ScalingPriority {
    // Scales down first
    First,
    // Scales down second
    Early,
    // Default scaling priority
    Normal,
    // Scales down second to last
    Late,
    // Scales down last
    Last,
    // Does not scale
    Never = 128,
};

enum class MainAxisAlignment {
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
    // Space between each item is the same (disregards gap)
    // |o...o...o|
    Between,
    // Space around each item is the same (disregards gap)
    // |.o..o..o.|
    Around,
};

enum class CrossAxisAlignment {
    // Align items to the start
    // |ooo......|
    Start,
    // All items are centered
    // |...ooo...|
    Center,
    // Align items to the end
    // |......ooo|
    End,
};

enum class AxisDirection {
    // Items are laid out from top to bottom
    TopToBottom = 1,
    // Items are laid out from bottom to top
    BottomToTop = 0,
    // Items are laid out from left to right
    LeftToRight = 0,
    // Items are laid out from right to left
    RightToLeft = 1,
    // Items are laid out from front to back
    FrontToBack = 0,
    // Items are laid out from back to front
    BackToFront = 1,
};

class GEODE_DLL SimpleAxisLayoutOptions : public LayoutOptions {
protected:
    class Impl;

    std::unique_ptr<Impl> m_impl;

    SimpleAxisLayoutOptions();

public:
    static SimpleAxisLayoutOptions* create();

    virtual ~SimpleAxisLayoutOptions();

    /**
     * Sets the minimum relative scale the node can be scaled to
     * if required to scale down to fit the layout
     */
    SimpleAxisLayoutOptions* setMinRelativeScale(std::optional<float> scale);
    /**
     * Sets the maximum relative scale the node can be scaled to
     * if required to scale up to fit the layout
     */
    SimpleAxisLayoutOptions* setMaxRelativeScale(std::optional<float> scale);
    /**
     * Sets the scaling priority for the node, to specify
     * when the node should be scaled compared to other nodes
     * during main axis scaling
     */
    SimpleAxisLayoutOptions* setScalingPriority(ScalingPriority priority);

    std::optional<float> getMinRelativeScale() const;
    std::optional<float> getMaxRelativeScale() const;
    ScalingPriority getScalingPriority() const;
};

class GEODE_DLL SimpleAxisLayout : public Layout {
protected:
    class Impl;

    std::unique_ptr<Impl> m_impl;

    SimpleAxisLayout(Axis);

public:
    static SimpleAxisLayout* create(Axis axis);

    virtual ~SimpleAxisLayout();

    void apply(cocos2d::CCNode* on) override;
    cocos2d::CCSize getSizeHint(cocos2d::CCNode* on) const override;

    /**
     * Sets the axis of the layout
     */
    SimpleAxisLayout* setAxis(Axis axis);
    /**
     * Sets the scaling behaviour of the main axis
     * The default is set to AxisScaling::None
     */
    SimpleAxisLayout* setMainAxisScaling(AxisScaling scaling);
    /**
     * Sets the scaling behaviour of the cross axis
     * The default is set to AxisScaling::None
     */
    SimpleAxisLayout* setCrossAxisScaling(AxisScaling scaling);
    /**
     * Sets how the items are aligned on the main axis
     * The default is set to MainAxisAlignment::Start
     */
    SimpleAxisLayout* setMainAxisAlignment(MainAxisAlignment alignment);
    /**
     * Sets how the items are aligned on the cross axis
     * The default is set to CrossAxisAlignment::Center
     */
    SimpleAxisLayout* setCrossAxisAlignment(CrossAxisAlignment alignment);
    /**
     * Sets the direction of the main axis
     * The default is set to AxisDirection::TopToBottom for SimpleRow
     * and AxisDirection::LeftToRight for SimpleColumn
     */
    SimpleAxisLayout* setMainAxisDirection(AxisDirection direction);
    /**
     * Sets the direction of the cross axis
     * The default is set to AxisDirection::TopToBottom for SimpleRow
     * and AxisDirection::LeftToRight for SimpleColumn
     */
    SimpleAxisLayout* setCrossAxisDirection(AxisDirection direction);
    /**
     * Sets the gap between items, unless overridden by a AxisGap node
     * The default is set to 0.0f
     */
    SimpleAxisLayout* setGap(float gap);
    /**
     * Sets the minimum relative scale the node can be scaled to
     * The default is set to 0.5f
     */
    SimpleAxisLayout* setMinRelativeScale(std::optional<float> scale);
    /**
     * Sets the maximum relative scale the node can be scaled to
     * The default is set to 2.0f
     */
    SimpleAxisLayout* setMaxRelativeScale(std::optional<float> scale);

    Axis getAxis() const;
    AxisScaling getMainAxisScaling() const;
    AxisScaling getCrossAxisScaling() const;
    MainAxisAlignment getMainAxisAlignment() const;
    CrossAxisAlignment getCrossAxisAlignment() const;
    AxisDirection getMainAxisDirection() const;
    AxisDirection getCrossAxisDirection() const;
    float getGap() const;
    std::optional<float> getMinRelativeScale() const;
    std::optional<float> getMaxRelativeScale() const;
};

class GEODE_DLL SimpleRowLayout : public SimpleAxisLayout {
protected:
    SimpleRowLayout();

public:
    static SimpleRowLayout* create();

    virtual ~SimpleRowLayout();
};

class GEODE_DLL SimpleColumnLayout : public SimpleAxisLayout {
protected:
    SimpleColumnLayout();

public:
    static SimpleColumnLayout* create();

    virtual ~SimpleColumnLayout();
};

class GEODE_DLL AxisGap : public cocos2d::CCNode {
protected:
    class Impl;

    std::unique_ptr<Impl> m_impl;

    AxisGap(float gap);

public:
    static AxisGap* create(float gap);

    virtual ~AxisGap();

    /**
     * Sets the gap between items in the layout
     */
    AxisGap* setGap(float gap);

    float getGap() const;
};
}