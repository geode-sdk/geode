#pragma once

#include "../GeodeStyle.hpp"
#include "../sources/ModSource.hpp"
#include <server/Server.hpp>

class DevListPopup : public GeodePopup<ModMetadata const&> {
protected:
    ModMetadata m_meta;
public:
    static DevListPopup* create(ModMetadata const&);
    void onClose(cocos2d::CCObject*);
private:
    bool setup(ModMetadata const&);
    void createList();
    void onMoreByThisDev(CCObject* sender);
};
