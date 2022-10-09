#include "HookListLayer.hpp"

bool HookListLayer::init(Mod* mod) {
    if (!GJDropDownLayer::init("Hooks", 220.f))
        return false;
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto hooks = CCArray::create();
    for (auto const& hook : mod->getHooks()) {
        hooks->addObject(new HookItem(hook));
    }
    this->m_listLayer->m_listView = HookListView::create(hooks, mod, 356.f, 220.f);
    this->m_listLayer->addChild(this->m_listLayer->m_listView);

    return true;
}

HookListLayer* HookListLayer::create(Mod* mod) {
    auto ret = new HookListLayer;
    if (ret && ret->init(mod)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
