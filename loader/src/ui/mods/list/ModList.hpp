#pragma once

#include <Geode/ui/General.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/ui/TextArea.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/ui/IconButtonSprite.hpp>
#include <Geode/binding/TextArea.hpp>
#include "ModItem.hpp"
#include "../sources/ModListSource.hpp"
#include <server/DownloadManager.hpp>

using namespace geode::prelude;

struct ModListErrorStatus {};
struct ModListUnkProgressStatus {};
struct ModListProgressStatus {
    uint8_t percentage;
};
using ModListStatus = std::variant<ModListErrorStatus, ModListUnkProgressStatus, ModListProgressStatus>;

class ModList : public CCNode {
protected:
    ModListSource* m_source;
    size_t m_page = 0;
    ScrollLayer* m_list;
    CCMenu* m_statusContainer;
    CCLabelBMFont* m_statusTitle;
    SimpleTextArea* m_statusDetails;
    CCMenuItemSpriteExtra* m_statusDetailsBtn;
    CCNode* m_statusLoadingCircle;
    Slider* m_statusLoadingBar;
    EventListener<ModListSource::PageLoadTask> m_listener;
    CCMenuItemSpriteExtra* m_pagePrevBtn;
    CCMenuItemSpriteExtra* m_pageNextBtn;
    CCNode* m_topContainer;
    CCNode* m_searchMenu;
    CCNode* m_updateAllContainer = nullptr;
    CCMenu* m_updateAllMenu = nullptr;
    Ref<IconButtonSprite> m_updateAllSpr = nullptr;
    CCMenuItemSpriteExtra* m_updateAllBtn = nullptr;
    CCNode* m_updateAllLoadingCircle = nullptr;
    IconButtonSprite* m_showUpdatesSpr = nullptr;
    IconButtonSprite* m_hideUpdatesSpr = nullptr;
    CCMenuItemToggler* m_toggleUpdatesOnlyBtn = nullptr;
    CCNode* m_errorsContainer = nullptr;
    CCMenuItemToggler* m_toggleErrorsOnlyBtn = nullptr;
    TextArea* m_updateCountLabel = nullptr;
    TextInput* m_searchInput;
    CCMenuItemSpriteExtra* m_filtersBtn;
    CCMenuItemSpriteExtra* m_clearFiltersBtn;
    EventListener<InvalidateCacheFilter> m_invalidateCacheListener;
    EventListener<server::ServerRequest<std::vector<std::string>>> m_checkUpdatesListener;
    EventListener<server::ModDownloadFilter> m_downloadListener;
    ModListDisplay m_display = ModListDisplay::SmallList;
    bool m_exiting = false;
    std::atomic<size_t> m_searchInputThreads = 0;

    bool init(ModListSource* src, CCSize const& size);

    void updateTopContainer();
    void onCheckUpdates(typename server::ServerRequest<std::vector<std::string>>::Event* event);
    void onInvalidateCache(InvalidateCacheEvent* event);

    void onPromise(ModListSource::PageLoadTask::Event* event);
    void onPage(CCObject*);
    void onShowStatusDetails(CCObject*);
    void onFilters(CCObject*);
    void onSort(CCObject*);
    void onClearFilters(CCObject*);
    void onToggleUpdates(CCObject*);
    void onToggleErrors(CCObject*);
    void onUpdateAll(CCObject*);
    void onEventInfo(CCObject*);

public:
    static ModList* create(ModListSource* src, CCSize const& size);

    size_t getPage() const;

    void reloadPage();
    void gotoPage(size_t page, bool update = false);
    void showStatus(ModListStatus status, std::string const& message, std::optional<std::string> const& details = std::nullopt);

    void updateState();
    void updateDisplay(ModListDisplay display);
    ModListDisplay getDisplay();
    void activateSearch(bool activate);
    void setIsExiting(bool exiting);
};
