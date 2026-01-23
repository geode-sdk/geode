#pragma once

#include "../GeodeStyle.hpp"
#include "../sources/ModSource.hpp"
#include <server/Server.hpp>

class DevListPopup : public GeodePopup {
protected:
    ModSource m_source;
public:
    static DevListPopup* create(ModSource source);
    void onClose(cocos2d::CCObject*) override;
private:
    bool init(ModSource source);
    void createList();
    void onMoreByThisDev(CCObject* sender);
};
