#include <Geode/ui/SimpleAxisLayout.hpp>
#include <Geode/ui/SpacerNode.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

class SimpleAxisLayoutOptions::Impl {
public:
    std::optional<float> m_minRelativeScale = .5f;
    std::optional<float> m_maxRelativeScale = 2.f;
    ScalingPriority m_scalingPriority = ScalingPriority::Normal;
};

SimpleAxisLayoutOptions::SimpleAxisLayoutOptions() : m_impl(std::make_unique<Impl>()) {}
SimpleAxisLayoutOptions::~SimpleAxisLayoutOptions() = default;

SimpleAxisLayoutOptions* SimpleAxisLayoutOptions::create() {
    auto ret = new SimpleAxisLayoutOptions();
    ret->autorelease();
    return ret;
}

SimpleAxisLayoutOptions* SimpleAxisLayoutOptions::setMinRelativeScale(std::optional<float> scale) {
    m_impl->m_minRelativeScale = scale;
    return this;
}

SimpleAxisLayoutOptions* SimpleAxisLayoutOptions::setMaxRelativeScale(std::optional<float> scale) {
    m_impl->m_maxRelativeScale = scale;
    return this;
}

SimpleAxisLayoutOptions* SimpleAxisLayoutOptions::setScalingPriority(ScalingPriority priority) {
    m_impl->m_scalingPriority = priority;
    return this;
}

std::optional<float> SimpleAxisLayoutOptions::getMinRelativeScale() const {
    return m_impl->m_minRelativeScale;
}

std::optional<float> SimpleAxisLayoutOptions::getMaxRelativeScale() const {
    return m_impl->m_maxRelativeScale;
}

ScalingPriority SimpleAxisLayoutOptions::getScalingPriority() const {
    return m_impl->m_scalingPriority;
}

class SimpleAxisLayout::Impl {
public:
    Axis m_axis = Axis::Column;
    AxisScaling m_mainAxisScaling = AxisScaling::ScaleDownGaps;
    AxisScaling m_crossAxisScaling = AxisScaling::None;
    MainAxisAlignment m_mainAxisAlignment = MainAxisAlignment::Center;
    CrossAxisAlignment m_crossAxisAlignment = CrossAxisAlignment::Center;
    AxisDirection m_mainAxisDirection = AxisDirection::FrontToBack;
    AxisDirection m_crossAxisDirection = AxisDirection::FrontToBack;
    float m_gap = 0.f;
    std::optional<float> m_minRelativeScale = 0.5f;
    std::optional<float> m_maxRelativeScale = 2.f;

    std::unordered_map<CCNode*, float> m_originalScalesPerNode;
    std::unordered_map<CCNode*, float> m_relativeScalesPerNode;

    Impl(Axis axis) : m_axis(axis) {
        switch (axis) {
            case Axis::Column:
                m_mainAxisDirection = AxisDirection::TopToBottom;
                m_crossAxisDirection = AxisDirection::LeftToRight;
                break;
            case Axis::Row:
                m_mainAxisDirection = AxisDirection::LeftToRight;
                m_crossAxisDirection = AxisDirection::TopToBottom;
                break;
        }
    }

    std::unordered_map<CCNode*, float> calculateCrossScaling(CCNode* layout, std::vector<CCNode*> const& nodes);
    std::unordered_map<CCNode*, float> calculateMainScaling(CCNode* layout, std::vector<CCNode*> const& nodes, float totalGap);
    
    void applyCrossPositioning(CCNode* layout, std::vector<CCNode*> const& nodes);
    void applyMainPositioning(CCNode* layout, std::vector<CCNode*> const& nodes, std::vector<SpacerNode*> const& spacers, float totalGap);

    void apply(cocos2d::CCNode* on);

    float getContentWidth(CCNode* on) const {
        if (m_axis == Axis::Column) {
            return on->getContentSize().width;
        }
        else {
            return on->getContentSize().height;
        }
    }

    float getContentHeight(CCNode* on) const {
        if (m_axis == Axis::Column) {
            return on->getContentSize().height;
        }
        else {
            return on->getContentSize().width;
        }
    }

    void setContentWidth(CCNode* on, float width) {
        if (m_axis == Axis::Column) {
            on->setContentSize({ width, on->getContentSize().height });
        }
        else {
            on->setContentSize({ on->getContentSize().width, width });
        }
    }

    void setContentHeight(CCNode* on, float height) {
        if (m_axis == Axis::Column) {
            on->setContentSize({ on->getContentSize().width, height });
        }
        else {
            on->setContentSize({ height, on->getContentSize().height });
        }
    }

    float getPositionX(CCNode* on) const {
        if (m_axis == Axis::Column) {
            return on->getPositionX();
        }
        else {
            return on->getPositionY();
        }
    }

    float getPositionY(CCNode* on) const {
        if (m_axis == Axis::Column) {
            return on->getPositionY();
        }
        else {
            return on->getPositionX();
        }
    }

    void setPositionX(CCNode* on, float x) {
        if (m_axis == Axis::Column) {
            on->setPosition(x, on->getPositionY());
        }
        else {
            on->setPosition(on->getPositionX(), x);
        }
    }

    void setPositionY(CCNode* on, float y) {
        if (m_axis == Axis::Column) {
            on->setPosition(on->getPositionX(), y);
        }
        else {
            on->setPosition(y, on->getPositionY());
        }
    }

    float getScale(CCNode* on) const {
        return on->getScale();
    }

    void setScale(CCNode* on, float scale) {
        on->setScale(scale);
    }

    SimpleAxisLayoutOptions* getLayoutOptions(CCNode* on) const {
        return typeinfo_cast<SimpleAxisLayoutOptions*>(on->getLayoutOptions());
    }

    // get the minimum allowed scale for the node
    // if the node already has a relative scale set,
    // it will be taken into account
    std::optional<float> getMinScale(CCNode* on) const {
        auto const layoutOptions = this->getLayoutOptions(on);
        auto const minScale = layoutOptions ? layoutOptions->getMinRelativeScale() : m_minRelativeScale;
        if (minScale) return *minScale / m_relativeScalesPerNode.at(on);
        return std::nullopt;
    }

    // get the maximum allowed scale for the node
    // if the node already has a relative scale set,
    // it will be taken into account
    std::optional<float> getMaxScale(CCNode* on) const {
        auto const layoutOptions = this->getLayoutOptions(on);
        auto const maxScale = layoutOptions ? layoutOptions->getMaxRelativeScale() : m_maxRelativeScale;
        if (maxScale) return *maxScale / m_relativeScalesPerNode.at(on);
        return std::nullopt;
    }

    // get the maximum allowed scale for the node
    // based on the layout's width and the node's width
    float getMaxCrossScale(CCNode* layout, CCNode* on) const {
        auto const layoutWidth = this->getContentWidth(layout);
        auto const width = this->getContentWidth(on) * this->getScale(on);
        auto const maxAllowedScale = layoutWidth / width;
        auto const maxScale = this->getMaxScale(on);
        if (maxScale) return std::min(maxAllowedScale, *maxScale);
        return maxAllowedScale;
    }
};

std::unordered_map<CCNode*, float> SimpleAxisLayout::Impl::calculateCrossScaling(CCNode* layout, std::vector<CCNode*> const& nodes) {
    std::unordered_map<CCNode*, float> scales;

    auto maxWidth = std::numeric_limits<float>::min();
    auto layoutWidth = this->getContentWidth(layout);

    // get the limits we are working with
    for (auto node : nodes) {
        auto const width = this->getContentWidth(node) * this->getScale(node);
        if (width > maxWidth) {
            maxWidth = width;
        }
    }

    switch (m_crossAxisScaling) {
        case AxisScaling::Grow:
            // grow the layout to fit the widest node
            if (maxWidth > layoutWidth) layoutWidth = maxWidth;
            break;
        case AxisScaling::Fit:
            // fit the layout to the widest node
            layoutWidth = maxWidth;
            break;
        default:
            break;
    }

    this->setContentWidth(layout, layoutWidth);

    // get the scales we need for current limits
    for (auto node : nodes) {
        switch (m_crossAxisScaling) {
            case AxisScaling::ScaleDownGaps:
            case AxisScaling::ScaleDown: {
                auto const width = this->getContentWidth(node) * this->getScale(node);
                auto const minScale = this->getMinScale(node);

                // scale down if needed
                if (width > layoutWidth) {
                    scales[node] = std::clamp(layoutWidth / width, minScale.value_or(0.f), 1.f);
                }
                break;
            }
            case AxisScaling::Scale: {
                auto const width = this->getContentWidth(node) * this->getScale(node);
                auto const minScale = this->getMinScale(node);
                auto const maxScale = this->getMaxCrossScale(layout, node);

                // scale both up and down
                scales[node] = std::clamp(layoutWidth / width, minScale.value_or(0.f), maxScale);
                break;
            }
            default:
                break;
        }
    }

    return scales;
}

// assumes scales are reverted before call
std::unordered_map<CCNode*, float> SimpleAxisLayout::Impl::calculateMainScaling(CCNode* layout, std::vector<CCNode*> const& nodes, float totalGap) {
    std::unordered_map<CCNode*, float> scales;

    auto totalHeight = totalGap;
    auto layoutHeight = this->getContentHeight(layout);

    // get the limits we are working with
    for (auto node : nodes) {
        auto const height = this->getContentHeight(node) * this->getScale(node);
        totalHeight += height;
    }

    switch (m_mainAxisScaling) {
        case AxisScaling::Grow:
            // grow the layout to fit all the nodes
            if (totalHeight > layoutHeight) layoutHeight = totalHeight;
            break;
        case AxisScaling::Fit:
            // fit the layout to all the nodes
            layoutHeight = totalHeight;
            break;
        case AxisScaling::ScaleDownGaps:
            // remove gaps if needed to fit the layout
            if (totalHeight > layoutHeight && totalHeight - totalGap <= layoutHeight) {
                totalHeight = layoutHeight;
            }
            else if (totalHeight > layoutHeight) {
                // remove as much as we can
                totalHeight -= totalGap;
            }
        default:
            break;
    }

    this->setContentHeight(layout, layoutHeight);

    std::unordered_map<ScalingPriority, std::vector<CCNode*>> sortedNodes;
    std::unordered_map<ScalingPriority, float> reducedHeightPerPriority;
    std::unordered_map<ScalingPriority, float> increasedHeightPerPriority;
    // calculate min max heights based on priorities
    for (auto node : nodes) {
        // sort the nodes by priority, so we can scale them later
        // in the correct order
        auto const layoutOptions = this->getLayoutOptions(node);
        auto const scalingPriority = layoutOptions ? layoutOptions->getScalingPriority() : ScalingPriority::Normal;
        sortedNodes[scalingPriority].push_back(node);

        switch (m_mainAxisScaling) {
            case AxisScaling::ScaleDownGaps:
            case AxisScaling::ScaleDown: {
                auto const height = this->getContentHeight(node) * this->getScale(node);
                auto const minScale = this->getMinScale(node);

                // scale down if needed
                auto minHeight = height * minScale.value_or(0.f);
                // store how much scaling reduced the height
                reducedHeightPerPriority[scalingPriority] += height - minHeight;

                break;
            }
            case AxisScaling::Scale: {
                auto const height = this->getContentHeight(node) * this->getScale(node);
                auto const minScale = this->getMinScale(node);
                auto const maxScale = this->getMaxCrossScale(layout, node);

                // scale both up and down
                auto minHeight = height * minScale.value_or(0.f);
                auto maxHeight = height * maxScale;
                // store how much scaling reduced and increased the height
                reducedHeightPerPriority[scalingPriority] += height - minHeight;
                increasedHeightPerPriority[scalingPriority] += maxHeight - height;

                break;
            }
            default:
                break;
        }
    }

    switch (m_mainAxisScaling) {
        case AxisScaling::None:
        case AxisScaling::Grow:
        case AxisScaling::Fit:
            return scales;
        default:
            break;
    }

    // sort the nodes by priority
    if (totalHeight > layoutHeight) {
        for (auto& [priority, sorted] : sortedNodes) {
            std::sort(sorted.begin(), sorted.end(), [&](CCNode* a, CCNode* b) {
                auto const prioA = this->getMinScale(a);
                auto const prioB = this->getMinScale(b);
                // biggest min scale will be first,
                // since it is likely the target scale
                // will be smaller than the allowed min scale,
                // allowing us to change the target scale later
                return prioA.value_or(0.f) > prioB.value_or(0.f);
            });
        }
    }
    else {
        for (auto& [priority, sorted] : sortedNodes) {
            std::sort(sorted.begin(), sorted.end(), [&](CCNode* a, CCNode* b) {
                auto const prioA = this->getMaxCrossScale(layout, a);
                auto const prioB = this->getMaxCrossScale(layout, b);
                // smallest max scale will be first,
                // since it is likely the target scale
                // will be bigger than the allowed max scale,
                // allowing us to change the target scale later
                return prioA < prioB;
            });
        }
    }

    for (auto priority : { 
        ScalingPriority::First, ScalingPriority::Early, ScalingPriority::Normal, 
        ScalingPriority::Late, ScalingPriority::Last 
    }) {
        if (totalHeight > layoutHeight) {
            // scale down the nodes, we are over the limit
            auto const reducedHeight = reducedHeightPerPriority[priority];
            auto difference = totalHeight - layoutHeight;
            if (reducedHeight > difference) {
                // only partially scale down, should be the last priority to scale
                auto priorityHeight = 0.f;
                for (auto node : sortedNodes[priority]) {
                    auto const height = this->getContentHeight(node) * this->getScale(node);
                    priorityHeight += height;
                }
                // remainingHeight stores unscaled remaining height
                auto remainingHeight = priorityHeight;

                // set our target scale to the remaining height
                // which may change if minScale is bigger than the target scale
                auto targetScale = (remainingHeight - difference) / remainingHeight;
                // minScales are sorted in a decreasing priority
                for (auto node : sortedNodes[priority]) {
                    auto const height = this->getContentHeight(node) * this->getScale(node);
                    auto const minScale = this->getMinScale(node);

                    auto const scale = std::max(targetScale, minScale.value_or(0.f));
                    auto const minHeight = height * scale;
                    scales[node] = scale;

                    // reduce the remaining height and difference
                    remainingHeight -= height;
                    difference -= height - minHeight;

                    // we need to readjust the target scale if we have remaining height
                    targetScale = (remainingHeight - difference) / remainingHeight;
                }

                break;
            }
            else {
                // scale down all the way
                for (auto node : sortedNodes[priority]) {
                    auto const minScale = this->getMinScale(node);
                    scales[node] = minScale.value_or(0.f);
                }
            }

            totalHeight -= reducedHeight;
        }
        else {
            if (m_mainAxisScaling != AxisScaling::Scale) {
                break;
            }
            // scale up the nodes, we are under the limit
            auto const increasedHeight = increasedHeightPerPriority[priority];
            auto difference = layoutHeight - totalHeight;
            if (increasedHeight > difference) {
                // only partially scale up, should be the last priority to scale
                auto priorityHeight = 0.f;
                for (auto node : sortedNodes[priority]) {
                    auto const height = this->getContentHeight(node) * this->getScale(node);
                    priorityHeight += height;
                }
                // remainingHeight stores unscaled remaining height
                auto remainingHeight = priorityHeight;

                // set our target scale to the remaining height
                // which may change if maxScale is smaller than the target scale
                auto targetScale = (remainingHeight + difference) / remainingHeight;
                // maxScales are sorted in an increasing priority
                for (auto node : sortedNodes[priority]) {
                    auto const height = this->getContentHeight(node) * this->getScale(node);
                    auto const maxScale = this->getMaxCrossScale(layout, node);

                    auto const scale = std::min(targetScale, maxScale);
                    auto const maxHeight = height * scale;
                    scales[node] = scale;

                    // reduce the remaining height and difference
                    remainingHeight -= height;
                    difference -= maxHeight - height;

                    // we need to readjust the target scale if we have remaining height
                    targetScale = (remainingHeight + difference) / remainingHeight;
                }

                break;
            }
            else {
                // scale up all the way
                for (auto node : sortedNodes[priority]) {
                    auto const maxScale = this->getMaxCrossScale(layout, node);
                    scales[node] = maxScale;
                }
            }
        }
    }

    return scales;
}

void SimpleAxisLayout::Impl::applyCrossPositioning(CCNode* layout, std::vector<CCNode*> const& nodes) {
    auto maxWidth = 0.f;
    auto layoutWidth = this->getContentWidth(layout);
    for (auto node : nodes) {
        auto const width = this->getContentWidth(node) * this->getScale(node);
        if (width > maxWidth) {
            maxWidth = width;
        }
    }

    // reapply grow/fit since main scaling may have changed the max width
    switch (m_crossAxisScaling) {
        case AxisScaling::Grow:
            if (maxWidth > layoutWidth) layoutWidth = maxWidth;
            break;
        case AxisScaling::Fit:
            layoutWidth = maxWidth;
            break;
        default:
            break;
    }

    this->setContentWidth(layout, layoutWidth);

    // cross axis direction only exists to disambiguate the alignment
    CrossAxisAlignment alignment = m_crossAxisAlignment;
    if (m_crossAxisDirection == AxisDirection::BackToFront) {
        switch (m_crossAxisAlignment) {
            case CrossAxisAlignment::Start:
                alignment = CrossAxisAlignment::End;
                break;
            case CrossAxisAlignment::End:
                alignment = CrossAxisAlignment::Start;
                break;
            default:
                break;
        }
    }

    for (auto node : nodes) {
        auto const scale = this->getScale(node);
        auto const width = this->getContentWidth(node) * scale;
        auto const remainingWidth = layoutWidth - width;

        node->ignoreAnchorPointForPosition(false);
        node->setAnchorPoint({ 0.5f, 0.5f });

        switch (alignment) {
            // remainingWidth is the space left after the node is placed
            // and width * .5 is added since the anchor point is in the middle
            case CrossAxisAlignment::Start:
                this->setPositionX(node, width * 0.5f);
                break;
            case CrossAxisAlignment::Center:
                this->setPositionX(node, remainingWidth * 0.5f + width * 0.5f);
                break;
            case CrossAxisAlignment::End:
                this->setPositionX(node, remainingWidth + width * 0.5f);
                break;
            default:
                break;
        }
    }
}
void SimpleAxisLayout::Impl::applyMainPositioning(CCNode* layout, std::vector<CCNode*> const& nodes, std::vector<SpacerNode*> const& spacers, float totalGap) {
    // get the limits we are working with
    auto totalHeight = totalGap;
    for (auto node : nodes) {
        auto const height = this->getContentHeight(node) * this->getScale(node);
        totalHeight += height;
    }
    auto const layoutHeight = this->getContentHeight(layout);

    auto gapPercentage = 1.f;
    if (m_mainAxisScaling == AxisScaling::ScaleDownGaps) {
        if (totalHeight > layoutHeight && totalHeight - totalGap <= layoutHeight) {
            auto const difference = totalHeight - layoutHeight;
            gapPercentage = 1.f - difference / totalGap;
            totalHeight = layoutHeight;
        }
        else if (totalHeight > layoutHeight) {
            // remove as much as we can
            gapPercentage = 0.f;
            totalHeight -= totalGap;
        }
    }

    auto const remainingHeight = layoutHeight - totalHeight;

    auto extraGap = remainingHeight;
    auto offset = 0.f;
    auto spacerGap = 0.f;
    if (spacers.size() > 0) {
        // if there are spacer nodes, we allocate all the remaining space to them
        size_t totalGrow = 0;
        for (auto spacer : spacers) {
            totalGrow += spacer->getGrow();
        }
        extraGap = 0.f;
        offset = 0.f;
        spacerGap = remainingHeight / totalGrow;
        // apply the new height to the spacers
        for (auto spacer : spacers) {
            this->setScale(spacer, 1.f);
            auto const height = spacer->getGrow() * spacerGap;
            this->setContentHeight(spacer, height);
            this->setContentWidth(spacer, this->getContentWidth(layout));
            this->setPositionX(spacer, this->getContentWidth(layout) / 2);
        }
    }
    else {
        switch (m_mainAxisAlignment) {
            // starts at the start of the layout
            // no offset is needed
            case MainAxisAlignment::Start:
                extraGap = 0.f;
                offset = 0.f;
                break;
            // starts at the center of the layout
            // half of the remaining space is added to the offset
            case MainAxisAlignment::Center:
                extraGap = 0.f;
                offset = remainingHeight / 2;
                break;
            // starts at the end of the layout
            // all of the remaining space is added to the offset
            case MainAxisAlignment::End:
                extraGap = 0.f;
                offset = remainingHeight;
                break;
            // remaining space is divided between the nodes + 1 (outside included)
            // and the offset is set to the extra gap
            case MainAxisAlignment::Even:
                extraGap = remainingHeight / (nodes.size() + 1);
                offset = extraGap;
                break;
            // remaining space is divided between the nodes - 1 (outside excluded)
            // and the offset is set to 0
            case MainAxisAlignment::Between:
                extraGap = remainingHeight / (nodes.size() - 1);
                offset = 0.0f;
                break;
            // remaining space is divided between the nodes (outside half included)
            // and the offset is set to half of the extra gap
            case MainAxisAlignment::Around:
                extraGap = remainingHeight / nodes.size();
                offset = extraGap / 2.0f;
                break;
        }
    }

    // change the offset based on the direction
    if (m_mainAxisDirection == AxisDirection::BackToFront) {
        offset = layoutHeight - offset;
    }

    CCNode* lastChild = nullptr;
    for (auto node : nodes) {
        // apply the gap between the nodes
        if (auto gap = typeinfo_cast<AxisGap*>(node)) {
            offset += gap->getGap() * gapPercentage;
            lastChild = nullptr;
            continue;
        }
        // otherwise use the default gap
        if (lastChild) {
            offset += m_gap * gapPercentage;
        }

        auto const height = this->getContentHeight(node) * this->getScale(node);
        
        node->ignoreAnchorPointForPosition(false);
        node->setAnchorPoint(ccp(0.5f, 0.5f));

        switch (m_mainAxisDirection) {
            // items are laid out from top to bottom
            // so the center is subtracted from the offset
            case AxisDirection::BackToFront:
                this->setPositionY(node, offset - height / 2);
                offset -= height + extraGap;
                break;
            // items are laid out from bottom to top
            // so the center is added to the offset
            case AxisDirection::FrontToBack:
                this->setPositionY(node, offset + height / 2);
                offset += height + extraGap;
                break;
        }
        lastChild = node;
    }
}

void SimpleAxisLayout::Impl::apply(cocos2d::CCNode* layout) {
    std::vector<CCNode*> realChildren;
    std::vector<CCNode*> positionChildren;
    std::vector<SpacerNode*> spacers;
    std::vector<AxisGap*> gaps;
    float totalGap = 0.f;
    CCNode* lastChild = nullptr;
    for (auto child : CCArrayExt<CCNode*>(layout->getChildren())) {
        if (auto spacer = typeinfo_cast<SpacerNode*>(child)) {
            spacers.push_back(spacer);
            positionChildren.push_back(spacer);
        }
        else if (auto gap = typeinfo_cast<AxisGap*>(child)) {
            gaps.push_back(gap);
            totalGap += gap->getGap();
            // axis gaps are not used for gap ignoring alignments
            switch (m_mainAxisAlignment) {
                case MainAxisAlignment::Start:
                case MainAxisAlignment::Center:
                case MainAxisAlignment::End:
                    positionChildren.push_back(gap);
                default:
                    break;
            }
            // we use lastChild only for gap calculation
            // so we reset it here to not use default gap
            // for the next child
            lastChild = nullptr;
        }
        else {
            if (lastChild) {
                totalGap += m_gap;
            }
            realChildren.push_back(child);
            positionChildren.push_back(child);
            lastChild = child;
        }
    }

    // revert back to original scale if needed
    for (auto child : realChildren) {
        auto const expectedScale = m_originalScalesPerNode[child] * m_relativeScalesPerNode[child];
        auto const scale = this->getScale(child);
        if (scale != expectedScale) {
            // the scale was manually changed, so lets accept
            // the new scale as the original scale
            m_originalScalesPerNode[child] = scale;
        }
        else {
            this->setScale(child, m_originalScalesPerNode[child]);
        }
        m_relativeScalesPerNode[child] = 1.f;
    }

    // calculate required cross scaling
    auto crossScales = this->calculateCrossScaling(layout, realChildren);
    for (auto child : realChildren) {
        auto scale = 1.f;
        if (crossScales.contains(child)) {
            scale *= crossScales[child];
        }

        this->setScale(child, scale);
        m_relativeScalesPerNode[child] = scale;
    }

    // calculate required main scaling
    // since cross scaling might change the relative scales,
    // minScale and maxScale functions account for this change
    auto mainScales = this->calculateMainScaling(layout, realChildren, totalGap);
    for (auto child : realChildren) {
        auto scale = m_relativeScalesPerNode[child];
        if (mainScales.contains(child)) {
            scale *= mainScales[child];
        }

        this->setScale(child, scale);
        m_relativeScalesPerNode[child] = scale;
    }

    // apply positions
    this->applyCrossPositioning(layout, realChildren);
    this->applyMainPositioning(layout, positionChildren, spacers, totalGap);
}

SimpleAxisLayout::SimpleAxisLayout(Axis axis) : m_impl(std::make_unique<Impl>(axis)) {}

SimpleAxisLayout::~SimpleAxisLayout() = default;

SimpleAxisLayout* SimpleAxisLayout::create(Axis axis) {
    auto ret = new SimpleAxisLayout(axis);
    ret->autorelease();
    return ret;
}

cocos2d::CCSize SimpleAxisLayout::getSizeHint(cocos2d::CCNode* on) const {
    return on->getContentSize();
}

void SimpleAxisLayout::apply(cocos2d::CCNode* on) {
    m_impl->apply(on);
}

SimpleAxisLayout* SimpleAxisLayout::setAxis(Axis axis) {
    m_impl->m_axis = axis;
    return this;
}

SimpleAxisLayout* SimpleAxisLayout::setMainAxisScaling(AxisScaling scaling) {
    m_impl->m_mainAxisScaling = scaling;
    return this;
}

SimpleAxisLayout* SimpleAxisLayout::setCrossAxisScaling(AxisScaling scaling) {
    m_impl->m_crossAxisScaling = scaling;
    return this;
}

SimpleAxisLayout* SimpleAxisLayout::setMainAxisAlignment(MainAxisAlignment alignment) {
    m_impl->m_mainAxisAlignment = alignment;
    return this;
}

SimpleAxisLayout* SimpleAxisLayout::setCrossAxisAlignment(CrossAxisAlignment alignment) {
    m_impl->m_crossAxisAlignment = alignment;
    return this;
}

SimpleAxisLayout* SimpleAxisLayout::setMainAxisDirection(AxisDirection direction) {
    m_impl->m_mainAxisDirection = direction;
    return this;
}

SimpleAxisLayout* SimpleAxisLayout::setCrossAxisDirection(AxisDirection direction) {
    m_impl->m_crossAxisDirection = direction;
    return this;
}

SimpleAxisLayout* SimpleAxisLayout::setGap(float gap) {
    m_impl->m_gap = gap;
    return this;
}

SimpleAxisLayout* SimpleAxisLayout::setMinRelativeScale(std::optional<float> scale) {
    m_impl->m_minRelativeScale = scale;
    return this;
}

SimpleAxisLayout* SimpleAxisLayout::setMaxRelativeScale(std::optional<float> scale) {
    m_impl->m_maxRelativeScale = scale;
    return this;
}

SimpleRowLayout::SimpleRowLayout() : SimpleAxisLayout(Axis::Row) {}

SimpleRowLayout::~SimpleRowLayout() = default;

SimpleRowLayout* SimpleRowLayout::create() {
    auto ret = new SimpleRowLayout();
    ret->autorelease();
    return ret;
}

SimpleColumnLayout::SimpleColumnLayout() : SimpleAxisLayout(Axis::Column) {}

SimpleColumnLayout::~SimpleColumnLayout() = default;

SimpleColumnLayout* SimpleColumnLayout::create() {
    auto ret = new SimpleColumnLayout();
    ret->autorelease();
    return ret;
}

class AxisGap::Impl {
public:
    float m_gap;

    Impl(float gap) : m_gap(gap) {}
};

AxisGap::AxisGap(float gap) : m_impl(std::make_unique<Impl>(gap)) {}

AxisGap::~AxisGap() = default;

AxisGap* AxisGap::create(float gap) {
    auto ret = new AxisGap(gap);
    ret->autorelease();
    return ret;
}

float AxisGap::getGap() const {
    return m_impl->m_gap;
}

AxisGap* AxisGap::setGap(float gap) {
    m_impl->m_gap = gap;
    return this;
}