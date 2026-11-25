#pragma once

#include <Geode/ui/General.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/ui/Toggler.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/ui/TextArea.hpp>
#include <Geode/ui/IconButtonSprite.hpp>
#include "ModListItem.hpp"
#include "../sources/ModListSource.hpp"
#include <server/DownloadManager.hpp>
#include <variant>

using namespace geode::prelude;

struct ModListErrorStatus {};
struct ModListUnkProgressStatus {};
using ModListStatus = std::variant<ModListErrorStatus, ModListUnkProgressStatus>;

struct InstalledModsUpdateCheck final {
    std::vector<Mod*> modsWithUpdates;
    std::vector<Mod*> modsWithDeprecations;
};

class ModList : public CCNode {
protected:
    ModListSource* m_source;
    size_t m_page = 0;
    ScrollLayer* m_list;
    CCNode* m_statusContainer;
    Label* m_statusTitle;
    SimpleTextArea* m_statusDetails;
    Button* m_statusDetailsBtn;
    CCNode* m_statusLoadingCircle;
    ListenerHandle m_pageLoadHandle;
    async::TaskHolder<Result<ModListSource::ProvidedMods, ModListSource::LoadPageError>> m_listener;
    Button* m_pagePrevBtn;
    Button* m_pageNextBtn;
    CCNode* m_topContainer;
    CCNode* m_searchMenu;
    CCNode* m_updateAllContainer = nullptr;
    CCLayerGradient* m_updateAllBG = nullptr;
    CCNode* m_updateAllMenu = nullptr;
    Button* m_updateAllBtn = nullptr;
    CCNode* m_updateAllLoadingCircle = nullptr;
    Toggler* m_toggleUpdatesOnlyBtn = nullptr;
    CCNode* m_errorsContainer = nullptr;
    Toggler* m_toggleErrorsOnlyBtn = nullptr;
    Label* m_updateCountLabel = nullptr;
    TextInput* m_searchInput;
    Button* m_filtersBtn;
    Button* m_clearFiltersBtn;
    ListenerHandle m_invalidateCacheHandle;
    ListenerHandle m_checkUpdatesHandle;
    ListenerHandle m_downloadHandle;
    async::TaskHolder<server::ServerResult<InstalledModsUpdateCheck>> m_checkUpdatesListener;
    ModListDisplay m_display = ModListDisplay::SmallList;
    bool m_exiting = false;
    bool m_hasDeprecations = false;
    std::atomic<size_t> m_searchInputThreads = 0;

    bool init(ModListSource* src, CCSize const& size, bool searchingDev);

    void updateTopContainer();
    void onCheckUpdates(InstalledModsUpdateCheck const& mods);
    void onInvalidateCache(ModListSource* source);

    void onPromise(ModListSource::PageLoadResult event);
    void onPage(int8_t direction);

public:
    static ModList* create(ModListSource* src, CCSize const& size, bool searchingDev = false);

    size_t getPage() const;

    void reloadPage();
    void gotoPage(size_t page, bool update = false);
    void showStatus(ModListStatus status, std::string message, std::optional<std::string> details = std::nullopt);

    void updateState();
    void updateDisplay(ModListDisplay display);
    ModListDisplay getDisplay();
    void activateSearch(bool activate);
    void setIsExiting(bool exiting);
};
