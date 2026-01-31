#include <Geode/utils/cocos.hpp>

class BaseAxisLayoutImpl {
public:
    geode::Axis m_axis;
    float m_gap;
    bool m_ignoreInvisibleChildren = true;

    BaseAxisLayoutImpl(geode::Axis axis, float gap) : m_axis(axis), m_gap(gap) {}

    cocos2d::CCArray* getNodesToPosition(cocos2d::CCNode* on) const {
        auto arr = cocos2d::CCArray::create();
        for (auto child : geode::cocos::CCArrayExt<cocos2d::CCNode*>(on->getChildren())) {
            if (!m_ignoreInvisibleChildren || child->isVisible()) {
                arr->addObject(child);
            }
        }
        return arr;
    }
};
