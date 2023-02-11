#include <cocos2d.h>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>

USE_GEODE_NAMESPACE();

#pragma warning(disable: 4273)

void CCNode::swapChildIndices(CCNode* first, CCNode* second) {
    m_pChildren->exchangeObject(first, second);
    std::swap(first->m_nZOrder, second->m_nZOrder);
    std::swap(first->m_uOrderOfArrival, second->m_uOrderOfArrival);
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
    float axisLength;
    float crossLength;
    float axisEndsLength;
    Ref<CCArray> nodes;

    Row(
        float scaleFactor,
        float axisLength,
        float crossLength,
        float axisEndsLength,
        CCArray* nodes
    ) : nextOverflowScaleDownFactor(scaleFactor),
        axisLength(axisLength),
        crossLength(crossLength),
        axisEndsLength(axisEndsLength),
        nodes(nodes)
    {
        this->autorelease();
    }
};

AxisLayout::Row* AxisLayout::fitInRow(CCNode* on, CCArray* nodes, float scale, float squish) const {
    float nextAxisLength = 0.f;
    float axisLength = 0.f;
    float crossLength = 0.f;
    auto res = CCArray::create();

    auto available = nodeAxis(on, m_axis, 1.f);
    size_t ix = 0;
    for (auto& node : CCArrayExt<CCNode*>(nodes)) {
        if (m_autoScale) {
            node->setScale(1);
        }
        auto pos = nodeAxis(node, m_axis, scale * squish);
        nextAxisLength += pos.axisLength;
        // if multiple rows are allowed and this row is full, time for the 
        // next row
        // also force at least one object to be added to this row, because if 
        // it's too large for this row it's gonna be too large for all rows
        if (m_growCrossAxis && nextAxisLength > available.axisLength && ix != 0) {
            break;
        }
        res->addObject(node);
        if (ix) {
            nextAxisLength += m_gap * scale * squish;
            axisLength += m_gap * scale * squish;
        }
        axisLength += pos.axisLength;
        if (pos.crossLength > crossLength) {
            crossLength = pos.crossLength;
        }
        ix++;
    }

    // whoops! removing objects from a CCArray while iterating is totes potes UB
    for (int i = 0; i < res->count(); i++) {
        nodes->removeFirstObject();
    }

    // reverse row if needed
    if (m_axisReverse) {
        res->reverseObjects();
    }

    float axisEndsLength = 0.f;
    if (res->count()) {
        auto first = static_cast<CCNode*>(res->firstObject());
        auto last = static_cast<CCNode*>(res->lastObject());
        axisEndsLength = (
            first->getScaledContentSize().width * scale / 2 +
            last->getScaledContentSize().width * scale / 2
        );
    }

    return new Row(
        // how much should the nodes be scaled down to fit the next row
        available.axisLength / nextAxisLength * scale * squish,
        axisLength,
        crossLength,
        axisEndsLength,
        res
    );
}

void AxisLayout::tryFitLayout(CCNode* on, CCArray* nodes, float scale, float squish) const {
    // where do all of these magical calculations come from?
    // idk i got tired of doing the math but they work so ¯\_(ツ)_/¯ 
    // like i genuinely have no clue fr why some of these work tho, 
    // i just threw in random equations and numbers until it worked

    auto rows = CCArray::create();
    float totalRowCrossLength = 0.f;
    float crossScaleDownFactor = 0.f;

    // fit everything into rows while possible
    size_t ix = 0;
    auto newNodes = nodes->shallowCopy();
    while (newNodes->count()) {
        auto row = this->fitInRow(on, newNodes, scale, squish);
        rows->addObject(row);
        if (
            row->nextOverflowScaleDownFactor > crossScaleDownFactor &&
            row->nextOverflowScaleDownFactor < scale
        ) {
            crossScaleDownFactor = row->nextOverflowScaleDownFactor;
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
        if (m_autoScale && crossScaleDownFactor > AXIS_MIN_SCALE) {
            rows->release();
            return this->tryFitLayout(on, nodes, crossScaleDownFactor, squish);
        }
    }

    // if we're still overflowing, squeeze nodes closer together
    if (!m_allowCrossAxisOverflow && totalRowCrossLength > available.crossLength) {
        // if squishing rows would take less squishing that squishing columns, 
        // then squish rows
        if (totalRowCrossLength / available.crossLength < crossScaleDownFactor) {
            rows->release();
            return this->tryFitLayout(on, nodes, scale, crossScaleDownFactor);
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

    float columnSquish = 1.f;
    if (!m_allowCrossAxisOverflow && totalRowCrossLength > available.crossLength) {
        columnSquish = available.crossLength / totalRowCrossLength;
        totalRowCrossLength *= columnSquish;
    }

    float rowsEndsLength = 0.f;
    if (rows->count()) {
        auto first = static_cast<Row*>(rows->firstObject());
        auto last = static_cast<Row*>(rows->lastObject());
        rowsEndsLength = first->crossLength / 2 + last->crossLength / 2;
    }

    float rowCrossPos;
    switch (m_crossAlignment) {
        case AxisAlignment::Start: {
            rowCrossPos = totalRowCrossLength - rowsEndsLength * 1.5f * scale * (1.f - columnSquish);
        } break;

        case AxisAlignment::Even: {
            totalRowCrossLength = available.crossLength;
            rowCrossPos = totalRowCrossLength - rowsEndsLength * 1.5f * scale * (1.f - columnSquish);
        } break;

        case AxisAlignment::Center: {
            rowCrossPos = available.crossLength / 2 + totalRowCrossLength / 2 - 
                rowsEndsLength * 1.5f * scale * (1.f - columnSquish);
        } break;

        case AxisAlignment::End: {
            rowCrossPos = available.crossLength - 
                rowsEndsLength * 1.5f * scale * (1.f - columnSquish);
        } break;
    }

    float rowEvenSpace = available.crossLength / rows->count();

    for (auto row : CCArrayExt<Row*>(rows)) {
        if (m_crossAlignment == AxisAlignment::Even) {
            rowCrossPos -= rowEvenSpace / 2 + row->crossLength / 2;
        }
        else {
            rowCrossPos -= row->crossLength * columnSquish;
        }

        // scale down & squish row if it overflows main axis
        float rowScale = scale;
        float rowSquish = squish;
        if (row->axisLength > available.axisLength) {
            row->axisLength /= scale * squish;
            if (m_autoScale) {
                rowScale = available.axisLength / row->axisLength;
                if (rowScale < AXIS_MIN_SCALE) {
                    rowScale = AXIS_MIN_SCALE;
                }
                row->axisLength *= rowScale;
            }
            // squishing needs to take into account the row ends
            if (row->axisLength > available.axisLength) {
                rowSquish = available.axisLength / row->axisLength;
            }
            row->axisLength *= rowSquish;
        }

        float rowAxisPos;
        switch (m_axisAlignment) {
            case AxisAlignment::Start: { 
                rowAxisPos = row->axisEndsLength * rowScale / 2 * (1.f - rowSquish);
            } break;

            case AxisAlignment::Even: { 
                rowAxisPos = row->axisEndsLength * rowScale / 2 * (1.f - rowSquish);
            } break;

            case AxisAlignment::Center: {
                rowAxisPos = available.axisLength / 2 - row->axisLength / 2 + 
                    row->axisEndsLength * rowScale / 2 * (1.f - rowSquish);
            } break;

            case AxisAlignment::End: {
                rowAxisPos = available.axisLength - row->axisLength + 
                    row->axisEndsLength * rowScale / 2 * (1.f - rowSquish);
            } break;
        }

        float evenSpace = available.axisLength / row->nodes->count();

        size_t ix = 0;
        for (auto& node : CCArrayExt<CCNode*>(row->nodes)) {
            // rescale node if overflowing
            if (m_autoScale) {
                // CCMenuItemSpriteExtra is quirky af
                if (auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(node)) {
                    btn->m_baseScale = rowScale;
                }
                node->setScale(rowScale);
            }
            auto pos = nodeAxis(node, m_axis, rowSquish);
            float axisPos;
            if (m_axisAlignment == AxisAlignment::Even) {
                axisPos = rowAxisPos + evenSpace / 2 - pos.axisLength * (.5f - pos.axisAnchor);
                rowAxisPos += evenSpace - 
                    row->axisEndsLength * rowScale * (1.f - rowSquish) * 1.f / nodes->count();
            }
            else {
                axisPos = rowAxisPos + pos.axisLength * pos.axisAnchor;
                rowAxisPos += pos.axisLength + m_gap * rowScale * rowSquish - 
                    row->axisEndsLength * rowScale * (1.f - rowSquish) * 1.f / nodes->count();
            }
            float crossOffset;
            switch (m_crossAlignment) {
                case AxisAlignment::Start: {
                    crossOffset = pos.crossLength * pos.crossAnchor;
                } break;

                case AxisAlignment::Center: case AxisAlignment::Even: {
                    crossOffset = row->crossLength / 2 - pos.crossLength * (.5f - pos.crossAnchor);
                } break;

                case AxisAlignment::End: {
                    crossOffset = row->crossLength - pos.crossLength * (1.f - pos.crossAnchor);
                } break;
            }
            if (m_axis == Axis::Row) {
                node->setPosition(axisPos, rowCrossPos + crossOffset);
            }
            else {
                node->setPosition(rowCrossPos + crossOffset, axisPos);
            }
            ix++;
        }
    
        if (m_crossAlignment == AxisAlignment::Even) {
            rowCrossPos -= rowEvenSpace / 2 - row->crossLength / 2 - 
                rowsEndsLength * 1.5f * scale * (1.f - columnSquish) * 1.f / rows->count();
        }
        else {
            rowCrossPos -= m_gap * columnSquish - 
                rowsEndsLength * 1.5f * scale * (1.f - columnSquish) * 1.f / rows->count();
        }
    }
}

void AxisLayout::apply(CCNode* on) {
    auto nodes = getNodesToPosition(on);
    this->tryFitLayout(on, nodes, 1.f, 1.f);
}

AxisLayout::AxisLayout(Axis axis) : m_axis(axis) {}

Axis AxisLayout::getAxis() const {
    return m_axis;
}

AxisAlignment AxisLayout::getCrossAxisAlignment() const {
    return m_crossAlignment;
}

AxisAlignment AxisLayout::getAxisAlignment() const {
    return m_axisAlignment;
}

float AxisLayout::getGap() const {
    return m_gap;
}

bool AxisLayout::getAxisReverse() const {
    return m_axisReverse;
}

bool AxisLayout::getCrossAxisReverse() const {
    return m_crossReverse;
}

bool AxisLayout::getAutoScale() const {
    return m_autoScale;
}

bool AxisLayout::getGrowCrossAxis() const {
    return m_growCrossAxis;
}

bool AxisLayout::getCrossAxisOverflow() const {
    return m_allowCrossAxisOverflow;
}

AxisLayout* AxisLayout::setAxis(Axis axis) {
    m_axis = axis;
    return this;
}

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

AxisLayout* AxisLayout::create(Axis axis) {
    return new AxisLayout(axis);
}

RowLayout::RowLayout() : AxisLayout(Axis::Row) {}

RowLayout* RowLayout::create() {
    return new RowLayout();
}

ColumnLayout::ColumnLayout() : AxisLayout(Axis::Column) {}

ColumnLayout* ColumnLayout::create() {
    return new ColumnLayout();
}
