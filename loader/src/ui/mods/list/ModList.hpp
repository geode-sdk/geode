#pragma once

#include <Geode/ui/General.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/ui/TextArea.hpp>
#include <Geode/ui/TextInput.hpp>
#include "ModItem.hpp"
#include "../sources/ModListSource.hpp"

using namespace geode::prelude;

struct ModListErrorStatus {};
struct ModListUnkProgressStatus {};
struct ModListProgressStatus {
    uint8_t percentage;
};
using ModListStatus = std::variant<ModListErrorStatus, ModListUnkProgressStatus, ModListProgressStatus>;

class ModList : public CCNode {
protected:
    Ref<ModListSource> m_source;
    size_t m_page = 0;
    ScrollLayer* m_list;
    CCMenu* m_statusContainer;
    CCLabelBMFont* m_statusTitle;
    SimpleTextArea* m_statusDetails;
    CCMenuItemSpriteExtra* m_statusDetailsBtn;
    CCSprite* m_statusLoadingCircle;
    Slider* m_statusLoadingBar;
    ModListSource::PageLoadEventListener m_listener;
    CCMenuItemSpriteExtra* m_pagePrevBtn;
    CCMenuItemSpriteExtra* m_pageNextBtn;
    Ref<CCNode> m_searchMenu;
    TextInput* m_searchInput;
    MiniFunction<void()> m_updateParentState = nullptr;
    bool m_bigSize = false;
    std::atomic<size_t> m_searchInputThreads = 0;

    bool init(ModListSource* src, CCSize const& size);

    void onPromise(ModListSource::PageLoadEvent* event);
    void onPage(CCObject*);
    void onShowStatusDetails(CCObject*);
    void onFilters(CCObject*);
    void onClearFilters(CCObject*);

public:
    static ModList* create(ModListSource* src, CCSize const& size);

    // poor man's delegate
    void onUpdateParentState(MiniFunction<void()> listener);
    size_t getPage() const;

    void reloadPage();
    void gotoPage(size_t page, bool update = false);
    void showStatus(ModListStatus status, std::string const& message, std::optional<std::string> const& details = std::nullopt);

    void updatePageNumber();
    void updateSize(bool big);
    void activateSearch(bool activate);
};
