#include "ConfirmInstall.hpp"
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/ui/Popup.hpp>
#include <ui/mods/GeodeStyle.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <server/DownloadManager.hpp>

using namespace geode::prelude;
using namespace server;

using Importance = ModMetadata::Dependency::Importance;

void letsConfirm();

class ConfirmPopup : public GeodePopup<> {
    CCNode* m_list = nullptr;
    std::unordered_map<std::string, bool> m_isInvalid;
    std::unordered_map<std::string, bool> m_checked;

    bool setup() override;

    void checkMod(std::string const& id);
    void checkMods();
public:
    void updateState();
    bool isInvalid(std::string const& id);
    // will enable dependencies
    void setSkipped(std::string const& id, bool skip, bool first = true);

    static ConfirmPopup* create();
};

class ConfirmModCell : public CCLayer {
    CCScale9Sprite* m_bgSpr;
    CCNode* m_main;
    CCMenuItemToggler* m_toggle;
    CCLabelBMFont* m_title;
    CCLabelBMFont* m_subtitle;
    bool m_isInvalid = false;
    bool m_isSkipped = false;

public:
    bool init(ModDownload& download, ConfirmPopup* parent) {
        CCLayer::init();
        this->setID(fmt::format("ConfirmModCell-{}", download.getID()));
        this->setContentSize({ 260, 30 });

        m_isSkipped = download.isSkipped();

        m_bgSpr = CCScale9Sprite::create("square02b_small.png");
        m_bgSpr->setScale(0.7f);
        m_bgSpr->setContentSize(this->getContentSize() / m_bgSpr->getScale());
        m_bgSpr->setAnchorPoint({0, 0});
        m_bgSpr->setOpacity(20);
        this->addChild(m_bgSpr);

        m_main = CCNode::create();
        m_main->ignoreAnchorPointForPosition(false);
        m_main->setAnchorPoint({0, 0});
        m_main->setContentSize(this->getContentSize() - 10.f);
        m_main->setPosition({5, 5});
        m_main->setLayout(SimpleRowLayout::create()
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setCrossAxisScaling(AxisScaling::ScaleDownGaps)
        );

        // make a menu just for the toggle
        auto* toggleMenu = CCMenu::create();
        toggleMenu->ignoreAnchorPointForPosition(false);
        toggleMenu->setContentSize({20, 20});

        m_toggle = CCMenuItemExt::createTogglerWithStandardSprites(0.5f, [parent, id = download.getID()](CCMenuItemToggler* toggler) {
            bool on = !toggler->isOn();
            parent->setSkipped(id, !on);
            // updateState eventually calls toggle,
            // and so when this callback would finish gd would
            // then flip it again and mess it up. so lets flip it again
            // to unflip it later
            toggler->toggle(!on);
        });

        auto* logo = geode::createServerModLogo(download.getID());
        logo->setLayoutOptions(SimpleAxisLayoutOptions::create()->setScalingPriority(ScalingPriority::Last));

        auto* infoLay = CCNode::create();
        infoLay->setLayout(SimpleColumnLayout::create()
            ->setMainAxisScaling(AxisScaling::Grow)
            ->setCrossAxisScaling(AxisScaling::Grow)
            ->setCrossAxisAlignment(CrossAxisAlignment::Start)
        );

        auto* logoInfoLay = CCNode::create();
        logoInfoLay->setLayout(SimpleRowLayout::create()
            ->setMainAxisScaling(AxisScaling::Grow)
            ->setCrossAxisScaling(AxisScaling::Grow)
        );
        logoInfoLay->setScale(0.775f);

        auto* leftLay = CCNode::create();
        leftLay->setLayout(SimpleColumnLayout::create()
            ->setMainAxisScaling(AxisScaling::Grow)
            ->setCrossAxisScaling(AxisScaling::Grow)
            ->setCrossAxisAlignment(CrossAxisAlignment::Start)
        );

        m_title = CCLabelBMFont::create("?", "bigFont.fnt");
        m_title->setScale(0.8f);
        auto* developers = CCLabelBMFont::create("by ?", "goldFont.fnt");
        if (auto meta = download.getMetadata()) {
            developers->setString(ModMetadata::formatDeveloperDisplayString(meta->getDevelopers()).c_str());
        }
        developers->setScale(0.8f);

        m_subtitle = CCLabelBMFont::create(" ", "chatFont.fnt");

        infoLay->addChild(m_title);
        infoLay->addChild(developers);

        logoInfoLay->addChild(logo);
        logoInfoLay->addChild(infoLay);

        leftLay->addChild(logoInfoLay);
        leftLay->addChild(m_subtitle);

        toggleMenu->addChildAtPosition(m_toggle, Anchor::Center);

        m_main->addChild(leftLay);
        // m_main->addChild(SpacerNode::create(1));
        m_main->addChild(toggleMenu);

        this->addChild(m_main);

        this->updateState(download, parent);

        infoLay->updateLayout();
        logoInfoLay->updateLayout();
        leftLay->updateLayout();
        m_main->updateLayout();

        return true;
    }

    void updateState(ModDownload& download, ConfirmPopup* parent) {
        m_toggle->toggle(!download.isSkipped());

        std::string name = download.getID();
        if (auto meta = download.getMetadata()) {
            name = meta->getName();
        }
        std::string version = "?";
        if (auto ver = download.getVersion()) {
            version = ver->toNonVString();
        }

        m_title->setString(fmt::format("{}", name, version).c_str());

        const float animTime = 0.15;
        const auto opacityFrom = 20;
        const auto opacityTo = 40;
        if (download.isSkipped()) {
            if (!m_isSkipped) {
                m_bgSpr->stopAllActions();
                m_bgSpr->runAction(CCSequence::createWithTwoActions(
                    CCFadeTo::create(animTime, opacityTo),
                    CCFadeTo::create(animTime, opacityFrom)
                ));
            }
            m_isInvalid = false;
            m_title->setSkewX(10);
            m_title->setColor(ccc3(196, 196, 196));
        } else if (parent->isInvalid(download.getID())) {
            if (!m_isInvalid) {
                m_bgSpr->stopAllActions();
                m_bgSpr->runAction(CCSequence::createWithTwoActions(
                    CCSpawn::createWithTwoActions(
                        CCFadeTo::create(animTime, opacityTo), CCTintTo::create(animTime, 255, 200, 200)
                    ),
                    CCSpawn::createWithTwoActions(
                        CCFadeTo::create(animTime, opacityFrom), CCTintTo::create(animTime, 255, 255, 255)
                    )
                ));
            }
            m_isInvalid = true;
            m_title->setSkewX(0);
            m_title->setColor(ccc3(255, 183, 183));
        } else {
            if (m_isSkipped || m_isInvalid) {
                m_bgSpr->stopAllActions();
                m_bgSpr->runAction(CCSequence::createWithTwoActions(
                    CCFadeTo::create(animTime, opacityTo),
                    CCFadeTo::create(animTime, opacityFrom)
                ));
            }
            m_isInvalid = false;
            m_title->setSkewX(0);
            m_title->setColor(ccWHITE);
        }
        m_isSkipped = download.isSkipped();
        if (auto deps = download.getDependencyFor(); !deps.empty()) {
            int requiredFor = 0;
            for (auto& dep : deps) {
                if (dep.importance == Importance::Required) {
                    requiredFor++;
                    if (requiredFor == 1) {
                        m_subtitle->setString(fmt::format("required by {}", dep.id).c_str());
                    }
                }
            }
            if (requiredFor > 1) {
                m_subtitle->setString(fmt::format("required by {} mods", requiredFor).c_str());
            } else if (requiredFor == 0) {
                if (deps.size() == 1) {
                    m_subtitle->setString(fmt::format("optional for {}", deps[0].id).c_str());
                } else {
                    m_subtitle->setString(fmt::format("optional for {} mods", deps.size()).c_str());
                }
            }
        }

        m_title->getParent()->updateLayout();
        m_subtitle->getParent()->updateLayout();
        m_main->updateLayout();
    }

    static ConfirmModCell* create(ModDownload& download, ConfirmPopup* parent) {
        auto* node = new ConfirmModCell();
        return (node->init(download, parent) && node->autorelease()) || (delete node, node = nullptr), node;
    }
};

void ConfirmPopup::updateState() {
    this->checkMods();
    for (auto& [_, download] : ModDownloadManager::get()->getDownloadsRef()) {
        if (!download.isConfirm()) continue;

        auto* cell = static_cast<ConfirmModCell*>(m_list->getChildByID(fmt::format("ConfirmModCell-{}", download.getID())));
        if (!cell) continue;
        cell->updateState(download, this);
    }
    m_list->updateLayout();
}

bool ConfirmPopup::isInvalid(std::string const& id) {
    return m_isInvalid[id];
}

void ConfirmPopup::checkMod(std::string const& id) {
    if (m_checked[id]) return;
    m_checked[id] = true;

    auto* info = ModDownloadManager::get()->getDownloadRef(id);
    if (!info || !info->isConfirm()) return;

    for (auto dep : info->getMetadata()->getDependencies()) {
        if (dep.importance != Importance::Required) continue;
        this->checkMod(dep.id);
        auto* depInfo = ModDownloadManager::get()->getDownloadRef(dep.id);
        if (!depInfo) continue;
        if (depInfo->isSkipped() || depInfo->canRetry() || m_isInvalid[dep.id]) {
            m_isInvalid[id] = true;
            break;
        }
    }
}

void ConfirmPopup::checkMods() {
    m_isInvalid.clear();
    m_checked.clear();
    for (auto& [_, download] : ModDownloadManager::get()->getDownloadsRef()) {
        if (!download.isConfirm()) continue;

        this->checkMod(download.getID());
    }
}

void ConfirmPopup::setSkipped(const std::string& id, bool skip, bool first) {
    auto* info = ModDownloadManager::get()->getDownloadRef(id);
    if (!info || !info->isConfirm()) return;

    if (info->isSkipped() == skip) return;
    info->setSkipped(skip);
    if (!skip) {
        // just enabled, lets enable required dependencies
        for (auto dep : info->getMetadata()->getDependencies()) {
            if (dep.importance != Importance::Required) continue;
            setSkipped(dep.id, false, false);
        }
    }
    if (first) {
        this->updateState();
    }
}

bool ConfirmPopup::setup() {
    auto* scroll = ScrollLayer::create(m_mainLayer->getContentSize() - ccp(10, 40));
    m_list = scroll->m_contentLayer;
    m_list->setLayout(
        SimpleColumnLayout::create()
            ->setMainAxisDirection(AxisDirection::TopToBottom)
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setCrossAxisAlignment(CrossAxisAlignment::Center)
            ->setMainAxisScaling(AxisScaling::Grow)
            ->setGap(5.f)
    );

    for (auto& [_, download] : ModDownloadManager::get()->getDownloadsRef()) {
        if (!download.isConfirm()) continue;

        auto* cell = ConfirmModCell::create(download, this);
        m_list->addChild(cell);
    }
    m_list->updateLayout();

    scroll->ignoreAnchorPointForPosition(false);
    scroll->m_contentLayer->setContentSize(m_list->getContentSize());
    m_mainLayer->addChildAtPosition(scroll, Anchor::Center);
    scroll->scrollToTop();

    m_buttonMenu->addChildAtPosition(CCMenuItemExt::createSpriteExtra(ButtonSprite::create("Cancel"), [this](auto*) {
        this->onClose(nullptr);
        ModDownloadManager::get()->cancelAll();
    }), Anchor::BottomLeft);
    m_buttonMenu->addChildAtPosition(CCMenuItemExt::createSpriteExtra(ButtonSprite::create("Confirm"), [this](auto*) {
        this->onClose(nullptr);
        letsConfirm();
    }), Anchor::BottomRight);

    this->updateState();
    return true;
}

ConfirmPopup* ConfirmPopup::create() {
    auto* node = new ConfirmPopup();
    return (node->init(300, 280) && node->autorelease()) || (delete node, node = nullptr), node;
}

void askConfirmModInstalls() {
    log::info("going to ask for confirm");

    ConfirmPopup::create()->show();
}

void letsConfirm() {
    struct ToConfirm final {
        size_t modCount = 0;
        size_t replacementCount = 0;
        size_t dependencyCount = 0;
        std::unordered_set<Mod*> toDisable;
        std::unordered_set<std::string> toDisableModId;
        std::unordered_set<Mod*> toEnable;
    };

    auto toConfirm = ToConfirm();

    // Collect all things we need to ask confirmation for
    for (auto download : ModDownloadManager::get()->getDownloads()) {
        if (download.isSkipped()) continue;

        auto status = download.getStatus();
        if (auto conf = std::get_if<DownloadStatusConfirm>(&status)) {
            // check if user picked the mod or if it came from a dependency
            if (download.getDependencyFor().empty()) {
                // Since the user has already explicitly chosen to download these mods, we
                // are going to assume they want these mods enabled over already installed
                // ones

                // If this mod has incompatibilities that are installed, disable them
                for (auto inc : conf->version.metadata.getIncompatibilities()) {
                    if (inc.mod && inc.version.compare(conf->version.metadata.getVersion()) && inc.mod->isOrWillBeEnabled()) {
                        toConfirm.toDisable.insert(inc.mod);
                    }
                    for (auto download : ModDownloadManager::get()->getDownloads()) {
                        if (download.isDone() && inc.id == download.getID() && inc.version.compare(conf->version.metadata.getVersion())) {
                            toConfirm.toDisableModId.insert(inc.id);
                        }
                    }
                }
                // If some installed mods are incompatible with this one, disable them
                for (auto mod : Loader::get()->getAllMods()) {
                    for (auto inc : mod->getMetadataRef().getIncompatibilities()) {
                        if (inc.id == conf->version.metadata.getID() && inc.version.compare(mod->getVersion()) && mod->isOrWillBeEnabled()) {
                            toConfirm.toDisable.insert(mod);
                        }
                    }
                }
                // If some newly downloaded mods are incompatible with this one, disable them
                for (auto download : ModDownloadManager::get()->getDownloads()) {
                    auto status = download.getStatus();
                    if (auto done = std::get_if<DownloadStatusDone>(&status)) {
                        for (auto inc : done->version.metadata.getIncompatibilities()) {
                            if (inc.id == conf->version.metadata.getID() && inc.version.compare(done->version.metadata.getVersion())) {
                                toConfirm.toDisableModId.insert(download.getID());
                            }
                        }
                    }
                }

                // If this mod has required dependencies that are disabled, enable them
                for (auto dep : conf->version.metadata.getDependencies()) {
                    if (
                        dep.importance == ModMetadata::Dependency::Importance::Required &&
                        dep.mod && !dep.mod->isOrWillBeEnabled()
                    ) {
                        toConfirm.toEnable.insert(dep.mod);
                    }
                }
            }
        }
    }

    for (auto mod : toConfirm.toDisable) {
        (void)mod->disable();
    }
    for (auto modId : toConfirm.toDisableModId) {
        Mod::get()->setSavedValue("should-load-" + modId, false);
    }
    for (auto mod : toConfirm.toEnable) {
        (void)mod->enable();
    }
    ModDownloadManager::get()->confirmAll();
}
