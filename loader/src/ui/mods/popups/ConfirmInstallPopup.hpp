#pragma once

#include <Geode/ui/Popup.hpp>
#include <server/DownloadManager.hpp>
#include "../GeodeStyle.hpp"

using namespace geode::prelude;

class ConfirmInstallPopup : public GeodePopup<> {
protected:
    CCNode* m_installations;
    CCNode* m_dependencies;
    CCNode* m_incompatabilities;
    EventListener<server::ModDownloadFilter> m_downloadListener;

    bool setup() override;

    void updateState();

public:
    static ConfirmInstallPopup* create();
    static void askForCustomize();
};
