#include <algorithm>
#include <memory>
#include <numeric>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <fmt/format.h>

#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/GameSettingsLayer.hpp>
#include <Geode/binding/GJKeybindManager.hpp>
#include <Geode/binding/KeybindingsLayer.hpp>
#include <Geode/binding/KeybindSource.hpp>
#include <Geode/binding/LevelBindingsLayer.hpp>
#include <Geode/binding/TabbedPopupLayer.hpp>
#include <Geode/binding/ScrollableContentLayer.hpp>
#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/cocos/CCDirector.h>
#include <Geode/cocos/cocoa/CCGeometry.h>
#include <Geode/cocos/include/ccTypes.h>
#include <Geode/cocos/menu_nodes/CCMenu.h>
#include <Geode/cocos/touch_dispatcher/CCTouchDispatcher.h>
#include <Geode/cocos/touch_dispatcher/CCTouchHandler.h>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <Geode/modify/GJKeybindManager.hpp>
#include <Geode/modify/KeybindingsLayer.hpp>
#include <Geode/modify/TabbedPopupLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/ui/Layout.hpp>
#include <Geode/ui/NineSlice.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/ui/SimpleAxisLayout.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/function.hpp>
#include <Geode/utils/Keyboard.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/ZStringView.hpp>

#include "LoaderImpl.hpp"
#include "ui/mods/GeodeStyle.hpp"

using namespace geode::prelude;

static int vanillaTabFor(KeybindingsLayer* layer, int id) {
    static std::unordered_map<int, int> s_actionTab;

    if (s_actionTab.empty()) {
        for (int tab = 0; tab < layer->m_searchTab; tab++) {
            for (int action : layer->KeybindingsLayer::actionsToShow(tab)) {
                s_actionTab.try_emplace(action, tab);
            }
        }
    }

    auto it = s_actionTab.find(id);
    return it != s_actionTab.end() ? it->second : -1;
}

static Layout* listColumnLayout(float gap, bool ignoreInvisible = true) {
    return SimpleColumnLayout::create()
        ->setMainAxisDirection(AxisDirection::TopToBottom)
        ->setMainAxisAlignment(MainAxisAlignment::End)
        ->setMainAxisScaling(AxisScaling::Fit)
        ->setGap(gap)
        ->ignoreInvisibleChildren(ignoreInvisible);
}


#ifndef GEODE_IS_IOS

struct GeodeKeybindSource : public KeybindSource {
    virtual std::vector<int> chordsForAction(int action) override {
        auto setting = LoaderImpl::get()->keybindSettingForActionId(action);

        if (setting == nullptr) {
            return GJKeybindManager::sharedState()->chordsForAction(action);
        }

        std::vector<int> result;
        result.reserve(setting->getValue().size());

        for (auto& keybind : setting->getValue()) {
            if (auto chord = keybind.toVanillaChord(); chord.has_value()) {
                result.push_back(*chord);
            }
        }

        return result;
    }

    virtual std::vector<int> actionsForChord(int chord) override {
        auto result = GJKeybindManager::sharedState()->actionsForChord(chord);

        auto loaderImpl = LoaderImpl::get();

        for (auto& [id, setting] : loaderImpl->m_actionIdToKeybindSetting) {
            auto mod = setting->getMod();

            if (mod == nullptr || !mod->isLoaded()) {
                continue;
            }

            if (std::ranges::contains(setting->getValue(), chord, &Keybind::toVanillaChord)) {
                result.push_back(id);
            }
        }

        return result;
    }

    virtual void bindChord(int chord, int action) override {
        auto loaderImpl = LoaderImpl::get();

        if (auto setting = loaderImpl->keybindSettingForActionId(action); setting != nullptr) {
            auto keybind = Keybind::fromVanillaChord(chord);
            std::vector<Keybind> keybinds = setting->getValue();

            if (!std::ranges::contains(keybinds, chord, &Keybind::toVanillaChord)) {
                keybinds.push_back(keybind);
                setting->setValue(std::move(keybinds));
            }
        } else {
            GJKeybindManager::sharedState()->bindChord(chord, action);
        }
    }

    virtual void unbindChord(int chord, int action) override {
        auto loaderImpl = LoaderImpl::get();

        if (auto setting = loaderImpl->keybindSettingForActionId(action); setting != nullptr) {
            std::vector<Keybind> keybinds = setting->getValue();

            auto deleted =  std::erase_if(keybinds, [&](auto& e) { return e.toVanillaChord() == chord; });
            if (deleted > 0) {
                setting->setValue(std::move(keybinds));
            }
        } else {
            GJKeybindManager::sharedState()->unbindChord(chord, action);
        }
    }
};

static bool s_parsingBlob = false;

class $modify(GJKeybindManager) {
    $override
    static char const* actionToString(int action) {
        if (s_parsingBlob) {
            return GJKeybindManager::actionToString(action);
        }

        if (auto name = LoaderImpl::get()->keybindNameForActionId(action); name.has_value()) {
            return name->c_str();
        }

        return GJKeybindManager::actionToString(action);
    }

    $override
    void parseBindsBlob(std::string const& blob, bool apply, int* outApplied, int* outUnknown) {
        s_parsingBlob = true;
        GJKeybindManager::parseBindsBlob(blob, apply, outApplied, outUnknown);
        s_parsingBlob = false;
    }
};

static std::unordered_map<int, float> s_scrollFromTop;
static std::unordered_set<std::string> s_collapsedMods;

static int makeKey(bool isLevel, int tab) {
    return (isLevel ? 1000 : 0) + tab;
}

Anchor nearestAnchor(CCPoint pos, CCSize size) {
    // 0 = left, 2 = right, 1 = center
    auto col = pos.x < size.width / 4 ? 0
        : pos.x > 3 * size.width / 4 ? 2
        : 1;
    // 0 = bottom, 2 = top, 1 = center
    auto row = pos.y < size.height / 4 ? 0
        : pos.y > 3 * size.height / 4 ? 2
        : 1;

    static const Anchor table[3][3] = {
        { Anchor::BottomLeft, Anchor::Bottom, Anchor::BottomRight },
        { Anchor::Left, Anchor::Center, Anchor::Right },
        { Anchor::TopLeft, Anchor::Top, Anchor::TopRight },
    };

    return table[row][col];
}

void convertToAnchorLayout(CCNode* node, CCSize size, CCPoint pos) {
    std::unordered_map<CCNode*, CCPoint> previousPositions;

    for (auto child : node->getChildrenExt()) {
        previousPositions[child] = node->convertToWorldSpace(child->getPosition());
    }
    node->setContentSize({size.width, size.height});
    node->setAnchorPoint({0.5, 0.5});
    node->ignoreAnchorPointForPosition(false);
    node->setPosition(pos);

    for (auto& [child, worldPos] : previousPositions) {
        auto local = node->convertToNodeSpace(worldPos);
        auto anchor = nearestAnchor(local, size);
        child->setLayoutOptions(AnchorLayoutOptions::create()
                                ->setAnchor(anchor)
                                ->setOffset(local - AnchorLayout::getAnchoredPosition(node, anchor, {0, 0})));
    }

    node->setLayout(AnchorLayout::create());
}

struct KeybindListEntry {
    bool isEntry;
    int action;
    Mod* mod;
    std::shared_ptr<KeybindSettingV3> setting;
};

namespace list {
    constexpr float VIEW_PADDING_X = 15.0f;
    constexpr float VIEW_BOTTOM = 128.0f;
    constexpr float VIEW_TOP = 96.0f;
    constexpr float VIEW_TOP_SEARCH = 64.0f;

    constexpr float ROW_HEIGHT = 30.0f;
    constexpr float ROW_GAP = 2.0f;
    constexpr float TEXT_PADDING = 10.0f;

    constexpr GLubyte ROW_OPACITY_EVEN = 70;
    constexpr GLubyte ROW_OPACITY_ODD = 40;

    constexpr float CATEGORY_GAP = 15.0f;
    // gameplay, practice, editor, tools, view + rest
    static const ccColor3B CATEGORY_COLORS[] = {
        {120, 255, 120}, {255, 230, 100}, {255, 170, 80}, {200, 150, 255}, {100, 200, 255},
    };

    constexpr auto ROW_BG_FRAME = "square02_small.png";

    constexpr GLubyte HEADER_OPACITY = 110;
    constexpr float HEADER_LOGO_SIZE = 22.0f;
    constexpr float HEADER_GAP = 8.0f;
    constexpr float HEADER_NAME_SCALE = 0.55f;
    constexpr float HEADER_BUTTON_SIZE = 24.0f;

    constexpr float TEXT_SCALE = 0.35f;
    constexpr float TEXT_MIN_SCALE = 0.2f;

    constexpr float CHORD_GAP = 4.0f;
    constexpr float CHORD_MIN_SCALE = 0.4f;
    constexpr float CHORD_SPRITE_HEIGHT = 22.0f;
    constexpr float CHORD_LABEL_SCALE = 0.4f;
    constexpr float ADD_LABEL_SCALE = 0.6f;
    constexpr float GLYPH_HEIGHT = 22.0f;
    static const ccColor3B COLOR_CONFLICT = { 255, 80, 80 };
    static const ccColor3B COLOR_P2 = { 255, 190, 120 };
    constexpr float DRAG_CANCEL_DISTANCE = 8.0f;

    constexpr GLubyte READONLY_OPACITY = 150;
}

// TODO: replace with geode::Toggler

static CCSprite* createCollapseState(char const* frame, float iconScale, char const* id) {
    auto bg = CCSprite::create("square02c_001.png");
    bg->setColor({ 25, 25, 25 });
    bg->setOpacity(105);
    auto icon = CCSprite::createWithSpriteFrameName(frame);
    icon->setScale(iconScale);
    bg->addChildAtPosition(icon, Anchor::Center);
    bg->setID(id);
    return bg;
}

static CCNode* createCollapseIcon(bool collapsed) {
    auto expanded = createCollapseState("edit_downBtn_001.png", 1.9f, "expanded-icon");
    auto folded = createCollapseState("edit_delCBtn_001.png", 1.5f, "collapsed-icon");

    auto node = CCNode::create();
    node->setContentSize(expanded->getContentSize());
    node->addChildAtPosition(expanded, Anchor::Center);
    node->addChildAtPosition(folded, Anchor::Center);
    expanded->setVisible(!collapsed);
    folded->setVisible(collapsed);

    node->setScale(list::HEADER_BUTTON_SIZE / node->getContentHeight());

    return node;
}

static void setCollapseIcon(Button* toggle, bool collapsed) {
    toggle->getChildByIDRecursive("expanded-icon")->setVisible(!collapsed);
    toggle->getChildByIDRecursive("collapsed-icon")->setVisible(collapsed);
}

class $modify(GeodeKeybindingsLayer, KeybindingsLayer) {
    struct Fields {
        Ref<ScrollLayer> list = nullptr;
        GeodeKeybindSource* moddedSource = nullptr;
        bool ready = false;
        int currentTab = -1;
        bool isLevel = false;
        int modsTab = -1;
        bool rebuilding = false;

        ~Fields() {
            if (list && currentTab >= 0) {
                s_scrollFromTop[makeKey(isLevel, currentTab)] = list->getScrollFromTop();
            }
        }
    };

    $override
    bool init(GameSettingsLayer* settings) {
        bool isLevelBindings = typeinfo_cast<LevelBindingsLayer*>(this) != nullptr;
        m_fields->isLevel = isLevelBindings;

        if (!isLevelBindings) {
            m_fields->moddedSource = new GeodeKeybindSource;
            m_keybindSource = m_fields->moddedSource;
            m_ownsKeybindSource = true;
        }

        if (!KeybindingsLayer::init(settings)) {
            return false;
        }

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        if (m_scrollLayer) {
            m_scrollLayer->removeFromParent();
        }
        m_scrollLayer = nullptr;
        m_contentMenu = nullptr;
        m_fields->ready = true;

        convertToAnchorLayout(m_mainLayer, { m_width, m_height }, winSize / 2);
        convertToAnchorLayout(m_buttonMenu, { m_width, m_height }, { m_width / 2, m_height / 2 });
        m_buttonMenu->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Center)->setOffset({0, 0}));
        m_mainLayer->updateLayout();

        this->showTab(m_currentTab);

        if (!isLevelBindings) {
            this->addEventListener(SettingChangedEventV3(), [&](std::string_view modId, std::string_view, std::shared_ptr<SettingV3> setting) {
                if (!typeinfo_pointer_cast<KeybindSettingV3>(setting)) {
                    return;
                }
                this->queueListRebuild();
            });
        }

        return true;
    }

    $override
    void showTab(int tab) {
        auto list = m_fields->list;
        if (list && m_currentTab >= 0) {
            s_scrollFromTop[makeKey(m_fields->isLevel, m_currentTab)] = list->getScrollFromTop();
        }
        m_fields->currentTab = tab;
        KeybindingsLayer::showTab(tab);
        if (auto list = m_fields->list; list && m_currentTab == tab) {
            list->scrollToFromTop(s_scrollFromTop[makeKey(m_fields->isLevel, tab)]);
        }
    }

    $override
    void buildContentForTab(int tab) {
        if (!m_fields->ready) {
            return;
        }
        auto isSearch = tab == m_searchTab;

        float viewW = m_width - 2 * list::VIEW_PADDING_X;
        float top = isSearch ? list::VIEW_TOP_SEARCH : list::VIEW_TOP;
        float viewH = top + list::VIEW_BOTTOM;
        float offsetY = (top - list::VIEW_BOTTOM) / 2;

        if (m_fields->list) {
            m_fields->list->removeFromParent();
            m_fields->list = nullptr;
        }
        auto list = ScrollLayer::create({0, 0, viewW, viewH});
        list->setID("keybinds-list");
        list->m_contentLayer->setLayout(listColumnLayout(list::ROW_GAP, false));
        m_mainLayer->addChildAtPosition(list, Anchor::Center, { -list->getContentWidth() / 2, -list->getContentHeight() / 2 + offsetY });
        m_fields->list = list;

        struct ModGroup { Mod* mod; std::vector<int> actions; };

        std::vector<int> vanilla;
        std::vector<ModGroup> groups;
        std::unordered_map<Mod*, size_t> groupIndex;

        auto settingFor = [&](int id) {
            return m_fields->moddedSource ? LoaderImpl::get()->keybindSettingForActionId(id) : nullptr;
        };

        for (auto id : this->actionsToShow(tab)) {
            auto setting = settingFor(id);
            if (!setting || isSearch) {
                vanilla.push_back(id);
                continue;
            }

            auto [it, inserted] = groupIndex.try_emplace(setting->getMod(), groups.size());
            if (inserted) {
                groups.push_back({ setting->getMod(), {}});
            }
            groups[it->second].actions.push_back(id);
        }

        int index = 0;

        for (int id : vanilla) {
            auto setting = settingFor(id);
            list->m_contentLayer->addChild(createActionRow(id, setting, index++, viewW));
        }

        for (auto& group : groups) {
            list->m_contentLayer->addChild(createModSection(group.mod, group.actions, viewW));
        }

        list->m_contentLayer->updateLayout();
        list->scrollToTop();
        LoaderImpl::get()->queueInMainThread([&, self = Ref(this)] { self->applyListPriorities(); });
    }

    $override
    std::vector<int> actionsToShow(int tab) {
        auto result = KeybindingsLayer::actionsToShow(tab);
        if (!m_fields->moddedSource) {
            return result;
        }

        auto loaderImpl = LoaderImpl::get();

        if (tab == m_fields->modsTab) {
            return loaderImpl->getActionsForKeybindCategory(std::nullopt);
        }
        if (tab == 0) {
            auto actions = loaderImpl->getActionsForKeybindCategory(KeybindCategory::Gameplay);
            result.append_range(actions);
        }
        if (tab == 2) {
            auto actions = loaderImpl->getActionsForKeybindCategory(KeybindCategory::Editor);
            result.append_range(actions);
        }
        if (tab == m_searchTab && !m_searchQuery.empty()) {
            for (auto id : loaderImpl->getActionsForKeybindCategory(std::nullopt)) {
                std::string query = utils::string::toLower(m_searchQuery);
                if (this->customMatchesSearch(id, query)) {
                    result.push_back(id);
                }
            }
        }

        return result;
    }

    void queueListRebuild() {
        if (m_fields->rebuilding) {
            return;
        }

        m_fields->rebuilding = true;
        LoaderImpl::get()->queueInMainThread([self = Ref(this)] {
            self->m_fields->rebuilding = false;
            self->showTab(self->m_currentTab);
        });
    }

    bool customMatchesSearch(int id, ZStringView query) {
        auto name = utils::string::toLower(nameForAction(id));
        if (utils::string::contains(name, query)) {
            return true;
        }

        for (auto chord : m_keybindSource->chordsForAction(id)) {
            auto chordName = utils::string::toLower(GJKeybindManager::sharedState()->chordToString(chord));
            if (utils::string::contains(chordName, query)) {
                return true;
            }
        }

        if (m_fields->moddedSource) {
            if (auto kbName = LoaderImpl::get()->keybindNameForActionId(id); kbName.has_value()) {
                auto kbNameLower = utils::string::toLower(*kbName);
                if (utils::string::contains(kbNameLower, query)) {
                    return true;
                }
            }
        }

        return false;
    }

    CCNode* createRowBg(float width, GLubyte opacity) {
        auto bg = NineSlice::create(list::ROW_BG_FRAME);
        bg->setContentSize({ width, list::ROW_HEIGHT });
        bg->setColor({ 0, 0, 0 });
        bg->setOpacity(opacity);
        bg->setID("bg");

        return bg;
    }

    CCNode* createHeaderRow(Mod* mod, float viewW, Button::ButtonCallback buttonCallback) {
        bool collapsed = s_collapsedMods.contains(mod->getID());

        auto row = CCNode::create();
        row->setContentSize({ viewW, list::ROW_HEIGHT });
        row->setAnchorPoint({ 0.5f, 0.5f });
        row->setID(fmt::format("{}-header", mod->getID()));

        row->addChildAtPosition(createRowBg(viewW, list::HEADER_OPACITY), Anchor::Center);

        auto info = CCNode::create();
        info->setID("info");
        info->setAnchorPoint({ 0.5f, 0.5f });
        info->setContentHeight(list::ROW_HEIGHT);
        info->setLayout(SimpleRowLayout::create()
            ->setGap(list::HEADER_GAP)
            ->setMainAxisScaling(AxisScaling::Fit));

        auto logo = createModLogo(mod);
        logo->setScale(list::HEADER_LOGO_SIZE / logo->getContentHeight());
        logo->setID("logo");
        logo->setLayoutOptions(SimpleAxisLayoutOptions::create()
            ->setScalingPriority(ScalingPriority::Never));
        info->addChild(logo);

        float nameX = list::TEXT_PADDING + list::HEADER_LOGO_SIZE + list::HEADER_GAP;
        auto name = geode::Label::create(mod->getName(), "goldFont.fnt");
        name->setAnchorPoint({ 0.5f, 0.5f });
        name->setID("name-label");
        name->setScale(list::HEADER_NAME_SCALE);
        info->addChild(name);
        info->updateLayout();

        info->setLayoutOptions(SimpleAxisLayoutOptions::create()
            ->setScalingPriority(ScalingPriority::First)
            ->setMinRelativeScale(list::TEXT_MIN_SCALE / list::HEADER_NAME_SCALE));

        auto gear = createGeodeCircleButton(
            CCSprite::createWithSpriteFrameName("settings.png"_spr), 1.f,
            CircleBaseSize::Small, false, true);
        // margin 3 above and below
        gear->setScale(list::HEADER_BUTTON_SIZE / gear->getContentHeight());
        auto settings = Button::createWithNode(gear, [mod](Button*) { openSettingsPopup(mod); });
        settings->setID("settings-button");

        auto toggle = Button::createWithNode(createCollapseIcon(collapsed), std::move(buttonCallback));
        toggle->setID("collapse-button");

        auto buttons = CCNode::create();
        buttons->setID("buttons");
        buttons->setContentHeight(list::ROW_HEIGHT);
        buttons->setLayout(SimpleRowLayout::create()
            ->setGap(list::HEADER_GAP)
            ->setMainAxisScaling(AxisScaling::Fit));
        buttons->setLayoutOptions(SimpleAxisLayoutOptions::create()
            ->setScalingPriority(ScalingPriority::Never));
        buttons->addChild(settings);
        buttons->addChild(toggle);
        buttons->updateLayout();

        auto content = CCNode::create();
        content->setContentSize({ viewW - 2 * list::TEXT_PADDING, list::ROW_HEIGHT });
        content->setID("content");
        content->setAnchorPoint({ 0.5f, 0.5f });
        content->setLayout(SimpleRowLayout::create()
            ->setMainAxisAlignment(MainAxisAlignment::Between)
            ->setMainAxisScaling(AxisScaling::ScaleDown));
        content->addChild(info);
        content->addChild(buttons);
        content->updateLayout();

        row->addChildAtPosition(content, Anchor::Center);

        return row;
    }

    void setupListButton(Button* button) {
        button->setSwallowTouches(false);
        button->setDragCancelDistance(list::DRAG_CANCEL_DISTANCE);
    }

    void applyListPriorities() {
        auto dispatcher = CCTouchDispatcher::get();
        auto popup = dispatcher->findHandler(this);
        auto list = m_fields->list;

        if (!popup || !list) {
            return;
        }

        int p = popup->getPriority();
        if (dispatcher->findHandler(list)) {
            dispatcher->setPriority(p - 1, list);
        }

        findFirstChildRecursive<Button>(list, [&](Button* button) {
            button->setTouchPriority(p - 2);
            if (dispatcher->findHandler(button)) {
                dispatcher->setPriority(p - 2, button);
            }
            return false; // just loop through all children
        });
    }

    Label* categoryLabel(int id, std::shared_ptr<KeybindSettingV3> setting) {
        std::string text;
        ccColor3B color;

        if (setting) {
            text = setting->getMod()->getName() + ":";
            color = ccWHITE;
        } else {
            int tab = vanillaTabFor(this, id);
            if (tab < 0) {
                text = "Unk:";
                color = list::CATEGORY_COLORS[1];
            } else {
                text = m_tabs[tab].m_name + ":";
                color = list::CATEGORY_COLORS[std::min(tab, 4)];
            }
        }

        auto label = Label::create(std::move(text), "bigFont.fnt");
        label->setColor(color);
        label->setID("category-label");
        return label;
    }


    bool chordConflicts(int id, int chord) {
        auto groupOf = [](int id) { return id <= 0 ? 0 : id / 10000; };
        int group = groupOf(id);

        for (auto other : m_keybindSource->actionsForChord(chord)) {
            if (other != id && GJKeybindManager::groupsConflict(group, groupOf(other))) {
                return true;
            }
        }

        if (this->crossCheckGlobalConflicts()) {
            for (auto other : GJKeybindManager::sharedState()->actionsForChord(chord)) {
                if (other == id || GJKeybindManager::isMovementAction(other)) {
                    continue;
                }
                if (GJKeybindManager::groupsConflict(group, groupOf(other))) {
                    return true;
                }
            }
        }

        return false;
    }

    ButtonSprite* chordButtonSprite(std::string const& str, bool conflict) {
        auto spr = ButtonSprite::create(
            str.c_str(),
            (int)std::max(list::CHORD_SPRITE_HEIGHT, str.size() * 8.0f),
            0,
            list::CHORD_LABEL_SCALE,
            true,
            "bigFont.fnt",
            "GJ_button_04.png",
            list::CHORD_SPRITE_HEIGHT
        );

        if (conflict) {
            spr->m_label->setColor(list::COLOR_CONFLICT);
        }
        return spr;
    }

    CCNode* chordSprite(int id, int chord) {
        bool conflict = this->chordConflicts(id, chord);
        bool p2 = false;

        if (auto glyph = KeybindingsLayer::buildControllerGlyph(chord & 0xffff, p2)) {
            glyph->setScale(list::GLYPH_HEIGHT / glyph->getContentHeight());
            if (conflict || p2) {
                KeybindingsLayer::tintGlyph(glyph, conflict ? list::COLOR_CONFLICT : list::COLOR_P2);
            }

            return glyph;
        }

        return this->chordButtonSprite(GJKeybindManager::sharedState()->chordToString(chord), conflict);
    }

    CCNode* createModSection(Mod* mod, std::vector<int> const& actions, float viewW) {
        auto settingFor = [&](int id) {
            return m_fields->moddedSource ? LoaderImpl::get()->keybindSettingForActionId(id) : nullptr;
        };

        std::string id = std::string(mod->getID());
        bool collapsed = s_collapsedMods.contains(id);

        auto section = CCNode::create();
        section->setID(fmt::format("{}-section", id));
        section->setContentWidth(viewW);
        section->setLayout(listColumnLayout(list::ROW_GAP));

        auto rows = CCNode::create();
        rows->setID(fmt::format("{}-rows", id));
        rows->setContentWidth(viewW);
        rows->setLayout(listColumnLayout(list::ROW_GAP));

        int idx = 0;
        for (int action : actions) {
            rows->addChild(createActionRow(action, settingFor(action), idx++, viewW));
        }
        rows->setVisible(!collapsed);
        rows->updateLayout();

        auto header = createHeaderRow(mod, viewW, [self = Ref(this), rows = Ref(rows), section = Ref(section), id](Button* toggle) {
             auto list = self->m_fields->list;
            if (!list) {
                return;
            }

            bool nowCollapsed = rows->isVisible();
            if (nowCollapsed) {
                s_collapsedMods.insert(id);
            } else {
                s_collapsedMods.erase(id);
            }

            float offset = list->getScrollFromTop();
            rows->setVisible(!nowCollapsed);
            section->updateLayout();
            list->m_contentLayer->updateLayout();
            list->scrollToFromTop(offset);
            setCollapseIcon(toggle, nowCollapsed);
        });

        section->addChild(header);
        section->addChild(rows);
        section->updateLayout();

        return section;
    }

    CCNode* createActionRow(int id, std::shared_ptr<KeybindSettingV3> setting, int index, float viewW) {
        auto isSearch = m_currentTab == m_searchTab;
        auto showController = this->showControllerChords();

        auto row = CCNode::create();
        row->setContentSize({ viewW, list::ROW_HEIGHT });
        row->setAnchorPoint({ 0.5f, 0.5f });
        row->setID(setting ? fmt::format("{}/{}", setting->getModID(), setting->getKey())
                           : fmt::format("action-{}", id));

        row->addChildAtPosition(createRowBg(viewW, index & 1 ? list::ROW_OPACITY_ODD : list::ROW_OPACITY_EVEN), Anchor::Center);

        auto text = CCNode::create();
        text->setID("text");
        text->setContentHeight(list::ROW_HEIGHT);
        text->setScale(list::TEXT_SCALE);
        text->setLayout(SimpleRowLayout::create()
            ->setGap(list::CATEGORY_GAP)
            ->setMainAxisScaling(AxisScaling::Fit));

        if (isSearch) {
            text->addChild(this->categoryLabel(id, setting));
        }

        auto name = Label::create(this->nameForAction(id), "bigFont.fnt");
        name->setID("name-label");
        text->addChild(name);
        text->updateLayout();

        text->setLayoutOptions(SimpleAxisLayoutOptions::create()
            ->setScalingPriority(ScalingPriority::First)
            ->setMinRelativeScale(list::TEXT_MIN_SCALE / list::TEXT_SCALE));

        auto controls = CCNode::create();
        controls->setContentHeight(list::ROW_HEIGHT);
        controls->setID("controls");
        controls->setLayout(SimpleRowLayout::create()
            ->setGap(list::CHORD_GAP)
            ->setMainAxisAlignment(MainAxisAlignment::End)
            ->setMainAxisScaling(AxisScaling::Fit));

        auto chords = m_keybindSource->chordsForAction(id);

        std::vector<int> order(chords.size());
        std::iota(order.begin(), order.end(), 0);
        std::ranges::stable_partition(order, [&](int i) {
            return GJKeybindManager::isControllerChord(chords[i]);
        });

        for (int idx : order) {
            if (GJKeybindManager::isControllerChord(chords[idx]) && !showController) {
                continue;
            }

            auto button = Button::createWithNode(this->chordSprite(id, chords[idx]), [this](Button* b) { this->onEditBinding(b); });
            // action holds low 24 bits, idx holds 8 high bits, idk gd reads it in callbacks
            button->setTag((id & 0xffffff) | (idx << 24));
            button->setID(fmt::format("chord-button-{}", chords[idx]));
            this->setupListButton(button);
            controls->addChild(button);
        }

        // add keybinds that this screen can't handle (anything that uses super, mouse buttons, etc)
        if (setting) {
            int n = 0;
            for (auto const& kb : setting->getValue()) {
                if (kb.toVanillaChord()) {
                    continue;
                }

                auto spr = chordButtonSprite(kb.toString(), false);
                spr->setOpacity(list::READONLY_OPACITY);
                spr->setID(fmt::format("readonly-chord-{}", n++));
                controls->addChild(spr);
            }
        }

        auto plus = Button::createWithNode(
            ButtonSprite::create("+", 10, 0, list::ADD_LABEL_SCALE, true, "bigFont.fnt", "GJ_button_01.png", list::CHORD_SPRITE_HEIGHT),
            [this](Button* b) { this->onAddBinding(b); }
        );
        plus->setTag(id);
        plus->setID("add-button");
        this->setupListButton(plus);
        controls->addChild(plus);
        controls->updateLayout();

        controls->setLayoutOptions(SimpleAxisLayoutOptions::create()
            ->setScalingPriority(ScalingPriority::Late)
            ->setMinRelativeScale(list::CHORD_MIN_SCALE));

        auto content = CCNode::create();
        content->setContentSize({ viewW - 2 * list::TEXT_PADDING, list::ROW_HEIGHT });
        content->setAnchorPoint({ 0.5f, 0.5f });
        content->setID("content");
        content->setLayout(SimpleRowLayout::create()
            ->setMainAxisAlignment(MainAxisAlignment::Between)
            ->setMainAxisScaling(AxisScaling::ScaleDown));
        content->addChild(text);
        content->addChild(controls);
        content->updateLayout();

        row->addChildAtPosition(content, Anchor::Center);

        return row;
    }
};

class $modify(TabbedPopupLayer) {
    void createTabButtons() {
        auto layer = typeinfo_cast<KeybindingsLayer*>(this);
        if (layer && !typeinfo_cast<LevelBindingsLayer*>(this)) {
            int index = this->addTab("Mods", "");
            static_cast<GeodeKeybindingsLayer*>(layer)->m_fields->modsTab = index;
        }

        TabbedPopupLayer::createTabButtons();
    }
};

#endif
