#include <Geode/ui/Layout.hpp>

using namespace geode::prelude;

class GridLayout::Impl {
public:
	GridLayout const* self;

	Axis m_expandAxis = Axis::Column;
	float m_gapX;
	float m_gapY;
	int m_maxCountCrossExpandAxis = 1;
	// bool m_autoExpand = true;
	bool m_autoAdjustSize = true;
	bool m_reverseExpandAxis;
	bool m_reverseCrossExpandAxis;

	// each inner vector represents a line cross expand axis
	std::vector<std::vector<CCNode*>> getGridOfNodes(CCNode* on) { 
		auto nodes = self->getNodesToPosition(on);

		std::vector<std::vector<CCNode*>> grid;
		// row if expands on the x, column if expands on the y.
		int lineOnExpandAxis = 0; 
		// column if expands on the x, row if expands on the y.
		// starts at -1 because its adding 1 right away
		int lineCrossExpandAxis = -1; 
		for (auto node : CCArrayExt<CCNode*>(nodes)) {
			if (lineOnExpandAxis % m_maxCountCrossExpandAxis == 0) { // need to add a new line
				lineCrossExpandAxis++;
				grid.push_back(std::vector<CCNode*>());
			}
			grid[lineCrossExpandAxis].push_back(node);
			lineOnExpandAxis++;
		}
		return grid;
	}
};

void GridLayout::apply(CCNode* on) {

	auto grid = m_impl->getGridOfNodes(on);
	if (grid.empty()) return;

	bool vertical = m_impl->m_expandAxis == Axis::Column;
	
	auto size = getSizeHint(on);
	if (m_impl->m_autoAdjustSize) vertical ? 
	on->setContentHeight(std::max(size.height, on->getContentHeight())) : 
	on->setContentWidth(std::max(size.width, on->getContentWidth()));


	float currentPosOnExpandAxis = (vertical ? grid[0][0]->getContentHeight() : grid[0][0]->getContentWidth()) / 2;
	if (m_impl->m_reverseExpandAxis) currentPosOnExpandAxis = (vertical ? size.height : size.width) - currentPosOnExpandAxis;
	int j = 0;
	for (auto line : grid) {
		float currentPosOnLine = (vertical ? line[0]->getContentWidth() : line[0]->getContentHeight()) / 2;
		float maxLengthCrossLine = 0.f;
		if (m_impl->m_reverseCrossExpandAxis) currentPosOnLine = (vertical ? size.width : size.height) - currentPosOnLine;

		int i = 0;
		for (auto node : line) {
			if (vertical) {
				node->setPosition({currentPosOnLine, currentPosOnExpandAxis});

				currentPosOnLine += node->getContentWidth() * (m_impl->m_reverseCrossExpandAxis ? -1 : 1);
				currentPosOnLine += m_impl->m_gapX * (m_impl->m_reverseCrossExpandAxis ? -1 : 1);

				float height = node->getContentHeight();
				if (height > maxLengthCrossLine) maxLengthCrossLine = height;
			} else {
				node->setPosition({currentPosOnExpandAxis, currentPosOnLine});

				currentPosOnLine += node->getContentHeight() * (m_impl->m_reverseCrossExpandAxis ? -1 : 1);
				currentPosOnLine += m_impl->m_gapY * (m_impl->m_reverseCrossExpandAxis ? -1 : 1);

				float width = node->getContentWidth();
				if (width > maxLengthCrossLine) maxLengthCrossLine = width;
			}

			i++;
		} 
		currentPosOnExpandAxis += maxLengthCrossLine * (m_impl->m_reverseExpandAxis ? -1 : 1);
		currentPosOnExpandAxis += (vertical ? m_impl->m_gapY : m_impl->m_gapX) * (m_impl->m_reverseExpandAxis ? -1 : 1);

		j++;
	} 
}

CCSize GridLayout::getSizeHint(CCNode* on) const {
	auto nodes = getNodesToPosition(on);

	if (nodes->count() == 0 /* || !m_impl->m_autoExpand */ ) {
		return on->getContentSize();
	} 

	bool vertical = m_impl->m_expandAxis == Axis::Column;

	auto grid = m_impl->getGridOfNodes(on);
	
	float totalLengthCrossExpandAxis = -(vertical ? m_impl->m_gapX : m_impl->m_gapY); // start with minus the gap once
	auto maxLengths = std::vector<float>(m_impl->m_maxCountCrossExpandAxis); 
	for (auto lineCrossExpandAxis : grid) {
		for (int i = 0; i < lineCrossExpandAxis.size(); i++) {
			auto node = lineCrossExpandAxis[i];
			auto length = vertical ? node->getContentWidth() : node->getContentHeight();
			if (length > maxLengths[i]) maxLengths[i] = length; // sets the max length for the respective line on the expand axis
		}
	}
	for (float length : maxLengths) {
		totalLengthCrossExpandAxis += length;
		totalLengthCrossExpandAxis += vertical ? m_impl->m_gapX : m_impl->m_gapY;
	}

	int lineNum = 0;

	float totalLengthOnExpandAxis = -(vertical ? m_impl->m_gapY : m_impl->m_gapX); // start with minus the gap once
	for (auto lineCrossExpandAxis : grid) {
		float maxLengthForLine = 0.f;
		for (auto node : lineCrossExpandAxis) {
			auto length = vertical ? node->getContentHeight() : node->getContentWidth();
			if (length > maxLengthForLine) maxLengthForLine = length;
		}
		totalLengthOnExpandAxis += vertical ? m_impl->m_gapY : m_impl->m_gapX;
		totalLengthOnExpandAxis += maxLengthForLine;
		lineNum++;
	}

	return vertical ? 
	CCSize(
		std::max(totalLengthCrossExpandAxis, on->getContentWidth()), 
		std::max(totalLengthOnExpandAxis, on->getContentHeight())
	) : 
	CCSize(
		std::max(totalLengthOnExpandAxis, on->getContentWidth()), 
		std::max(totalLengthCrossExpandAxis, on->getContentHeight())
	);
}

Axis GridLayout::getExpandAxis() const {
	return m_impl->m_expandAxis;
}
float GridLayout::getGapX() const {
	return m_impl->m_gapX;
}
float GridLayout::getGapY() const {
	return m_impl->m_gapY;
}
int GridLayout::getMaxCountCrossExpandAxis() const {
	return m_impl->m_maxCountCrossExpandAxis;
}
// bool GridLayout::doesExpand() const {
// 	return m_impl->m_autoExpand;
// }
bool GridLayout::isAutoAdjustSize() const {
	return m_impl->m_autoAdjustSize;
}
bool GridLayout::isReverseExpandAxis() const {
	return m_impl->m_reverseExpandAxis;
}
bool GridLayout::isReverseCrossExpandAxis() const {
	return m_impl->m_reverseCrossExpandAxis;
}

GridLayout* GridLayout::setExpandAxis(Axis axis) {
	m_impl->m_expandAxis = axis;
	return this;
}
GridLayout* GridLayout::setGapX(float gapX) {
	m_impl->m_gapX = gapX;
	return this;
}
GridLayout* GridLayout::setGapY(float gapY) {
	m_impl->m_gapY = gapY;
	return this;
}
GridLayout* GridLayout::setMaxCountCrossExpandAxis(int value) {
	m_impl->m_maxCountCrossExpandAxis = value;
	return this;
}
// bool GridLayout::expand(bool allow) const {
// 	m_impl->m_autoExpand = allow;
// }
GridLayout* GridLayout::autoAdjustSize(bool allow) {
	m_impl->m_autoAdjustSize = allow;
	return this;
}
GridLayout* GridLayout::reverseExpandAxis(bool reverse) {
	m_impl->m_reverseExpandAxis = reverse;
	return this;
}
GridLayout* GridLayout::reverseCrossExpandAxis(bool reverse) {
	m_impl->m_reverseCrossExpandAxis = reverse;
	return this;
} 

GridLayout::GridLayout() : m_impl(std::make_unique<Impl>()) {
	m_impl->self = this;
}
GridLayout::~GridLayout() {}

GridLayout* GridLayout::create() {
	auto ret = new GridLayout();
	ret->autorelease();
	return ret;
}