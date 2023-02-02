#include <cocos2d.h>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/loader/Log.hpp>

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

void RowLayout::apply(CCNode* on) {

    auto nodes = getNodesToPosition(on);
    if (m_reverse) {
        nodes->reverseObjects();
    }

    auto availableWidth = m_maxAutoScale.has_value() ?
        on->getScaledContentSize().width :
        on->getContentSize().width;

    size_t ix = 0;
    float totalWidth = .0f;
    for (auto& node : CCArrayExt<CCNode*>(nodes)) {
        // if no need to fit fully inside, figure out what part may overflow 
        // for first item
        if (ix == 0 && !m_fitInside) {
            totalWidth += node->getScaledContentSize().width
             * (1.f - node->getAnchorPoint().x);
        }
        // if no need to fit fully inside, figure out what part may overflow
        // for last item
        else if (ix == nodes->count() - 1 && !m_fitInside) {
            totalWidth += node->getScaledContentSize().width
             * node->getAnchorPoint().x;
        }
        // otherwise either we need to fit fully inside or this node is not 
        // at the start or end
        else {
            totalWidth += node->getScaledContentSize().width;
        }
        if (ix) {
            totalWidth += m_gap;
        }
        ix++;
    }

    auto squeeze = availableSize.width / totalWidth;
    if (squeeze > 1.f) {
        squeeze = 1.f;
    }

    float pos;
    switch (m_alignment) {
        default:
        case Alignment::Center: pos = -totalWidth / 2; break;
        case Alignment::Begin: pos = -totalWidth; break;
        case Alignment::End: pos = 0.f; break;
    }
    for (auto& node : CCArrayExt<CCNode*>(nodes)) {
        auto sw = node->getScaledContentSize().width;
        float disp;
        switch (m_alignment) {
            default:
            case Alignment::Center: disp = sw * node->getAnchorPoint().x; break;
            case Alignment::Begin:  disp = sw; break;
            case Alignment::End:    disp = 0.f; break;
        }
        node->setPositionX(pos + disp);
        if (m_alignVertically) {
            node->setPositionY(m_alignVertically.value());
        }
        pos += (sw + m_gap) * squeeze;
    }
}

RowLayout* RowLayout::create() {
    return new RowLayout();
}

RowLayout* RowLayout::setVAlignment(std::optional<Alignment> align) {
    m_vAlignment = align;
    return this;
}

RowLayout* RowLayout::setGap(float gap) {
    m_gap = gap;
    return this;
}

RowLayout* RowLayout::setReverse(bool reverse) {
    m_reverse = reverse;
    return this;
}

RowLayout* RowLayout::setMaxAutoScale(std::optional<float> scale) {
    m_maxAutoScale = scale;
    return this;
}

RowLayout* RowLayout::setFitInside(bool fit) {
    m_fitInside = fit;
    return this;
}

void ColumnLayout::apply(CCNode* on) {
    float totalHeight = .0f;
    size_t ix = 0;
    auto nodes = getNodesToPosition(on);
    auto availableSize = on->getScaledContentSize();
    for (auto& node : CCArrayExt<CCNode*>(nodes)) {
        totalHeight += node->getScaledContentSize().height;
        if (ix) {
            totalHeight += m_gap;
        }
    }

    auto squeeze = availableSize.height / totalHeight;
    if (squeeze > 1.f) {
        squeeze = 1.f;
    }
    if (totalHeight > availableSize.height) {
        totalHeight = availableSize.height;
    }

    float pos;
    switch (m_alignment) {
        default:
        case Alignment::Center: pos = -totalHeight / 2; break;
        case Alignment::Begin: pos = -totalHeight; break;
        case Alignment::End: pos = 0.f; break;
    }
    if (m_reverse) {
        nodes->reverseObjects();
    }
    log::debug("start pos: {}", pos);
    log::debug("squeeze: {}", squeeze);
    for (auto& node : CCArrayExt<CCNode*>(nodes)) {
        auto sh = node->getScaledContentSize().height;
        float disp;
        switch (m_alignment) {
            default:
            case Alignment::Center: disp = sh * node->getAnchorPoint().y; break;
            case Alignment::Begin:  disp = (m_reverse ? 0.f : sh); break;
            case Alignment::End:    disp = (m_reverse ? sh : 0.f); break;
        }
        log::debug("positioning at: {}", pos + disp);
        node->setPositionY(pos + disp);
        if (m_alignHorizontally) {
            node->setPositionX(m_alignHorizontally.value());
        }
        auto opos = pos;
        pos += (sh + m_gap) * squeeze;
        log::debug("pos: {} -> {}", opos, pos);
    }
}

ColumnLayout* ColumnLayout::create() {
    return new ColumnLayout();
}

ColumnLayout* ColumnLayout::setHAlignment(std::optional<Alignment> align) {
    m_hAlignment = align;
    return this;
}

ColumnLayout* ColumnLayout::setGap(float gap) {
    m_gap = gap;
    return this;
}

ColumnLayout* ColumnLayout::setReverse(bool reverse) {
    m_reverse = reverse;
    return this;
}

ColumnLayout* ColumnLayout::setMaxAutoScale(std::optional<float> scale) {
    m_maxAutoScale = scale;
    return this;
}

ColumnLayout* ColumnLayout::setFitInside(bool fit) {
    m_fitInside = fit;
    return this;
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

