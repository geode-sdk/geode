#include "HookListView.hpp"

#include <Geode/binding/StatsCell.hpp>
#include <Geode/binding/TableView.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/loader/Mod.hpp>

HookCell::HookCell(char const* name, CCSize size) : TableViewCell(name, size.width, size.height) {}

void HookCell::draw() {
    reinterpret_cast<StatsCell*>(this)->StatsCell::draw();
}

void HookCell::updateBGColor(int index) {
    if (index & 1) m_backgroundLayer->setColor(ccc3(0xc2, 0x72, 0x3e));
    else m_backgroundLayer->setColor(ccc3(0xa1, 0x58, 0x2c));
    m_backgroundLayer->setOpacity(0xff);
}

void HookCell::onEnable(CCObject* pSender) {
    auto toggle = as<CCMenuItemToggler*>(pSender);
    if (!toggle->isToggled()) {
        auto res = m_mod->enableHook(m_hook);
        if (!res) {
            FLAlertLayer::create(
                nullptr, "Error Enabling Hook", std::string(res.unwrapErr()), "OK", nullptr, 280.f
            )
                ->show();
        }
    }
    else {
        auto res = m_mod->disableHook(m_hook);
        if (!res) {
            FLAlertLayer::create(
                nullptr, "Error Disabling Hook", std::string(res.unwrapErr()), "OK", nullptr, 280.f
            )
                ->show();
        }
    }
    toggle->toggle(!m_hook->isEnabled());
}

void HookCell::loadFromHook(Hook* hook, Mod* Mod) {
    m_hook = hook;
    m_mod = Mod;

    m_mainLayer->setVisible(true);
    m_backgroundLayer->setOpacity(255);

    auto menu = CCMenu::create();
    menu->setPosition(m_width - m_height, m_height / 2);
    m_mainLayer->addChild(menu);

    auto enableBtn =
        CCMenuItemToggler::createWithStandardSprites(this, menu_selector(HookCell::onEnable), .6f);
    enableBtn->setPosition(0, 0);
    enableBtn->toggle(hook->isEnabled());
    menu->addChild(enableBtn);

    std::stringstream moduleName;

    // #ifdef GEODE_IS_WINDOWS // add other platforms?
    // HMODULE module;
    // if (GetModuleHandleExA(
    //     GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
    //     GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
    //     as<LPCSTR>(hook->getAddress()),
    //     &module
    // )) {
    //     addr -= as<uintptr_t>(module);
    //     char name[MAX_PATH];
    //     if (GetModuleFileNameA(
    //         module, name, sizeof name
    //     )) {
    //         auto fileName = std::filesystem::path(name).stem();
    //         moduleName << fileName.string() << " + ";
    //     }
    // }
    // #endif
    if (hook->getDisplayName() != "") moduleName << hook->getDisplayName();
    else moduleName << "0x" << std::hex << hook->getAddress();
    auto label = CCLabelBMFont::create(moduleName.str().c_str(), "chatFont.fnt");
    label->setPosition(m_height / 2, m_height / 2);
    label->setScale(.7f);
    label->setAnchorPoint({ .0f, .5f });
    m_mainLayer->addChild(label);
}

HookCell* HookCell::create(char const* key, CCSize size) {
    auto pRet = new HookCell(key, size);
    if (pRet) {
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void HookListView::setupList() {
    m_itemSeparation = 30.0f;

    if (!m_entries->count()) return;

    m_tableView->reloadData();

    if (m_entries->count() == 1)
        m_tableView->moveToTopWithOffset(m_itemSeparation);

    m_tableView->moveToTop();
}

TableViewCell* HookListView::getListCell(char const* key) {
    return HookCell::create(key, CCSize { m_width, m_itemSeparation });
}

void HookListView::loadCell(TableViewCell* cell, unsigned int index) {
    as<HookCell*>(cell)->loadFromHook(
        as<HookItem*>(m_entries->objectAtIndex(index))->m_hook, m_mod
    );
    as<HookCell*>(cell)->updateBGColor(index);
}

HookListView* HookListView::create(CCArray* hooks, Mod* Mod, float width, float height) {
    auto pRet = new HookListView;
    if (pRet) {
        pRet->m_mod = Mod;
        if (pRet->init(hooks, kBoomListType_Hooks, width, height)) {
            pRet->autorelease();
            return pRet;
        }
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}
