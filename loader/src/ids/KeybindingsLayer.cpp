#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/binding/KeybindingsLayer.hpp>
#include <Geode/cocos/extensions/GUI/CCControlExtension/CCScale9Sprite.h>
#include <Geode/modify/IDManager.hpp>
#include <Geode/utils/NodeIDs.hpp>
#include <unordered_map>

using namespace geode::prelude;
using namespace geode::node_ids;

$register_ids(KeybindingsLayer) {
    m_buttonMenu->setID("button-menu");
    m_searchBG->setID("search-bg");
    m_searchInput->setID("search-input");
    setIDSafe<CCScale9Sprite>(m_mainLayer, 0, "border");
    setIDSafe<CCScale9Sprite>(m_mainLayer, 1, "bg");

    setIDSafe<CCMenuItemSpriteExtra>(m_buttonMenu, 0, "close-button");
    // TODO: maybe these should go in TabbedPopupLayer?
    setIDSafe<CCMenuItemSpriteExtra>(m_buttonMenu, 8, "page-left-button");
    setIDSafe<CCMenuItemSpriteExtra>(m_buttonMenu, 9, "page-right-button");
    setIDSafe<CCMenuItemSpriteExtra>(m_buttonMenu, 10, "search-clear-button");
    setIDSafe<CCMenuItemSpriteExtra>(m_buttonMenu, 11, "settings-button");

    static std::unordered_map<int, std::string> tabIds = {
        {0, "gameplay-tab"},
        {1, "practice-tab"},
        {2, "editor-tab"},
        {3, "tools-tab"},
        {4, "view-tab"},
        {m_searchTab, "search-tab"},
        {6, "mods-tab"},
    };

    for (size_t i = 0; i < m_tabs.size(); i++) {
        if (auto it = tabIds.find(i); it != tabIds.end()) {
            m_tabs[i].m_button->setID(it->second);
        }
    }
}
