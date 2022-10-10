#include <cocos2d.h>
#include <Geode/utils/WackyGeodeMacros.hpp>

using namespace cocos2d;

void RowLayout::apply(CCArray* nodes, CCSize const& availableSize) {
    float totalWidth = .0f;
    CCARRAY_FOREACH_B_BASE(nodes, node, CCNode*, ix) {
        totalWidth += node->getScaledContentSize().width;
        if (ix) {
            totalWidth += m_gap;
        }
    }

    float pos;
    switch (m_alignment) {
        default:
        case Alignment::Center: pos = -totalWidth / 2; break;
        case Alignment::Begin: pos = -totalWidth; break;
        case Alignment::End: pos = 0.f; break;
    }
    CCARRAY_FOREACH_B_TYPE(nodes, node, CCNode) {
        auto sw = node->getScaledContentSize().width;
        node->setPositionX(pos + sw / 2);
        if (m_alignVertically) {
            node->setPositionY(m_alignVertically.value());
        }
        pos += sw + m_gap;
    }
}

RowLayout* RowLayout::create(
    float gap,
    std::optional<float> alignVertically
) {
    auto ret = new RowLayout;
    ret->m_gap = gap;
    ret->m_alignVertically = alignVertically;
    return ret;
}

RowLayout* RowLayout::setAlignment(Alignment align) {
    m_alignment = align;
    return this;
}

RowLayout* RowLayout::setGap(float gap) {
    m_gap = gap;
    return this;
}

RowLayout* RowLayout::setAlignVertically(std::optional<float> align) {
    m_alignVertically = align;
    return this;
}

void ColumnLayout::apply(CCArray* nodes, CCSize const& availableSize) {
    float totalHeight = .0f;
    CCARRAY_FOREACH_B_BASE(nodes, node, CCNode*, ix) {
        totalHeight += node->getScaledContentSize().height;
        if (ix) {
            totalHeight += m_gap;
        }
    }

    float pos;
    switch (m_alignment) {
        default:
        case Alignment::Center: pos = -totalHeight / 2; break;
        case Alignment::Begin: pos = -totalHeight; break;
        case Alignment::End: pos = 0.f; break;
    }
    CCARRAY_FOREACH_B_TYPE(nodes, node, CCNode) {
        auto sw = node->getScaledContentSize().height;
        node->setPositionY(pos + sw / 2);
        if (m_alignHorizontally) {
            node->setPositionX(m_alignHorizontally.value());
        }
        pos += sw + m_gap;
    }
}

ColumnLayout* ColumnLayout::create(
    float gap,
    std::optional<float> alignHorizontally
) {
    auto ret = new ColumnLayout;
    ret->m_gap = gap;
    ret->m_alignHorizontally = alignHorizontally;
    return ret;
}

ColumnLayout* ColumnLayout::setAlignment(Alignment align) {
    m_alignment = align;
    return this;
}

ColumnLayout* ColumnLayout::setGap(float gap) {
    m_gap = gap;
    return this;
}

ColumnLayout* ColumnLayout::setAlignHorizontally(std::optional<float> align) {
    m_alignHorizontally = align;
    return this;
}

void GridLayout::apply(CCArray* nodes, CCSize const& availableSize) {
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

