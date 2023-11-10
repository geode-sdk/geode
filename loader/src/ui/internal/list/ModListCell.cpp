#include "ModListCell.hpp"
#include "ModListLayer.hpp"
#include "../info/ModInfoPopup.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/StatsCell.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <loader/LoaderImpl.hpp>
#include "../info/TagNode.hpp"
#include "../info/DevProfilePopup.hpp"
#include "ProblemsListPopup.hpp"

template <class T>
static bool tryOrAlert(Result<T> const& res, char const* title) {
    if (!res) {
        FLAlertLayer::create(title, res.unwrapErr(), "OK")->show();
    }
    return res.isOk();
}

void ModListCell::draw() {
    reinterpret_cast<StatsCell*>(this)->StatsCell::draw();
}

float ModListCell::getLogoSize() const {
    return m_height / 1.5f;
}

void ModListCell::setupInfo(
    ModMetadata const& metadata,
    bool spaceForTags,
    ModListDisplay display,
    bool inactive
) {
    m_menu = CCMenu::create();
    m_menu->setPosition({m_width / 2, m_height / 2});
    m_menu->setContentSize({m_width - 20, m_height});
    m_menu->setAnchorPoint({.5f, .5f});
    m_menu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAutoScale(false)
            ->setCrossAxisOverflow(false)
    );

    this->addChild(m_menu);

    auto logoSize = this->getLogoSize();

    auto logoSpr = this->createLogo({ logoSize, logoSize });
    auto logoSprColor = typeinfo_cast<CCRGBAProtocol*>(logoSpr);
    if (inactive && logoSprColor) {
        logoSprColor->setColor({ 163, 163, 163 });
    }
    m_menu->addChild(logoSpr);

    m_columnMenu = CCMenu::create();
    m_columnMenu->setContentSize({m_width, m_height});
    m_columnMenu->setLayout(
        ColumnLayout::create()
            ->setAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisLineAlignment(AxisAlignment::Start)
            ->setAxisReverse(true)
            ->setAutoScale(false)
            ->setGap(spaceForTags ? 3.f : 5.f)
    );
    m_menu->addChild(SpacerNodeChild::create(m_columnMenu));

    m_labelMenu = CCMenu::create();
    m_labelMenu->setContentSize({m_width, m_height / 2});
    m_labelMenu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAutoScale(true)
    );
    m_labelMenu->setLayoutOptions(
        AxisLayoutOptions::create()
            ->setNextGap(spaceForTags ? -2.f : 0.f)
            ->setPrevGap(0.f)
    );
    m_columnMenu->addChild(m_labelMenu);

    bool hasDesc =
        display == ModListDisplay::Expanded && 
        metadata.getDescription().has_value();

    auto titleLabel = CCLabelBMFont::create(metadata.getName().c_str(), "bigFont.fnt");
    titleLabel->limitLabelWidth(m_width / 2 - 40.f, .5f, .1f);
    titleLabel->setLayoutOptions(
        AxisLayoutOptions::create()
            ->setScalePriority(1)
            ->setMaxScale(0.5f)
            ->setMinScale(0.1f)
    );
    if (inactive) {
        titleLabel->setColor({ 163, 163, 163 });
    }
    m_labelMenu->addChild(titleLabel);

    auto versionLabel = CCLabelBMFont::create(
        metadata.getVersion().toString(false).c_str(),
        "bigFont.fnt"
    );
    versionLabel->setColor({ 0, 255, 0 });
    if (inactive) {
        versionLabel->setColor({ 0, 163, 0 });
    }
    versionLabel->setLayoutOptions(
        AxisLayoutOptions::create()
            ->setMaxScale(0.3)
            ->setMinScale(0.1)
    );
    m_labelMenu->addChild(versionLabel);

    TagNode* apiLabel = nullptr;


    if (auto tag = metadata.getVersion().getTag()) {
        auto tagLabel = TagNode::create(tag.value().toString().c_str());
        tagLabel->setLayoutOptions(
            AxisLayoutOptions::create()
                ->setMaxScale(0.3)
                ->setMinScale(0.1)
        );
        m_labelMenu->addChild(tagLabel);
    }

    if (metadata.isAPI()) {
        apiLabel = TagNode::create("API");
        apiLabel->setLayoutOptions(
            AxisLayoutOptions::create()
                ->setMaxScale(0.3)
                ->setMinScale(0.1)
        );
        m_labelMenu->addChild(apiLabel);
    }

    auto creatorStr = "by " + metadata.getDeveloper();
    auto creatorLabel = CCLabelBMFont::create(creatorStr.c_str(), "goldFont.fnt");
    creatorLabel->setScale(.43f);
    if (inactive) {
        creatorLabel->setColor({ 163, 163, 163 });
    }

    m_developerBtn = CCMenuItemSpriteExtra::create(
        creatorLabel, this, menu_selector(ModListCell::onViewDev)
    );
    m_columnMenu->addChild(m_developerBtn);

    if (hasDesc) {
        auto descBG = CCScale9Sprite::create("square02b_001.png", {0.0f, 0.0f, 80.0f, 80.0f});
        descBG->setColor({0, 0, 0});
        descBG->setOpacity(90);
        descBG->setContentSize({m_width * 2, 60.f});
        descBG->setScale(.25f);
        m_columnMenu->addChild(descBG);

        // limitLabelWidth defaults to 1.0 even though we give a bigger scale
        auto node = CCNode::create();
        node->setContentSize(descBG->getContentSize() / 4);
        node->setAnchorPoint({ .5f, .5f });
        node->setScale(4.f);
        node->setPosition(descBG->getContentSize() / 2);
        descBG->addChild(node);

        m_description = CCLabelBMFont::create(metadata.getDescription().value().c_str(), "chatFont.fnt");
        m_description->setAnchorPoint({ .5f, .5f });
        m_description->setPosition(node->getContentSize() / 2);
        m_description->limitLabelWidth(node->getContentSize().width - 5.f, 0.5f, .1f);
        if (inactive) {
            m_description->setColor({ 163, 163, 163 });
        }
        node->addChild(m_description);
    }

    this->updateCellLayout();
}

void ModListCell::updateCellLayout() {
    m_menu->updateLayout();
    m_labelMenu->setContentSize(m_columnMenu->getContentSize());
    m_labelMenu->updateLayout();
    m_columnMenu->updateLayout();
}

void ModListCell::onViewDev(CCObject*) {
    DevProfilePopup::create(this->getDeveloper(), m_layer)->show();
}

bool ModListCell::init(ModListLayer* list, CCSize const& size) {
    m_width = size.width;
    m_height = size.height;
    m_layer = list;
    this->setContentSize(size);
    this->setID("mod-list-cell");
    return true;
}

void ModListCell::disableDeveloperButton() {
    m_developerBtn->setEnabled(false);
}

// ModCell

ModCell* ModCell::create(
    Mod* mod,
    ModListLayer* list,
    ModListDisplay display,
    CCSize const& size
) {
    auto ret = new ModCell();
    if (ret && ret->init(mod, list, display, size)) {
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void ModCell::onEnable(CCObject* sender) {
    if (!Mod::get()->getSavedValue("shown-mod-toggle-info", true)) {
        FLAlertLayer::create(
            "Notice",
            "<cb>Toggling</c> a mod requires you to <cg>restart</c> the game.\n"
            "When a mod is <cr>disabled</c>, it will not get loaded at all.",
            "OK"
        )->show();
    }
    if (!as<CCMenuItemToggler*>(sender)->isToggled()) {
        tryOrAlert(m_mod->enable(), "Error enabling mod");
    }
    else {
        tryOrAlert(m_mod->disable(), "Error disabling mod");
    }
    if (m_layer) {
        m_layer->reloadList();   
    }
}

void ModCell::onUnresolvedInfo(CCObject*) {
    ProblemsListPopup::create(m_mod)->show();
}

void ModCell::onInfo(CCObject*) {
    LocalModInfoPopup::create(m_mod, m_layer)->show();
}

void ModCell::onRestart(CCObject*) {
    utils::game::restart();
}

void ModCell::updateState() {
    bool unresolved = m_mod->hasUnresolvedDependencies();
    bool shouldLoad = m_mod->shouldLoad();
    auto toggleable = !unresolved || !shouldLoad;
    if (m_enableToggle) {
        m_enableToggle->toggle(m_mod->isEnabled());
        m_enableToggle->setEnabled(toggleable);
        m_enableToggle->m_offButton->setOpacity(!toggleable ? 100 : 255);
        m_enableToggle->m_offButton->setColor(!toggleable ? cc3x(155) : cc3x(255));
        m_enableToggle->m_onButton->setOpacity(!toggleable ? 100 : 255);
        m_enableToggle->m_onButton->setColor(!toggleable ? cc3x(155) : cc3x(255));
    }
    bool hasProblems = false;
    for (auto const& item : Loader::get()->getProblems()) {
        if (!std::holds_alternative<Mod*>(item.cause) ||
            std::get<Mod*>(item.cause) != m_mod ||
            item.type <= LoadProblem::Type::Recommendation)
            continue;
        hasProblems = true;
        break;
    }
    m_unresolvedExMark->setVisible(hasProblems);

    this->updateCellLayout();
}

bool ModCell::init(
    Mod* mod,
    ModListLayer* list,
    ModListDisplay display,
    CCSize const& size
) {
    if (!ModListCell::init(list, size))
        return false;
    m_mod = mod;

    this->setupInfo(mod->getMetadata(), false, display, mod->getRequestedAction() != ModRequestedAction::None);

    auto exMark = CCSprite::createWithSpriteFrameName("exMark_001.png");
    exMark->setScale(.5f);

    m_unresolvedExMark =
        CCMenuItemSpriteExtra::create(exMark, this, menu_selector(ModCell::onUnresolvedInfo));
    m_unresolvedExMark->setVisible(false);
    m_menu->addChild(m_unresolvedExMark);

    if (mod->getRequestedAction() != ModRequestedAction::None) {
        auto restartSpr = ButtonSprite::create("Restart", "bigFont.fnt", "GJ_button_03.png", .8f);
        restartSpr->setScale(.65f);

        auto restartBtn = CCMenuItemSpriteExtra::create(restartSpr, this, menu_selector(ModCell::onRestart));
        m_menu->addChild(restartBtn);
    }
    else {
        if (m_mod->getMetadata().getID() != "geode.loader") {
            m_enableToggle =
                CCMenuItemToggler::createWithStandardSprites(this, menu_selector(ModCell::onEnable), .7f);
            m_menu->addChild(m_enableToggle);
        }

        auto viewSpr = ButtonSprite::create("View", "bigFont.fnt", "GJ_button_01.png", .8f);
        viewSpr->setScale(.65f);

        if (m_mod->isEnabled()) {
            auto latestIndexItem = Index::get()->getMajorItem(
                mod->getMetadata().getID()
            );

            if (latestIndexItem && Index::get()->isUpdateAvailable(latestIndexItem)) {
                viewSpr->updateBGImage("GE_button_01.png"_spr);

                auto minorIndexItem = Index::get()->getItem(
                    mod->getMetadata().getID(),
                    ComparableVersionInfo(mod->getMetadata().getVersion(), VersionCompare::MoreEq)
                );

                if (latestIndexItem->getMetadata().getVersion().getMajor() > minorIndexItem->getMetadata().getVersion().getMajor()) {
                    auto updateIcon = CCSprite::createWithSpriteFrameName("updates-available.png"_spr);
                    updateIcon->setZOrder(99);
                    updateIcon->setScale(.5f);
                    viewSpr->addChild(updateIcon);
                }
            }
        }

        auto viewBtn = CCMenuItemSpriteExtra::create(viewSpr, this, menu_selector(ModCell::onInfo));
        m_menu->addChild(viewBtn);
    }

    this->updateState();

    return true;
}

std::string ModCell::getDeveloper() const {
    return m_mod->getDeveloper();
}

CCNode* ModCell::createLogo(CCSize const& size) {
    return geode::createModLogo(m_mod, size);
}

// IndexItemCell

void IndexItemCell::onInfo(CCObject*) {
    IndexItemInfoPopup::create(m_item, m_layer)->show();
}

void IndexItemCell::onRestart(CCObject*) {
    utils::game::restart();
}

IndexItemCell* IndexItemCell::create(
    IndexItemHandle item,
    ModListLayer* list,
    ModListDisplay display,
    CCSize const& size
) {
    auto ret = new IndexItemCell();
    if (ret && ret->init(item, list, display, size)) {
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool IndexItemCell::init(
    IndexItemHandle item,
    ModListLayer* list,
    ModListDisplay display,
    CCSize const& size
) {
    if (!ModListCell::init(list, size))
        return false;

    m_item = item;

    bool justInstalled = item->isInstalled() && !Loader::get()->isModInstalled(item->getMetadata().getID());

    this->setupInfo(item->getMetadata(), item->getTags().size(), display, justInstalled);

    if (justInstalled) {
        auto restartSpr = ButtonSprite::create("Restart", "bigFont.fnt", "GJ_button_03.png", .8f);
        restartSpr->setScale(.65f);

        auto restartBtn = CCMenuItemSpriteExtra::create(restartSpr, this, menu_selector(IndexItemCell::onRestart));
        m_menu->addChild(restartBtn);
    }
    else {
        auto viewSpr = ButtonSprite::create("View", "bigFont.fnt", "GJ_button_01.png", .8f);
        viewSpr->setScale(.65f);

        auto viewBtn =
            CCMenuItemSpriteExtra::create(viewSpr, this, menu_selector(IndexItemCell::onInfo));
        m_menu->addChild(viewBtn);
    }

    if (item->getTags().size()) {
        auto tagRow = CCNode::create();
        tagRow->setContentSize({m_width, m_height});
        tagRow->setLayout(
            RowLayout::create()
                ->setAxisAlignment(AxisAlignment::Start)
                ->setAutoScale(false)
                ->setGap(3.f)
        );
        m_columnMenu->insertAfter(tagRow, m_developerBtn);
        for (auto& category : item->getTags()) {
            auto node = TagNode::create(category);
            node->setScale(.3f);
            tagRow->addChild(node);
        }
        tagRow->updateLayout();
    }

    this->updateState();

    return true;
}

void IndexItemCell::updateState() {
    this->updateCellLayout();
}

std::string IndexItemCell::getDeveloper() const {
    return m_item->getMetadata().getDeveloper();
}

CCNode* IndexItemCell::createLogo(CCSize const& size) {
    return geode::createIndexItemLogo(m_item, size);
}

// InvalidGeodeFileCell

void InvalidGeodeFileCell::onInfo(CCObject*) {
    FLAlertLayer::create(
        this, "Error Info",
        m_info.reason,
        "OK", "Remove file", 360.f
    )->show();
}

void InvalidGeodeFileCell::FLAlert_Clicked(FLAlertLayer*, bool btn2) {
    if (btn2) {
        try {
            if (ghc::filesystem::remove(m_info.path)) {
                FLAlertLayer::create(
                    "File Removed", "Removed <cy>" + m_info.path.string() + "</c>", "OK"
                )->show();
            }
            else {
                FLAlertLayer::create(
                    "Unable to Remove File",
                    "Unable to remove <cy>" + m_info.path.string() + "</c>", "OK"
                )->show();
            }
        }
        catch (std::exception& e) {
            FLAlertLayer::create(
                "Unable to Remove File",
                "Unable to remove <cy>" + m_info.path.string() + "</c>: <cr>" +
                    std::string(e.what()) + "</c>",
                "OK"
            )
                ->show();
        }
        if (m_layer) {
            m_layer->reloadList();
        }
    }
}

bool InvalidGeodeFileCell::init(
    InvalidGeodeFile const& info,
    ModListLayer* list,
    ModListDisplay display,
    CCSize const& size
) {
    if (!ModListCell::init(list, size))
        return false;

    m_info = info;

    auto menu = CCMenu::create();
    menu->setPosition(m_width - m_height, m_height / 2);
    this->addChild(menu);

    auto titleLabel = CCLabelBMFont::create("Failed to Load", "bigFont.fnt");
    titleLabel->setAnchorPoint({.0f, .5f});
    titleLabel->setScale(.5f);
    titleLabel->setPosition(m_height / 2, m_height / 2 + 7.f);
    this->addChild(titleLabel);

    auto pathLabel = CCLabelBMFont::create(
        m_info.path.string().c_str(),
        "chatFont.fnt"
    );
    pathLabel->setAnchorPoint({ .0f, .5f });
    pathLabel->setScale(.43f);
    pathLabel->setPosition(m_height / 2, m_height / 2 - 7.f);
    pathLabel->setColor({ 255, 255, 0 });
    this->addChild(pathLabel);

    auto whySpr = ButtonSprite::create("Info", 0, false, "bigFont.fnt", "GJ_button_01.png", 0, .8f);
    whySpr->setScale(.65f);

    auto viewBtn =
        CCMenuItemSpriteExtra::create(whySpr, this, menu_selector(InvalidGeodeFileCell::onInfo));
    menu->addChild(viewBtn);

    return true;
}

InvalidGeodeFileCell* InvalidGeodeFileCell::create(
    InvalidGeodeFile const& file,
    ModListLayer* list,
    ModListDisplay display,
    CCSize const& size
) {
    auto ret = new InvalidGeodeFileCell();
    if (ret && ret->init(file, list, display, size)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void InvalidGeodeFileCell::updateState() {}

std::string InvalidGeodeFileCell::getDeveloper() const {
    return "";
}

CCNode* InvalidGeodeFileCell::createLogo(CCSize const& size) {
    return nullptr;
}

// ProblemsCell

void ProblemsCell::onInfo(CCObject*) {
    ProblemsListPopup::create(nullptr)->show();
}

bool ProblemsCell::init(
    ModListLayer* list,
    ModListDisplay display,
    CCSize const& size
) {
    if (!ModListCell::init(list, size))
        return false;

    LoadProblem::Type problemType = LoadProblem::Type::Unknown;
    // iterate problems to find the most important severity
    for (auto const& problem : Loader::get()->getProblems()) {
        if (problemType < problem.type)
            problemType = problem.type;
        // already found the most important one (error)
        if (problemType > LoadProblem::Type::Conflict)
            break;
    }

    std::string icon;
    std::string title;
    switch (problemType) {
        case LoadProblem::Type::Unknown:
            title = "?????";
            break;
        case LoadProblem::Type::Suggestion:
            icon = "GJ_infoIcon_001.png";
            title = "You have suggested mods";
            m_color = { 66, 135, 245 };
            break;
        case LoadProblem::Type::Recommendation:
            icon = "GJ_infoIcon_001.png";
            title = "You have recommended mods";
            m_color = { 66, 135, 245 };
            break;
        case LoadProblem::Type::Conflict:
            icon = "info-warning.png"_spr;
            title = "Some mods had warnings when loading";
            m_color = { 250, 176, 37 };
            break;
        default:
            icon = "info-alert.png"_spr;
            title = "Some mods had problems loading";
            m_color = { 245, 66, 66 };
            break;
    }

    m_menu = CCMenu::create();
    m_menu->setPosition({m_width / 2, m_height / 2});
    m_menu->setContentSize({m_width - 20, m_height});
    m_menu->setAnchorPoint({.5f, .5f});
    m_menu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            // ->setAutoScale(false)
            ->setCrossAxisOverflow(false)
    );
    this->addChild(m_menu);

    auto logoSize = this->getLogoSize();

    if (!icon.empty()) {
        auto logoSpr = CCSprite::createWithSpriteFrameName(icon.c_str());
        limitNodeSize(logoSpr, size, 1.f, .1f);
        m_menu->addChild(logoSpr);
    }

    auto titleLabel = CCLabelBMFont::create(title.c_str(), "bigFont.fnt");
    titleLabel->setLayoutOptions(
        AxisLayoutOptions::create()
            ->setScalePriority(1)
            ->setMaxScale(1.f)
            ->setMinScale(0.1f)
    );
    m_menu->addChild(titleLabel);

    // m_menu->addChild(SpacerNode::create());

    auto viewSpr = ButtonSprite::create("View", "bigFont.fnt", "GJ_button_01.png", .8f);
    viewSpr->setScale(.65f);

    auto viewBtn =
        CCMenuItemSpriteExtra::create(viewSpr, this, menu_selector(ProblemsCell::onInfo));
    m_menu->addChild(viewBtn);

    m_menu->updateLayout();

    return true;
}

std::optional<ccColor3B> ProblemsCell::getColor() {
    return m_color;
}

ProblemsCell* ProblemsCell::create(
    ModListLayer* list,
    ModListDisplay display,
    CCSize const& size
) {
    auto ret = new ProblemsCell();
    if (ret->init(list, display, size)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void ProblemsCell::updateState() {}

std::string ProblemsCell::getDeveloper() const {
    return "";
}

CCNode* ProblemsCell::createLogo(CCSize const& size) {
    return nullptr;
}
