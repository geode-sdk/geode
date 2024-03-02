#include <cocos2d.h>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>

using namespace geode::prelude;

// if 5k iterations isn't enough to fit the layout, then something is wrong
static size_t RECURSION_DEPTH_LIMIT = 5000;

static AxisLayoutOptions const* axisOpts(CCNode* node) {
    if (!node) return nullptr;
    return typeinfo_cast<AxisLayoutOptions*>(node->getLayoutOptions());
}

static bool isOptsBreakLine(AxisLayoutOptions const* opts) {
    if (opts) {
        return opts->getBreakLine();
    }
    return false;
}

static bool isOptsSameLine(AxisLayoutOptions const* opts) {
    if (opts) {
        return opts->getSameLine();
    }
    return false;
}

static int optsScalePrio(AxisLayoutOptions const* opts) {
    if (opts) {
        return opts->getScalePriority();
    }
    return AXISLAYOUT_DEFAULT_PRIORITY;
}

static float optsMinScale(AxisLayoutOptions const* opts, float defaultMinScale) {
    if (opts && opts->hasExplicitMinScale()) {
        return opts->getMinScale();
    }
    return defaultMinScale;
}

static float optsMaxScale(AxisLayoutOptions const* opts, float defaultMaxScale) {
    if (opts && opts->hasExplicitMaxScale()) {
        return opts->getMaxScale();
    }
    return defaultMaxScale;
}

static float optsRelScale(AxisLayoutOptions const* opts) {
    if (opts) {
        return opts->getRelativeScale();
    }
    return 1.f;
}

static float scaleByOpts(
    AxisLayoutOptions const* opts,
    float scale, int prio, bool squishMode,
    float defaultMinScale, float defaultMaxScale
) {
    if (prio > optsScalePrio(opts)) {
        return optsMaxScale(opts, defaultMaxScale) * optsRelScale(opts);
    }
    // otherwise if it matches scale it down by the factor
    else if (!squishMode && prio == optsScalePrio(opts)) {
        auto trueScale = scale;
        auto min = optsMinScale(opts, defaultMinScale);
        auto max = optsMaxScale(opts, defaultMaxScale);
        if (trueScale < min) {
            trueScale = min;
        }
        if (trueScale > max) {
            trueScale = max;
        }
        return trueScale * optsRelScale(opts);
    }
    // otherwise it's been scaled down to minimum
    else {
        return optsMinScale(opts, defaultMinScale) * optsRelScale(opts);
    }
}

struct AxisPosition {
    float axisLength;
    float axisAnchor;
    float crossLength;
    float crossAnchor;
};

static AxisPosition nodeAxis(CCNode* node, Axis axis, float scale) {
    auto scaledSize = node->getScaledContentSize() * scale;
    std::optional<float> axisLength = std::nullopt;
    if (auto opts = axisOpts(node)) {
        axisLength = opts->getLength();
    }
    // CCMenuItemToggler is a common quirky class
    if (auto toggle = typeinfo_cast<CCMenuItemToggler*>(node)) {
        scaledSize = toggle->m_offButton->getScaledContentSize();
    }
    auto anchor = node->getAnchorPoint();
    if (axis == Axis::Row) {
        return AxisPosition {
            .axisLength = axisLength.value_or(scaledSize.width),
            .axisAnchor = anchor.x,
            .crossLength = scaledSize.height,
            .crossAnchor = anchor.y,
        };
    }
    else {
        return AxisPosition {
            .axisLength = axisLength.value_or(scaledSize.height),
            .axisAnchor = anchor.y,
            .crossLength = scaledSize.width,
            .crossAnchor = anchor.x,
        };
    }
}

class AxisLayout::Impl {
public:
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
    std::optional<float> m_autoGrowAxisMinLength;
    std::pair<float, float> m_defaultScaleLimits = { AXISLAYOUT_DEFAULT_MIN_SCALE, 1 };

    struct Row : public CCObject {
        float nextOverflowScaleDownFactor;
        float nextOverflowSquishFactor;
        float axisLength;
        float crossLength;
        float axisEndsLength;

        // all layout calculations happen within a single frame so no Ref needed
        CCArray* nodes;

        // calculated values for scale, squish and prio to fit the nodes in this 
        // row when positioning
        float scale;
        float squish;
        float prio;

        Row(
            float scaleFactor,
            float squishFactor,
            float axisLength,
            float crossLength,
            float axisEndsLength,
            CCArray* nodes,
            float scale,
            float squish,
            float prio
        ) : nextOverflowScaleDownFactor(scaleFactor),
            nextOverflowSquishFactor(squishFactor),
            axisLength(axisLength),
            crossLength(crossLength),
            axisEndsLength(axisEndsLength),
            nodes(nodes),
            scale(scale),
            squish(squish),
            prio(prio)
        {
            this->autorelease();
        }

        void accountSpacers(Axis axis, float availableLength, float crossLength) {
            std::vector<SpacerNode*> spacers;
            for (auto& node : CCArrayExt<CCNode*>(nodes)) {
                if (auto spacer = typeinfo_cast<SpacerNode*>(node)) {
                    spacers.push_back(spacer);
                }
            }
            if (spacers.size()) {
                auto unusedSpace = availableLength - this->axisLength;
                size_t sum = 0;
                for (auto& spacer : spacers) {
                    sum += spacer->getGrow();
                }
                for (auto& spacer : spacers) {
                    auto size = unusedSpace * spacer->getGrow() / static_cast<float>(sum);
                    if (axis == Axis::Row) {
                        spacer->setContentSize({ size, crossLength });
                    }
                    else {
                        spacer->setContentSize({ crossLength, size });
                    }
                }
                this->axisLength = availableLength;
            }
        }
    };
    
    float minScaleForPrio(CCArray* nodes, int prio) const {
        float min = m_defaultScaleLimits.first;
        bool first = true;
        for (auto node : CCArrayExt<CCNode*>(nodes)) {
            auto scale = optsMinScale(axisOpts(node), m_defaultScaleLimits.first);
            if (first) {
                min = scale;
                first = false;
            }
            else if (scale < min) {
                min = scale;
            }
        }
        return min;
    }

    float maxScaleForPrio(CCArray* nodes, int prio) const {
        float max = m_defaultScaleLimits.second;
        bool first = true;
        for (auto node : CCArrayExt<CCNode*>(nodes)) {
            auto scale = optsMaxScale(axisOpts(node), m_defaultScaleLimits.second);
            if (first) {
                max = scale;
                first = false;
            }
            else if (scale > max) {
                max = scale;
            }
        }
        return max;
    }

    bool shouldAutoScale(AxisLayoutOptions const* opts) const {
        if (opts) {
            return opts->getAutoScale().value_or(m_autoScale);
        }
        else {
            return m_autoScale;
        }
    }

    bool canTryScalingDown(
        CCArray* nodes,
        int& prio, float& scale,
        float crossScaleDownFactor,
        std::pair<int, int> const& minMaxPrios
    ) const {
        bool attemptRescale = false;
        auto minScaleForPrio = this->minScaleForPrio(nodes, prio);
        if (
            // if the scale is less than the lowest min scale allowed, then 
            // trying to scale will have no effect and not help anywmore
            crossScaleDownFactor < minScaleForPrio ||
            // if the scale down factor is really close to the same as before, 
            // then we've entered an infinite loop (float == float is unreliable)
            (fabsf(crossScaleDownFactor - scale) < .001f)
        ) {
            // is there still some lower priority nodes we could try scaling?
            if (prio > minMaxPrios.first) {
                while (true) {
                    prio -= 1;
                    auto mscale = this->maxScaleForPrio(nodes, prio);
                    if (!mscale) {
                        continue;
                    }
                    scale = mscale;
                    break;
                }
                attemptRescale = true;
            }
            // otherwise set scale to min and squish
            else {
                scale = minScaleForPrio;
            }
        }
        // otherwise scale as usual
        else {
            attemptRescale = true;
            scale = crossScaleDownFactor;
        }
        return attemptRescale;
    }

    float nextGap(AxisLayoutOptions const* now, AxisLayoutOptions const* next) const {
        std::optional<float> gap;
        if (now) {
            gap = now->getNextGap();
        }
        if (next && (!gap || gap.value() < next->getPrevGap())) {
            gap = next->getPrevGap();
        }
        return gap.value_or(m_gap);
    }

    Row* fitInRow(
        CCNode* on, CCArray* nodes,
        std::pair<int, int> const& minMaxPrios,
        bool doAutoScale,
        float scale, float squish, int prio
    ) const {
        float nextAxisScalableLength;
        float nextAxisUnscalableLength;
        float axisUnsquishedLength;
        float axisLength;
        float crossLength;
        auto res = CCArray::create();

        auto available = nodeAxis(on, m_axis, 1.f / on->getScale());

        auto fit = [&](CCArray* nodes) {
            nextAxisScalableLength = 0.f;
            nextAxisUnscalableLength = 0.f;
            axisUnsquishedLength = 0.f;
            axisLength = 0.f;
            crossLength = 0.f;
            AxisLayoutOptions const* prev = nullptr;
            size_t ix = 0;
            for (auto& node : CCArrayExt<CCNode*>(nodes)) {
                auto opts = axisOpts(node);
                if (this->shouldAutoScale(opts)) {
                    node->setScale(1.f);
                }
                auto nodeScale = scaleByOpts(opts, scale, prio, false, m_defaultScaleLimits.first, m_defaultScaleLimits.second);
                auto pos = nodeAxis(node, m_axis, nodeScale * squish);
                auto squishPos = nodeAxis(node, m_axis, scaleByOpts(opts, scale, prio, true, m_defaultScaleLimits.first, m_defaultScaleLimits.second));
                if (prio == optsScalePrio(opts)) {
                    nextAxisScalableLength += pos.axisLength;
                }
                else {
                    nextAxisUnscalableLength += pos.axisLength;
                }
                // if multiple rows are allowed and this row is full, time for the 
                // next row
                // also force at least one object to be added to this row, because if 
                // it's too large for this row it's gonna be too large for all rows
                if (
                    m_growCrossAxis && (
                        (nextAxisScalableLength + nextAxisUnscalableLength > available.axisLength) && 
                        ix != 0 && !isOptsSameLine(opts)
                    )
                ) {
                    break;
                }
                if (nodes != res) {
                    res->addObject(node);
                }
                if (ix) {
                    auto gap = nextGap(prev, opts);
                    // if we've exhausted all priority scale options, scale gap too
                    if (prio == minMaxPrios.first) {
                        nextAxisScalableLength += gap * scale * squish;
                        axisLength += gap * scale * squish;
                        axisUnsquishedLength += gap * scale;
                    }
                    else {
                        nextAxisUnscalableLength += gap * squish;
                        axisLength += gap * squish;
                        axisUnsquishedLength += gap;
                    }
                }
                axisLength += pos.axisLength;
                axisUnsquishedLength += squishPos.axisLength;
                // squishing doesn't affect cross length, that's done separately
                if (pos.crossLength / squish > crossLength) {
                    crossLength = pos.crossLength / squish;
                }
                prev = opts;
                if (m_growCrossAxis && isOptsBreakLine(opts)) {
                    break;
                }
                ix++;
            }
        };

        fit(nodes);

        // whoops! removing objects from a CCArray while iterating is totes potes UB
        for (int i = 0; i < res->count(); i++) {
            nodes->removeFirstObject();
        }

        // todo: make this calculation more smart to avoid so much unnecessary recursion
        auto scaleDownFactor = scale - .002f;
        auto squishFactor = available.axisLength / (axisUnsquishedLength + .01f) * squish;

        // calculate row scale, squish, and prio
        int tries = 1000;
        while (axisLength > available.axisLength) {
            if (this->canTryScalingDown(res, prio, scale, scale - .002f, minMaxPrios)) {
                scale -= .002f;
            }
            else {
                squish = available.axisLength / axisUnsquishedLength;
            }
            fit(res);
            // Avoid infinite loops
            if (tries-- <= 0) {
                break;
            }
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
                first->getScaledContentSize().width * 
                    scaleByOpts(axisOpts(first), scale, prio, false, m_defaultScaleLimits.first, m_defaultScaleLimits.second) / 2 +
                last->getScaledContentSize().width * 
                    scaleByOpts(axisOpts(last), scale, prio, false, m_defaultScaleLimits.first, m_defaultScaleLimits.second) / 2
            );
        }

        return new Row(
            // how much should the nodes be scaled down to fit the next row
            // the .01f is because floating point arithmetic is imprecise and you 
            // end up in a situation where it confidently tells you that
            // 241 > 241 == true
            scaleDownFactor,
            // how much should the nodes be squished to fit the next item in this 
            // row
            squishFactor,
            axisLength, crossLength, axisEndsLength,
            res,
            scale, squish, prio
        );
    }

    void tryFitLayout(
        CCNode* on, CCArray* nodes,
        std::pair<int, int> const& minMaxPrios,
        bool doAutoScale,
        float scale, float squish, int prio,
        size_t depth
    ) const {
        // where do all of these magical calculations come from?
        // idk i got tired of doing the math but they work so ¯\_(ツ)_/¯ 
        // like i genuinely have no clue fr why some of these work tho, 
        // i just threw in random equations and numbers until it worked

        auto rows = CCArray::create();
        float maxRowAxisLength = 0.f;
        float totalRowCrossLength = 0.f;
        float crossScaleDownFactor = 0.f;
        float crossSquishFactor = 0.f;

        // make spacers have zero size so they don't affect spacing calculations
        for (auto& node : CCArrayExt<CCNode*>(nodes)) {
            if (auto spacer = typeinfo_cast<SpacerNode*>(node)) {
                spacer->setContentSize(CCSizeZero);
            }
        }
        
        // fit everything into rows while possible
        size_t ix = 0;
        auto newNodes = nodes->shallowCopy();
        while (newNodes->count()) {
            auto row = this->fitInRow(
                on, newNodes,
                minMaxPrios, doAutoScale,
                scale, squish, prio
            );
            rows->addObject(row);
            if (
                row->nextOverflowScaleDownFactor > crossScaleDownFactor &&
                row->nextOverflowScaleDownFactor < scale
            ) {
                crossScaleDownFactor = row->nextOverflowScaleDownFactor;
            }
            if (
                row->nextOverflowSquishFactor > crossSquishFactor &&
                row->nextOverflowSquishFactor < squish
            ) {
                crossSquishFactor = row->nextOverflowSquishFactor;
            }
            totalRowCrossLength += row->crossLength;
            if (ix) {
                totalRowCrossLength += m_gap;
            }
            if (row->axisLength > maxRowAxisLength) {
                maxRowAxisLength = row->axisLength;
            }
            ix++;
        }
        newNodes->release();

        if (!rows->count()) {
            return;
        }

        auto available = nodeAxis(on, m_axis, 1.f / on->getScale());
        if (available.axisLength <= 0.f) {
            return;
        }

        // if cross axis overflow not allowed and it's overflowing, try to scale 
        // down layout if there are any nodes with auto-scale enabled (or 
        // auto-scale is enabled by default)
        if (
            !m_allowCrossAxisOverflow && 
            doAutoScale && 
            totalRowCrossLength > available.crossLength && 
            depth < RECURSION_DEPTH_LIMIT
        ) {
            if (this->canTryScalingDown(nodes, prio, scale, crossScaleDownFactor, minMaxPrios)) {
                rows->release();
                return this->tryFitLayout(
                    on, nodes,
                    minMaxPrios, doAutoScale,
                    scale, squish, prio,
                    depth + 1
                );
            }
        }

        // if we're still overflowing, squeeze nodes closer together
        if (
            !m_allowCrossAxisOverflow &&
            totalRowCrossLength > available.crossLength && 
            depth < RECURSION_DEPTH_LIMIT
        ) {
            // if squishing rows would take less squishing that squishing columns, 
            // then squish rows
            if (
                !m_growCrossAxis ||
                totalRowCrossLength / available.crossLength < crossSquishFactor
            ) {
                rows->release();
                return this->tryFitLayout(
                    on, nodes,
                    minMaxPrios, doAutoScale,
                    scale, crossSquishFactor, prio,
                    depth + 1
                );
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
            row->accountSpacers(m_axis, available.axisLength, available.crossLength);

            if (m_crossAlignment == AxisAlignment::Even) {
                rowCrossPos -= rowEvenSpace / 2 + row->crossLength / 2;
            }
            else {
                rowCrossPos -= row->crossLength * columnSquish;
            }

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

            float evenSpace = available.axisLength / row->nodes->count();

            size_t ix = 0;
            AxisLayoutOptions const* prev = nullptr;
            for (auto& node : CCArrayExt<CCNode*>(row->nodes)) {
                auto opts = axisOpts(node);
                // rescale node if overflowing
                if (this->shouldAutoScale(opts)) {
                    auto nodeScale = scaleByOpts(opts, row->scale, row->prio, false, m_defaultScaleLimits.first, m_defaultScaleLimits.second);
                    // CCMenuItemSpriteExtra is quirky af
                    if (auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(node)) {
                        btn->m_baseScale = nodeScale;
                    }
                    node->setScale(nodeScale);
                }
                if (!ix) {
                    rowAxisPos += row->axisEndsLength * row->scale / 2 * (1.f - row->squish);
                }
                auto pos = nodeAxis(node, m_axis, row->squish);
                float axisPos;
                if (m_axisAlignment == AxisAlignment::Even) {
                    axisPos = rowAxisPos + evenSpace / 2 - pos.axisLength * (.5f - pos.axisAnchor);
                    rowAxisPos += evenSpace - 
                        row->axisEndsLength * row->scale * (1.f - row->squish) * 1.f / nodes->count();
                }
                else {
                    if (ix) {
                        if (row->prio == minMaxPrios.first) {
                            rowAxisPos += this->nextGap(prev, opts) * row->scale * row->squish;
                        }
                        else {
                            rowAxisPos += this->nextGap(prev, opts) * row->squish;
                        }
                    }
                    axisPos = rowAxisPos + pos.axisLength * pos.axisAnchor;
                    rowAxisPos += pos.axisLength - 
                        row->axisEndsLength * row->scale * (1.f - row->squish) * 1.f / nodes->count();
                }
                float crossOffset;
                switch (m_crossLineAlignment) {
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
                prev = opts;
                ix++;
            }
        
            if (m_crossAlignment == AxisAlignment::Even) {
                rowCrossPos -= rowEvenSpace / 2 - row->crossLength / 2 - 
                    rowsEndsLength * 1.5f * row->scale * (1.f - columnSquish) * 1.f / rows->count();
            }
            else {
                rowCrossPos -= m_gap * columnSquish - 
                    rowsEndsLength * 1.5f * row->scale * (1.f - columnSquish) * 1.f / rows->count();
            }
        }
    }
};

void AxisLayout::apply(CCNode* on) {
    auto nodes = getNodesToPosition(on);
    
    std::pair<int, int> minMaxPrio;
    bool doAutoScale = false;

    float totalLength = 0;
    AxisLayoutOptions const* prev = nullptr;

    bool first = true;
    for (auto node : CCArrayExt<CCNode*>(nodes)) {
        // Require all nodes not to have this stupid option enabled because it 
        // screws up all position calculations
        node->ignoreAnchorPointForPosition(false);
        int prio = 0;
        auto opts = axisOpts(node);
        if (opts) {
            prio = opts->getScalePriority();
            // this does cause a recheck of m_autoScale every iteration but it 
            // should be pretty fast and this correctly handles the situation 
            // where auto-scale is enabled on the layout but explicitly 
            // disabled on all its children
            if (opts->getAutoScale().value_or(m_impl->m_autoScale)) {
                doAutoScale = true;
            }
        }
        else {
            if (m_impl->m_autoScale) {
                doAutoScale = true;
            }
        }
        if (first) {
            minMaxPrio = { prio, prio };
            first = false;
        }
        else {
            if (prio < minMaxPrio.first) {
                minMaxPrio.first = prio;
            }
            if (prio > minMaxPrio.second) {
                minMaxPrio.second = prio;
            }
        }
        if (m_impl->m_autoGrowAxisMinLength.has_value()) {
            totalLength += nodeAxis(node, m_impl->m_axis, 1.f).axisLength + m_impl->nextGap(prev, opts);
            prev = opts;
        }
    }

    if (m_impl->m_autoGrowAxisMinLength.has_value()) {
        if (totalLength < m_impl->m_autoGrowAxisMinLength.value()) {
            totalLength = m_impl->m_autoGrowAxisMinLength.value();
        }
        if (m_impl->m_axis == Axis::Row) {
            on->setContentSize({ totalLength, on->getContentSize().height });
        }
        else {
            on->setContentSize({ on->getContentSize().width, totalLength });
        }
    }

    m_impl->tryFitLayout(
        on, nodes,
        minMaxPrio, doAutoScale,
        m_impl->maxScaleForPrio(nodes, minMaxPrio.second), 1.f, minMaxPrio.second,
        0
    );
}

CCSize AxisLayout::getSizeHint(CCNode* on) const {
    // Ideal is single row / column with no scaling
    auto nodes = getNodesToPosition(on);
    float length = 0.f;
    float cross = 0.f;
    for (auto& node : CCArrayExt<CCNode*>(nodes)) {
        auto axis = nodeAxis(node, m_impl->m_axis, 1.f);
        length += axis.axisLength;
        if (axis.crossLength > cross) {
            axis.crossLength = cross;
        }
    }
    if (!m_impl->m_allowCrossAxisOverflow) {
        cross = nodeAxis(on, m_impl->m_axis, 1.f).crossLength;
    }
    if (m_impl->m_axis == Axis::Row) {
        return { length, cross };
    }
    else {
        return { cross, length };
    }
}

Axis AxisLayout::getAxis() const {
    return m_impl->m_axis;
}
AxisAlignment AxisLayout::getCrossAxisAlignment() const {
    return m_impl->m_crossAlignment;
}
AxisAlignment AxisLayout::getCrossAxisLineAlignment() const {
    return m_impl->m_crossLineAlignment;
}
AxisAlignment AxisLayout::getAxisAlignment() const {
    return m_impl->m_axisAlignment;
}
float AxisLayout::getGap() const {
    return m_impl->m_gap;
}
bool AxisLayout::getAxisReverse() const {
    return m_impl->m_axisReverse;
}
bool AxisLayout::getCrossAxisReverse() const {
    return m_impl->m_crossReverse;
}
bool AxisLayout::getAutoScale() const {
    return m_impl->m_autoScale;
}
bool AxisLayout::getGrowCrossAxis() const {
    return m_impl->m_growCrossAxis;
}
bool AxisLayout::getCrossAxisOverflow() const {
    return m_impl->m_allowCrossAxisOverflow;
}
std::optional<float> AxisLayout::getAutoGrowAxis() const {
    return m_impl->m_autoGrowAxisMinLength;
}
float AxisLayout::getDefaultMinScale() const {
    return m_impl->m_defaultScaleLimits.first;
}
float AxisLayout::getDefaultMaxScale() const {
    return m_impl->m_defaultScaleLimits.second;
}

AxisLayout* AxisLayout::setAxis(Axis axis) {
    m_impl->m_axis = axis;
    return this;
}
AxisLayout* AxisLayout::setCrossAxisAlignment(AxisAlignment align) {
    m_impl->m_crossAlignment = align;
    return this;
}
AxisLayout* AxisLayout::setCrossAxisLineAlignment(AxisAlignment align) {
    m_impl->m_crossLineAlignment = align;
    return this;
}
AxisLayout* AxisLayout::setAxisAlignment(AxisAlignment align) {
    m_impl->m_axisAlignment = align;
    return this;
}
AxisLayout* AxisLayout::setGap(float gap) {
    m_impl->m_gap = gap;
    return this;
}
AxisLayout* AxisLayout::setAxisReverse(bool reverse) {
    m_impl->m_axisReverse = reverse;
    return this;
}
AxisLayout* AxisLayout::setCrossAxisReverse(bool reverse) {
    m_impl->m_crossReverse = reverse;
    return this;
}
AxisLayout* AxisLayout::setCrossAxisOverflow(bool fit) {
    m_impl->m_allowCrossAxisOverflow = fit;
    return this;
}
AxisLayout* AxisLayout::setAutoScale(bool scale) {
    m_impl->m_autoScale = scale;
    return this;
}
AxisLayout* AxisLayout::setGrowCrossAxis(bool shrink) {
    m_impl->m_growCrossAxis = shrink;
    return this;
}
AxisLayout* AxisLayout::setAutoGrowAxis(std::optional<float> allowAndMinLength) {
    m_impl->m_autoGrowAxisMinLength = allowAndMinLength;
    return this;
}
AxisLayout* AxisLayout::setDefaultScaleLimits(float min, float max) {
    m_impl->m_defaultScaleLimits = { min, max };
    return this;
}

AxisLayout::AxisLayout(Axis axis) : m_impl(std::make_unique<Impl>()) {
    m_impl->m_axis = axis;
}
AxisLayout::~AxisLayout() {}

AxisLayout* AxisLayout::create(Axis axis) {
    auto ret = new AxisLayout(axis);
    ret->autorelease();
    return ret;
}

// RowLayout

RowLayout::RowLayout() : AxisLayout(Axis::Row) {}

RowLayout* RowLayout::create() {
    auto ret = new RowLayout();
    ret->autorelease();
    return ret;
}

// ColumnLayout

ColumnLayout::ColumnLayout() : AxisLayout(Axis::Column) {}

ColumnLayout* ColumnLayout::create() {
    auto ret = new ColumnLayout();
    ret->autorelease();
    return ret;
}

// AxisLayoutOptions

class AxisLayoutOptions::Impl {
public:
    std::optional<bool> m_autoScale = std::nullopt;
    std::pair<std::optional<float>, std::optional<float>> m_scaleLimits;
    float m_relativeScale = 1.f;
    std::optional<float> m_length = std::nullopt;
    std::optional<float> m_nextGap = std::nullopt;
    std::optional<float> m_prevGap = std::nullopt;
    bool m_breakLine = false;
    bool m_sameLine = false;
    int m_scalePriority = AXISLAYOUT_DEFAULT_PRIORITY;
};

AxisLayoutOptions* AxisLayoutOptions::create() {
    auto ret = new AxisLayoutOptions();
    ret->autorelease();
    return ret;
}

AxisLayoutOptions::AxisLayoutOptions() : m_impl(std::make_unique<Impl>()) {}
AxisLayoutOptions::~AxisLayoutOptions() = default;

std::optional<bool> AxisLayoutOptions::getAutoScale() const {
    return m_impl->m_autoScale;
}
float AxisLayoutOptions::getMaxScale() const {
    return m_impl->m_scaleLimits.second.value_or(1.f);
}
float AxisLayoutOptions::getMinScale() const {
    return m_impl->m_scaleLimits.first.value_or(AXISLAYOUT_DEFAULT_MIN_SCALE);
}
bool AxisLayoutOptions::hasExplicitMaxScale() const {
    return m_impl->m_scaleLimits.second.has_value();
}
bool AxisLayoutOptions::hasExplicitMinScale() const {
    return m_impl->m_scaleLimits.first.has_value();
}
float AxisLayoutOptions::getRelativeScale() const {
    return m_impl->m_relativeScale;
}
std::optional<float> AxisLayoutOptions::getLength() const {
    return m_impl->m_length;
}
std::optional<float> AxisLayoutOptions::getPrevGap() const {
    return m_impl->m_prevGap;
}
std::optional<float> AxisLayoutOptions::getNextGap() const {
    return m_impl->m_nextGap;
}
bool AxisLayoutOptions::getBreakLine() const {
    return m_impl->m_breakLine;
}
bool AxisLayoutOptions::getSameLine() const {
    return m_impl->m_sameLine;
}
int AxisLayoutOptions::getScalePriority() const {
    return m_impl->m_scalePriority;
}

AxisLayoutOptions* AxisLayoutOptions::setMaxScale(float scale) {
    m_impl->m_scaleLimits.second = scale;
    return this;
}
AxisLayoutOptions* AxisLayoutOptions::setMinScale(float scale) {
    m_impl->m_scaleLimits.first = scale;
    return this;
}
AxisLayoutOptions* AxisLayoutOptions::setScaleLimits(std::optional<float> min, std::optional<float> max) {
    m_impl->m_scaleLimits = { min, max };
    return this;
}
AxisLayoutOptions* AxisLayoutOptions::setRelativeScale(float scale) {
    m_impl->m_relativeScale = scale;
    return this;
}
AxisLayoutOptions* AxisLayoutOptions::setAutoScale(std::optional<bool> enabled) {
    m_impl->m_autoScale = enabled;
    return this;
}
AxisLayoutOptions* AxisLayoutOptions::setLength(std::optional<float> length) {
    m_impl->m_length = length;
    return this;
}
AxisLayoutOptions* AxisLayoutOptions::setPrevGap(std::optional<float> gap) {
    m_impl->m_prevGap = gap;
    return this;
}
AxisLayoutOptions* AxisLayoutOptions::setNextGap(std::optional<float> gap) {
    m_impl->m_nextGap = gap;
    return this;
}
AxisLayoutOptions* AxisLayoutOptions::setBreakLine(bool enable) {
    m_impl->m_breakLine = enable;
    return this;
}
AxisLayoutOptions* AxisLayoutOptions::setSameLine(bool enable) {
    m_impl->m_sameLine = enable;
    return this;
}
AxisLayoutOptions* AxisLayoutOptions::setScalePriority(int priority) {
    m_impl->m_scalePriority = priority;
    return this;
}
