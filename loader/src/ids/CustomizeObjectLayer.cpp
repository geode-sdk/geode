#include "AddIDs.hpp"

#include <Geode/modify/CustomizeObjectLayer.hpp>

using namespace geode::prelude;

$register_ids(CustomizeObjectLayer) {
    auto winSize = CCDirector::get()->getWinSize();

    setIDs(
        m_mainLayer, 0,
        "alert-bg",
        "title-label",
        "button-menu",
        "selected-channel-label",
        "channel-input-bg",
        "channel-input",
        "text-input-bg",
        "text-input",
        "glow-label"
    );

    m_buttonMenu->setID("ok-menu");

    setIDs(
        m_buttonMenu,
        0,
        "ok-button",
        "info-button",
        // 2
        "base-tab-button",
        "detail-tab-button",
        "base-hsv-button",
        "detail-hsv-button",
        "text-tab-button",
        // 7
        "player-color-1-button",
        "player-color-2-button",
        "light-bg-button",
        "default-button",
        "channel-1-button",
        "channel-2-button",
        "channel-3-button",
        "channel-4-button",
        "channel-5-button",
        "channel-6-button",
        "channel-7-button",
        "channel-8-button",
        "channel-9-button",
        "channel-custom-button",
        // 21
        "select-channel-up-button",
        "select-channel-down-button",
        "select-channel-button",
        // 24
        "split-text-button",
        "clear-text-button",
        // 26
        "next-free-button"
        // 27
    );

    if (m_targetObject) {
        setIDs(
            m_buttonMenu,
            27,
            "copy-button",
            "paste-button",

            "browse-button",

            "glow-toggle"
        );
    }
    else {
        setIDs(
            m_buttonMenu,
            27,
            "browse-button",

            "glow-toggle"
        );
    }

    auto tabsLayout = RowLayout::create()
        ->setAxisAlignment(AxisAlignment::Center)
        ->setCrossAxisAlignment(AxisAlignment::Center);
    tabsLayout->ignoreInvisibleChildren(true);
    auto tabsMenu = detachAndCreateMenu(
        m_mainLayer,
        "tabs-menu",
        tabsLayout,
        m_buttonMenu->getChildByID("base-tab-button"),
        m_buttonMenu->getChildByID("detail-tab-button"),
        SpacerNode::create(),
        m_buttonMenu->getChildByID("text-tab-button")
    );
    tabsMenu->setContentSize({ 360.f, 50.f });
    tabsMenu->setPositionX(winSize.width / 2);
    tabsMenu->updateLayout();

    auto specialChannelsMenu = detachAndCreateMenu(
        m_mainLayer,
        "special-channels-menu",
        RowLayout::create()
            ->setCrossAxisOverflow(false)
            ->setGrowCrossAxis(true)
            ->setAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisAlignment(AxisAlignment::Center)
            ->setGap(14.f),
        m_buttonMenu->getChildByID("player-color-1-button"),
        m_buttonMenu->getChildByID("player-color-2-button"),
        m_buttonMenu->getChildByID("light-bg-button"),
        m_buttonMenu->getChildByID("default-button")
    );
    specialChannelsMenu->setContentSize({ 350.f, 50.f });
    specialChannelsMenu->setPositionX(winSize.width / 2);
    specialChannelsMenu->updateLayout();

    auto channelsMenu = detachAndCreateMenu(
        m_mainLayer,
        "channels-menu",
        RowLayout::create()
            ->setCrossAxisOverflow(false)
            ->setGrowCrossAxis(true)
            ->setAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisAlignment(AxisAlignment::Center)
            ->setGap(20.f),
        m_buttonMenu->getChildByID("channel-1-button"),
        m_buttonMenu->getChildByID("channel-2-button"),
        m_buttonMenu->getChildByID("channel-3-button"),
        m_buttonMenu->getChildByID("channel-4-button"),
        m_buttonMenu->getChildByID("channel-5-button"),
        m_buttonMenu->getChildByID("channel-6-button"),
        m_buttonMenu->getChildByID("channel-7-button"),
        m_buttonMenu->getChildByID("channel-8-button"),
        m_buttonMenu->getChildByID("channel-9-button"),
        m_buttonMenu->getChildByID("channel-custom-button")
    );
    channelsMenu->setContentSize({ 350.f, 100.f });
    channelsMenu->setPosition(winSize.width / 2, winSize.height / 2 - 25.f);
    channelsMenu->updateLayout();

    auto selectedChannelMenu = detachAndCreateMenu(
        m_mainLayer,
        "selected-channel-menu",
        nullptr,
        m_buttonMenu->getChildByID("select-channel-button"),
        m_mainLayer->getChildByID("selected-channel-label")
    );
    selectedChannelMenu->setContentSize({ 120.f, 40.f });
    selectedChannelMenu->setPosition(
        winSize.width / 2 + 110.f,
        winSize.height / 2 - 90.f
    );
    if (auto label = selectedChannelMenu->getChildByID("selected-channel-label")) {
        label->setPosition(75.f, 20.f);
    }
    if (auto button = selectedChannelMenu->getChildByID("select-channel-button")) {
        button->setPosition(100.f, 20.f);
    }

    auto togglesMenu = detachAndCreateMenu(
        m_mainLayer,
        "toggles-menu",
        nullptr,
        m_mainLayer->getChildByID("glow-label"),
        m_buttonMenu->getChildByID("glow-toggle")
    );
    togglesMenu->setContentSize({ 75.f, 120.f });
    togglesMenu->setPosition(55.f, 90.f);
    if (auto label = togglesMenu->getChildByID("glow-label")) {
        label->setPosition(57.f, 20.f);
    }
    if (auto toggle = togglesMenu->getChildByID("glow-toggle")) {
        toggle->setPosition(57.f, 5.f);
    }

    auto baseHSVMenu = detachAndCreateMenu(
        m_mainLayer,
        "base-hsv-menu",
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start),
        m_buttonMenu->getChildByID("base-hsv-button")
    );
    baseHSVMenu->setContentSize({ 80.f, 60.f });
    baseHSVMenu->setPositionX(winSize.width / 2 - 132.5f);
    baseHSVMenu->updateLayout();

    auto detailHSVMenu = detachAndCreateMenu(
        m_mainLayer,
        "detail-hsv-menu",
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start),
        m_buttonMenu->getChildByID("detail-hsv-button")
    );
    detailHSVMenu->setContentSize({ 80.f, 60.f });
    detailHSVMenu->setPositionX(baseHSVMenu->getPositionX());
    detailHSVMenu->updateLayout();

    auto nextFreeMenu = detachAndCreateMenu(
        m_mainLayer,
        "next-free-menu",
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start),
        m_buttonMenu->getChildByID("next-free-button")
    );
    nextFreeMenu->setContentSize({ 120.f, 60.f });
    nextFreeMenu->setPositionX(winSize.width / 2 - 110.f);
    nextFreeMenu->updateLayout();

    auto textActionsMenu = detachAndCreateMenu(
        m_mainLayer,
        "text-actions-menu",
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::End)
            ->setAxisReverse(true),
        m_buttonMenu->getChildByID("split-text-button")
    );
    textActionsMenu->setContentSize({ 120.f, 60.f });
    textActionsMenu->setPositionX(winSize.width / 2 + 110.f);
    textActionsMenu->updateLayout();

    auto clearTextMenu = detachAndCreateMenu(
        m_mainLayer,
        "clear-text-menu",
        nullptr,
        m_buttonMenu->getChildByID("clear-text-button")
    );

    auto infoMenu = detachAndCreateMenu(
        m_mainLayer,
        "info-menu",
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::End)
            ->setAxisReverse(true),
        m_buttonMenu->getChildByID("info-button")
    );
    infoMenu->setContentSize({ 80.f, 60.f });
    infoMenu->setPosition(winSize.width / 2 + 132.5f, baseHSVMenu->getPositionY());
    infoMenu->updateLayout();

    auto browseMenu = detachAndCreateMenu(
        m_mainLayer,
        "browse-menu",
        ColumnLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAxisReverse(true),
        m_buttonMenu->getChildByID("browse-button")
    );
    browseMenu->setContentSize({ 100.f, 140.f });
    browseMenu->setPositionY(winSize.height / 2 - 70.f);
    browseMenu->updateLayout();

    auto copyPasteMenu = detachAndCreateMenu(
        m_mainLayer,
        "copy-paste-menu",
        ColumnLayout::create()
            ->setAxisAlignment(AxisAlignment::End)
            ->setAxisReverse(true),
        m_buttonMenu->getChildByID("copy-button"),
        m_buttonMenu->getChildByID("paste-button")
    );
    copyPasteMenu->setContentSize({ 100.f, 140.f });
    copyPasteMenu->setPositionY(winSize.height / 2 + 100.f);
    copyPasteMenu->updateLayout();

    auto selectChannelMenu = detachAndCreateMenu(
        m_mainLayer,
        "select-channel-menu",
        nullptr,
        m_buttonMenu->getChildByID("select-channel-up-button"),
        m_buttonMenu->getChildByID("select-channel-down-button")
    );
    selectChannelMenu->setPosition(m_customColorInputBG->getPosition());
    selectChannelMenu->setContentSize({ 60.f, 140.f });
    if (auto btn = selectChannelMenu->getChildByID("select-channel-up-button")) {
        btn->setPosition({ 30.f, 110.f });
    }
    if (auto btn = selectChannelMenu->getChildByID("select-channel-down-button")) {
        btn->setPosition({ 30.f, 30.f });
    }
}

struct CustomizeObjectLayerIDs : Modify<CustomizeObjectLayerIDs, CustomizeObjectLayer> {
    static void onModify(auto& self) {
        if (!self.setHookPriority("CustomizeObjectLayer::init", GEODE_ID_PRIORITY)) {
            log::warn("Failed to set CustomizeObjectLayer::init hook priority, node IDs may not work properly");
        }
    }

    void toggleMenuIf(const char* id, int mode) {
        if (auto menu = m_mainLayer->getChildByID(id)) {
            menu->setVisible(m_selectedMode == mode);
        }
    }

    void toggleMenuIfNot(const char* id, int mode) {
        if (auto menu = m_mainLayer->getChildByID(id)) {
            menu->setVisible(m_selectedMode != mode);
        }
    }

    void toggleVisible() {
        CustomizeObjectLayer::toggleVisible();
        // have to manually toggle menu visibility to allow touches being correctly passed
        this->toggleMenuIf("base-hsv-menu", 1);
        this->toggleMenuIf("detail-hsv-menu", 2);
        this->toggleMenuIf("text-actions-menu", 3);
        this->toggleMenuIf("clear-text-menu", 3);
        this->toggleMenuIfNot("next-free-menu", 3);
        this->toggleMenuIfNot("toggles-menu", 3);
        this->toggleMenuIfNot("channels-menu", 3);
        this->toggleMenuIfNot("special-channels-menu", 3);
        this->toggleMenuIfNot("selected-channel-menu", 3);
        this->toggleMenuIfNot("browse-menu", 3);
        this->toggleMenuIfNot("copy-paste-menu", 3);
        this->toggleMenuIfNot("select-channel-menu", 3);
    }

    bool init(GameObject* obj, CCArray* objs) {
        if (!CustomizeObjectLayer::init(obj, objs))
            return false;

        NodeIDs::get()->provide(this);
        this->toggleVisible();

        return true;
    }
};
