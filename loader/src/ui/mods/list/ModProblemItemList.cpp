#include "ModProblemItemList.hpp"
#include "ModProblemItem.hpp"

bool ModProblemItemList::init(float height) {
    if (!CCNode::init())
        return false;
    
    this->setContentSize({ 250, height });

    m_scrollLayer = ScrollLayer::create({ m_obContentSize.width, height });
    for (auto problem : Loader::get()->getProblems()) {
        m_scrollLayer->m_contentLayer->addChild(ModProblemItem::parse(problem));
    }
    m_scrollLayer->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setAutoGrowAxis(height)
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
    );
    this->addChildAtPosition(m_scrollLayer, Anchor::BottomLeft);

    return true;
}

ModProblemItemList* ModProblemItemList::create(float height) {
    auto ret = new ModProblemItemList();
    if (ret && ret->init(height)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
