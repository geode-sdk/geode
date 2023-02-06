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

static constexpr float AXIS_MIN_SCALE = 0.65f;

struct AxisPosition {
    float axisLength;
    float axisAnchor;
    float crossLength;
    float crossAnchor;
};

static AxisPosition nodeAxis(CCNode* node, Axis axis, float scale) {
    auto scaledSize = node->getScaledContentSize() * scale;
    auto anchor = node->getAnchorPoint();
    if (axis == Axis::Row) {
        return AxisPosition {
            .axisLength = scaledSize.width,
            .axisAnchor = anchor.x,
            .crossLength = scaledSize.height,
            .crossAnchor = anchor.y,
        };
    }
    else {
        return AxisPosition {
            .axisLength = scaledSize.height,
            .axisAnchor = anchor.y,
            .crossLength = scaledSize.width,
            .crossAnchor = anchor.x,
        };
    }
}

struct AxisLayout::Row : public CCObject {
    float nextOverflowScaleDownFactor;
    float nextOverflowSquichFactor;
    float axisLength;
    float crossLength;
    Ref<CCArray> nodes;

    Row(
        float scaleFactor,
        float squishFactor,
        float axisLength,
        float crossLength,
        CCArray* nodes
    ) : nextOverflowScaleDownFactor(scaleFactor),
        nextOverflowSquichFactor(squishFactor),
        axisLength(axisLength),
        crossLength(crossLength),
        nodes(nodes)
    {
        this->autorelease();
    }
};

AxisLayout::Row* AxisLayout::fitInRow(CCNode* on, CCArray* nodes, float scale, float squish) const {    
    float nextAxisLength = 0.f;
    float axisLength;
    float crossLength = 0.f;
    auto res = CCArray::create();

    auto available = nodeAxis(on, m_axis, 1.f);
    size_t ix = 0;
    for (auto& node : CCArrayExt<CCNode*>(nodes)) {
        auto pos = nodeAxis(node, m_axis, scale * squish);
        nextAxisLength += pos.axisLength;
        // if multiple rows are allowed and this row is full, time for the 
        // next row
        if (m_growCrossAxis && nextAxisLength > available.axisLength) {
            break;
        }
        res->addObject(node);
        nodes->removeFirstObject();
        if (ix) {
            nextAxisLength += m_gap * squish;
            axisLength += m_gap * squish;
        }
        axisLength += pos.axisLength;
        if (pos.crossLength > crossLength) {
            crossLength = pos.crossLength;
        }
        ix++;
    }

    // reverse row if needed
    if (m_axisReverse) {
        res->reverseObjects();
    }

    return new Row(
        // how much should the nodes be scaled down to fit the next row
        available.axisLength / (
            nextAxisLength - m_gap * (res->count() - 1)
        ) * scale * squish,
        available.axisLength / nextAxisLength * scale * squish,
        axisLength,
        crossLength,
        res
    );
}

void AxisLayout::tryFitLayout(CCNode* on, CCArray* nodes, float scale, float squish) const {
    auto rows = CCArray::create();
    float totalRowCrossLength = 0.f;
    float crossScaleDownFactor = AXIS_MIN_SCALE;
    float squishFactor = 1.f;
    size_t ix = 0;

    // fit everything into rows while possible
    auto newNodes = nodes->shallowCopy();
    while (newNodes->count()) {
        auto row = this->fitInRow(on, newNodes, scale, squish);
        rows->addObject(row);
        if (
            row->nextOverflowScaleDownFactor > crossScaleDownFactor &&
            crossScaleDownFactor < 1.f
        ) {
            crossScaleDownFactor = row->nextOverflowScaleDownFactor;
        }
        if (row->nextOverflowSquichFactor < squishFactor) {
            squishFactor = row->nextOverflowSquichFactor;
        }
        totalRowCrossLength += row->crossLength;
        if (ix) {
            totalRowCrossLength += m_gap;
        }
        ix++;
    }
    newNodes->release();

    auto available = nodeAxis(on, m_axis, 1.f);

    // if cross axis overflow not allowed, try to scale down layout
    if (!m_allowCrossAxisOverflow && totalRowCrossLength > available.crossLength) {
        if (m_autoScale && scale > AXIS_MIN_SCALE) {
            rows->release();
            return this->tryFitLayout(on, nodes, crossScaleDownFactor, squish);
        }
    }

    // if we're still overflowing, squeeze nodes closer together
    if (totalRowCrossLength > available.crossLength) {
        // if squishing rows would take less squishing that squishing columns, 
        // then squish rows
        if (totalRowCrossLength / available.crossLength < squishFactor) {
            rows->release();
            return this->tryFitLayout(on, nodes, scale, squishFactor);
        }
    }

    // if we're here, the nodes are ready to be positioned

    if (m_crossReverse) {
        rows->reverseObjects();
    }

    // resize cross axis if needed
    if (m_allowCrossAxisOverflow) {
        available.crossLength = totalRowCrossLength;
        if (m_axis == Axis::Row) {
            log::debug("axisLength: {}, totalRowCrossLength: {}", available.axisLength, totalRowCrossLength);
            on->setContentSize({
                available.axisLength,
                totalRowCrossLength,
            });
        }
        else {
            on->setContentSize({
                totalRowCrossLength,
                available.axisLength,
            });
        }
    }

    float crossPos;
    switch (m_crossAlignment) {
        case AxisAlignment::Start: {
            crossPos = 0.f;
        } break;

        case AxisAlignment::Center: {
            crossPos = available.crossLength / 2 - totalRowCrossLength / 2;
        } break;

        case AxisAlignment::End: {
            crossPos = available.crossLength - totalRowCrossLength;
        } break;

        case AxisAlignment::Even: {
            crossPos = 0.f;
        } break;
    }

    for (auto row : CCArrayExt<Row*>(rows)) {
        float rowAxisPos;
        switch (m_axisAlignment) {
            case AxisAlignment::Start: { 
                rowAxisPos = 0.f;
            } break;

            case AxisAlignment::Even: { 
                rowAxisPos = 0.f;
            } break;

            case AxisAlignment::Center: {
                rowAxisPos = available.axisLength / 2 - row->axisLength / 2;
            } break;

            case AxisAlignment::End: {
                rowAxisPos = available.axisLength - row->axisLength;
            } break;
        }

        for (auto& node : CCArrayExt<CCNode*>(row->nodes)) {
            // rescale node if overflowing
            if (m_autoScale) {
                // CCMenuItemSpriteExtra is quirky af
                if (auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(node)) {
                    btn->m_baseScale = scale;
                }
                node->setScale(scale);
            }
            auto pos = nodeAxis(node, m_axis, scale * squish);
            float axisPos = rowAxisPos + pos.axisLength * pos.axisAnchor;
            float crossPos;
            switch (m_crossAlignment) {
                case AxisAlignment::Start: {
                    crossPos = pos.crossLength * pos.crossAnchor;
                } break;

                case AxisAlignment::Center: case AxisAlignment::Even: {
                    crossPos = row->crossLength / 2 - pos.crossLength * (.5f - pos.crossAnchor);
                } break;

                case AxisAlignment::End: {
                    crossPos = row->crossLength - pos.crossLength * (1.f - pos.crossAnchor);
                } break;
            }
            log::debug("axisPos: {}", axisPos);
            log::debug("crossPos: {}", crossPos);
            if (m_axis == Axis::Row) {
                node->setPosition(axisPos, crossPos);
            }
            else {
                node->setPosition(crossPos, axisPos);
            }
            rowAxisPos += pos.axisLength + m_gap * squish;
        }
    }
}

void AxisLayout::apply(CCNode* on) {
    auto nodes = getNodesToPosition(on);
    this->tryFitLayout(on, nodes, 1.f, 1.f);
}

AxisLayout::AxisLayout(Axis axis) : m_axis(axis) {}

AxisLayout* AxisLayout::setCrossAxisAlignment(AxisAlignment align) {
    m_crossAlignment = align;
    return this;
}

AxisLayout* AxisLayout::setAxisAlignment(AxisAlignment align) {
    m_axisAlignment = align;
    return this;
}

AxisLayout* AxisLayout::setGap(float gap) {
    m_gap = gap;
    return this;
}

AxisLayout* AxisLayout::setAxisReverse(bool reverse) {
    m_axisReverse = reverse;
    return this;
}

AxisLayout* AxisLayout::setCrossAxisReverse(bool reverse) {
    m_crossReverse = reverse;
    return this;
}

AxisLayout* AxisLayout::setCrossAxisOverflow(bool fit) {
    m_allowCrossAxisOverflow = fit;
    return this;
}

AxisLayout* AxisLayout::setAutoScale(bool scale) {
    m_autoScale = scale;
    return this;
}

AxisLayout* AxisLayout::setGrowCrossAxis(bool shrink) {
    m_growCrossAxis = shrink;
    return this;
}

RowLayout::RowLayout() : AxisLayout(Axis::Row) {}

RowLayout* RowLayout::create() {
    return new RowLayout();
}

ColumnLayout::ColumnLayout() : AxisLayout(Axis::Column) {}

ColumnLayout* ColumnLayout::create() {
    return new ColumnLayout();
}
