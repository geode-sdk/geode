#pragma once

#include <Geode/ui/General.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/ui/TextArea.hpp>
#include "ModItem.hpp"
#include "ModListSource.hpp"

using namespace geode::prelude;

class ModList : public CCNode, public SetTextPopupDelegate {
protected:
    ModListSource* m_source;
    size_t m_page = 0;
    ScrollLayer* m_list;
    SimpleTextArea* m_statusText;
    CCSprite* m_statusLoadingCircle;
    ModListSource::PageLoadEventListener m_listener;
    CCMenuItemSpriteExtra* m_pagePrevBtn;
    CCMenuItemSpriteExtra* m_pageNextBtn;
    CCMenuItemSpriteExtra* m_pageLabelBtn;
    CCLabelBMFont* m_pageLabel;

    bool init(ModListSource* src, CCSize const& size);

    void onPromise(PromiseEvent<ModListSource::Page>* event);
    void onPage(CCObject*);
    void onGoToPage(CCObject*);

    void setTextPopupClosed(SetTextPopup*, gd::string value) override;

    void updatePageUI(bool hide = false);

public:
    static ModList* create(ModListSource* src, CCSize const& size);

    void reloadPage();
    void gotoPage(size_t page, bool update = false);
    void showStatus(std::string const& status, bool loading);
};

class ModsLayer : public CCLayer {
protected:
    CCNode* m_frame;
    std::vector<CCMenuItemSpriteExtra*> m_tabs;
    ModListSource* m_currentSource = nullptr;
    std::unordered_map<ModListSource*, Ref<ModList>> m_lists;

    bool init();

    void keyBackClicked() override;
    void onTab(CCObject* sender);

public:
    static ModsLayer* create();
    static ModsLayer* scene();

    void onBack(CCObject*);
    void onRefreshList(CCObject*);

    void gotoTab(ModListSource* src = nullptr);
};
