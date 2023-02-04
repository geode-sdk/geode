#include <cocos2d.h>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>

USE_GEODE_NAMESPACE();

#pragma warning(disable: 4273)

void CCNode::swapChildIndices(CCNode* first, CCNode* second) {
    m_pChildren->exchangeObject(first, second);
}

CCArray* Layout::getNodesToPosition(CCNode* on) {
    auto filtered = CCArray::create();
    for (auto& child : CCArrayExt<CCNode>(on->getChildren())) {
        if (child->getPositionHint() != PositionHint::Absolute) {
            filtered->addObject(child);
        }
    }
    return filtered;
}

void AxisLayout::apply(CCNode* on) {
    auto nodes = getNodesToPosition(on);
    if (m_reverse) {
        nodes->reverseObjects();
    }

    float availableAxisLength;
    float originalCrossHeight;

    if (m_axis == Axis::Row) {
        availableAxisLength = on->getContentSize().width;
        originalCrossHeight = on->getContentSize().height;
    }
    else {
        availableAxisLength = on->getContentSize().height;
        originalCrossHeight = on->getContentSize().width;
    }

    size_t ix = 0;
    float totalAxisLength = .0f;
    float maxCrossLength = 0.f;
    for (auto& node : CCArrayExt<CCNode*>(nodes)) {
        float axisLength;
        float axisAnchor;
        float crossLength;
        if (m_axis == Axis::Row) {
            axisLength = node->getScaledContentSize().width;
            axisAnchor = node->getAnchorPoint().x;
            crossLength = node->getScaledContentSize().height;
        }
        else {
            axisLength = node->getScaledContentSize().height;
            axisAnchor = node->getAnchorPoint().y;
            crossLength = node->getScaledContentSize().width;
        }
        // if no need to fit fully inside and only one item exists, the total 
        // width taken up is 0
        if (nodes->count() == 1 && !m_fitInside) {
            totalAxisLength = 0;
        }
        // if no need to fit fully inside, figure out what part may overflow 
        // for first item
        else if (ix == 0 && !m_fitInside) {
            totalAxisLength += axisLength * (1.f - axisAnchor);
        }
        // if no need to fit fully inside, figure out what part may overflow
        // for last item
        else if (ix == nodes->count() - 1 && !m_fitInside) {
            totalAxisLength += axisLength * axisAnchor;
        }
        // otherwise either we need to fit fully inside or this node is not 
        // at the start or end
        else {
            totalAxisLength += axisLength;
        }
        if (ix) {
            totalAxisLength += m_gap;
        }
        if (crossLength > maxCrossLength) {
            maxCrossLength = crossLength;
        }
        ix++;
    }

    const auto minScale = .65f;

    // assume intended scale is 1x
    auto setScale = 1.f;
    auto squeeze = 1.f;

    // check for overflow
    // first try to make the node smaller
    if (totalAxisLength > availableAxisLength && m_autoScale) {
        setScale = availableAxisLength / totalAxisLength;
        if (setScale < minScale) {
            setScale = minScale;
        }
        totalAxisLength *= setScale;
    }

    // if we're still overflowing, squeeze nodes closer together
    if (totalAxisLength > availableAxisLength) {
        squeeze = availableAxisLength / totalAxisLength;
        totalAxisLength = availableAxisLength;
    }

    // resize target to match settings
    if (m_shrinkCrossAxis) {
        if (m_axis == Axis::Row) {
            on->setContentSize({
                availableAxisLength,
                maxCrossLength,
            });
        }
        else {
            on->setContentSize({
                maxCrossLength,
                availableAxisLength,
            });
        }
    }

    float pos;
    switch (m_axisAlignment) {
        case Alignment::Begin: { 
            pos = 0.f;
        } break;

        case Alignment::Center: {
            pos = availableAxisLength / 2 - totalAxisLength / 2;
        } break;

        case Alignment::End: {
            pos = availableAxisLength - totalAxisLength;
        } break;
    }
    ix = 0;
    for (auto& node : CCArrayExt<CCNode*>(nodes)) {
        // rescale node if overflowing
        if (m_autoScale) {
            // CCMenuItemSpriteExtra is quirky af
            if (auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(node)) {
                btn->m_baseScale = setScale;
            }
            node->setScale(setScale);
        }
        float axisLength;
        float axisAnchor;
        float crossLength;
        float crossAnchor;
        if (m_axis == Axis::Row) {
            axisLength = node->getScaledContentSize().width;
            axisAnchor = node->getAnchorPoint().x;
            crossLength = node->getScaledContentSize().height;
            crossAnchor = node->getAnchorPoint().y;
        }
        else {
            axisLength = node->getScaledContentSize().height;
            axisAnchor = node->getAnchorPoint().y;
            crossLength = node->getScaledContentSize().width;
            crossAnchor = node->getAnchorPoint().x;
        }
        float axisPos;
        if (ix == 0 && !m_fitInside) {
            axisPos = pos;
            pos += (axisLength * (1.f - axisAnchor) + m_gap * setScale) * squeeze;
        }
        else {
            axisPos = pos + axisLength * axisAnchor * squeeze;
            pos += (axisLength + m_gap * setScale) * squeeze;
        }
        float crossPos;
        switch (m_crossAlignment) {
            case Alignment::Begin: {
                crossPos = crossLength * crossAnchor;
            } break;

            case Alignment::Center: {
                crossPos = maxCrossLength / 2 - crossLength * (.5f - crossAnchor);
            } break;

            case Alignment::End: {
                crossPos = maxCrossLength - crossLength * (1.f - crossAnchor);
            } break;
        }
        if (m_axis == Axis::Row) {
            node->setPosition(axisPos, crossPos);
        }
        else {
            node->setPosition(crossPos, axisPos);
        }
        ix++;
    }
}

AxisLayout::AxisLayout(Axis axis) : m_axis(axis) {}

AxisLayout* AxisLayout::setCrossAxisAlignment(Alignment align) {
    m_crossAlignment = align;
    return this;
}

AxisLayout* AxisLayout::setAxisAlignment(Alignment align) {
    m_axisAlignment = align;
    return this;
}

AxisLayout* AxisLayout::setGap(float gap) {
    m_gap = gap;
    return this;
}

AxisLayout* AxisLayout::setReverse(bool reverse) {
    m_reverse = reverse;
    return this;
}

AxisLayout* AxisLayout::setAutoScale(bool scale) {
    m_autoScale = scale;
    return this;
}

AxisLayout* AxisLayout::setFitInside(bool fit) {
    m_fitInside = fit;
    return this;
}

AxisLayout* AxisLayout::setShrinkCrossAxis(bool shrink) {
    m_shrinkCrossAxis = shrink;
    return this;
}

RowLayout::RowLayout() : AxisLayout(AxisLayout::Row) {}

RowLayout* RowLayout::create() {
    return new RowLayout();
}

ColumnLayout::ColumnLayout() : AxisLayout(AxisLayout::Column) {}

ColumnLayout* ColumnLayout::create() {
    return new ColumnLayout();
}

void GridLayout::apply(CCNode* on) {
    // todo
}

GridLayout* GridLayout::create(
    std::optional<size_t> rowSize,
    GridAlignment alignment,
    GridDirection direction
) {
    auto ret = new GridLayout;
    ret->m_rowSize = rowSize;
    ret->m_alignment = alignment;
    ret->m_direction = direction;
    return ret;
}

GridLayout* GridLayout::setDirection(GridDirection direction) {
    m_direction = direction;
    return this;
}

GridLayout* GridLayout::setAlignment(GridAlignment alignment) {
    m_alignment = alignment;
    return this;
}

GridLayout* GridLayout::setRowSize(std::optional<size_t> rowSize) {
    m_rowSize = rowSize;
    return this;
}

