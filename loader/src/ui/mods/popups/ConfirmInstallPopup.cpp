#include "ConfirmInstallPopup.hpp"
#include <Geode/ui/GeodeUI.hpp>

class SmallModItem : public CCNode {
protected:
    bool init(ModMetadata const& metadata) {
        if (!CCNode::init())
            return false;
        
        this->setContentSize({ 60, 80 });

        auto logo = createServerModLogo(metadata.getID());
        limitNodeSize(logo, { 45, 45 }, 5.f, .1f);
        this->addChildAtPosition(logo, Anchor::Center, ccp(0, 10));

        auto title = CCLabelBMFont::create(metadata.getName().c_str(), "bigFont.fnt");
        title->limitLabelWidth(m_obContentSize.width, .3f, .1f);
        this->addChildAtPosition(title, Anchor::Center, ccp(0, -20));

        auto developer = CCLabelBMFont::create(fmt::format("by {}", metadata.getDeveloper()).c_str(), "goldFont.fnt");
        developer->limitLabelWidth(m_obContentSize.width, .3f, .1f);
        this->addChildAtPosition(developer, Anchor::Center, ccp(0, -30));

        return true;
    }

public:
    static SmallModItem* create(ModMetadata const& metadata) {
        auto ret = new SmallModItem();
        if (ret && ret->init(metadata)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

bool ConfirmInstallPopup::setup() {
    m_noElasticity = true;

    this->setTitle("Customize Installation");

    auto installationsTitle = CCLabelBMFont::create("You are Installing:", "bigFont.fnt");
    installationsTitle->setScale(.5f);
    m_mainLayer->addChildAtPosition(installationsTitle, Anchor::Center, ccp(0, 100));

    m_installations = CCNode::create();
    m_installations->setAnchorPoint({ .5f, .5f });
    m_installations->setContentSize({ m_size.width, 60 });
    m_installations->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(.1f, .65f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
    );
    m_mainLayer->addChildAtPosition(m_installations, Anchor::Center, ccp(0, 60));

    auto dependenciesTitle = CCLabelBMFont::create("Which Depend(s) on:", "bigFont.fnt");
    dependenciesTitle->setScale(.5f);
    m_mainLayer->addChildAtPosition(dependenciesTitle, Anchor::Center, ccp(0, 20));

    m_dependencies = CCNode::create();
    m_dependencies->setAnchorPoint({ .5f, .5f });
    m_dependencies->setContentSize({ m_size.width, 60 });
    m_dependencies->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(.1f, .65f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
    );
    m_mainLayer->addChildAtPosition(m_dependencies, Anchor::Center, ccp(0, -20));

    auto incompatablitiesTitle = CCLabelBMFont::create("But is/are Incompatible with:", "bigFont.fnt");
    incompatablitiesTitle->setScale(.5f);
    m_mainLayer->addChildAtPosition(incompatablitiesTitle, Anchor::Center, ccp(0, -60));

    m_incompatabilities = CCNode::create();
    m_incompatabilities->setAnchorPoint({ .5f, .5f });
    m_incompatabilities->setContentSize({ m_size.width, 60 });
    m_incompatabilities->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(.1f, .65f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
    );
    m_mainLayer->addChildAtPosition(m_incompatabilities, Anchor::Center, ccp(0, -100));

    m_downloadListener.bind([this](auto) { this->updateState(); });

    this->updateState();

    return true;
}

void ConfirmInstallPopup::updateState() {
    m_installations->removeAllChildren();
    m_dependencies->removeAllChildren();
    m_incompatabilities->removeAllChildren();

    for (auto& download : server::ModDownloadManager::get()->getDownloads()) {
        auto status = download.getStatus();
        if (auto confirm = std::get_if<server::DownloadStatusConfirm>(&status)) {
            if (download.getDependencyFor()) {
                m_dependencies->addChild(SmallModItem::create(confirm->version.metadata));
            }
            else {
                m_installations->addChild(SmallModItem::create(confirm->version.metadata));
            }
            for (auto& inc : confirm->version.metadata.getIncompatibilities()) {
                if (inc.mod) {
                    m_installations->addChild(SmallModItem::create(inc.mod->getMetadata()));
                }
            }
        }
    }

    m_installations->updateLayout();
    m_dependencies->updateLayout();
    m_incompatabilities->updateLayout();
}

ConfirmInstallPopup* ConfirmInstallPopup::create() {
    auto ret = new ConfirmInstallPopup();
    if (ret && ret->init(350, 280)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
void ConfirmInstallPopup::askForCustomize() {
    size_t confirmCount = 0;
    size_t dependencyCount = 0;
    for (auto& download : server::ModDownloadManager::get()->getDownloads()) {
        auto status = download.getStatus();
        if (auto confirm = std::get_if<server::DownloadStatusConfirm>(&status)) {
            confirmCount += 1;
            if (download.getDependencyFor()) {
                dependencyCount += 1;
            }
        }
    }

    createQuickPopup(
        "Confirm Install",
        fmt::format(
            "<cy>{} mods</c> will be installed, out of which <cr>{}</c> are <cr>dependencies</c>. "
            "Do you want to <cb>customize</c> the installation, or continue with default options?",
            confirmCount, dependencyCount
        ),
        "Customize", "Continue",
        [](auto*, bool btn2) {
            if (btn2) {
                server::ModDownloadManager::get()->confirmAll();
            }
            else {
                ConfirmInstallPopup::create()->show();
            }
        }
    );
}
