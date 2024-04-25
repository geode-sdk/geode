#include "ModProblemItem.hpp"
#include <Geode/utils/ColorProvider.hpp>
#include <Geode/ui/GeodeUI.hpp>

bool ModProblemItem::init() {
    if (!CCNode::init())
        return false;
    
    this->setContentSize({ 250, 28 });

    auto bg = CCScale9Sprite::create("square02b_small.png");
    bg->setOpacity(20);
    bg->ignoreAnchorPointForPosition(false);
    bg->setAnchorPoint({ .5f, .5f });
    bg->setScale(.7f);
    bg->setContentSize(m_obContentSize / bg->getScale());
    this->addChildAtPosition(bg, Anchor::Center);

    m_menu = CCMenu::create();
    m_menu->setContentWidth(m_obContentSize.width / 2 - 10);
    m_menu->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(.1f, .35f)
            ->setAxisAlignment(AxisAlignment::End)
    );
    this->addChildAtPosition(m_menu, Anchor::Right, ccp(-5, 0), ccp(1, .5f));

    this->setLoading();

    return true;
}

void ModProblemItem::clearState() {
    this->removeChildByID("loading-spinner");
    this->removeChildByID("error-label");
}
void ModProblemItem::setLoading() {
    this->clearState();
    
    auto loadingCircle = createLoadingCircle(20);
    this->addChildAtPosition(loadingCircle, Anchor::Center);
}
void ModProblemItem::setError(std::string const& error) {
    this->clearState();

    auto label = CCLabelBMFont::create(error.c_str(), "bigFont.fnt");
    label->setColor("mod-list-errors-found"_cc3b);
    label->limitLabelWidth(m_obContentSize.width, .5f, .1f);
    label->setID("error-label");
    this->addChildAtPosition(label, Anchor::Center);
}
void ModProblemItem::setMod(ModMetadata const& metadata) {
    this->clearState();
    
    auto h = m_obContentSize.height;

    auto logo = createServerModLogo(metadata.getID());
    limitNodeSize(logo, { h / 1.4f, h / 1.4f }, 5.f, .1f);
    this->addChildAtPosition(logo, Anchor::Left, ccp(h / 2, 0));

    auto title = CCLabelBMFont::create(metadata.getName().c_str(), "bigFont.fnt");
    title->limitLabelWidth(m_obContentSize.width / 2, .35f, .1f);
    this->addChildAtPosition(title, Anchor::Left, ccp(h, h / 5), ccp(0, .5f));

    auto versionLabel = CCLabelBMFont::create(
        metadata.getVersion().toVString().c_str(),
        "bigFont.fnt"
    );
    versionLabel->setColor("mod-list-version-label"_cc3b);
    versionLabel->limitLabelWidth(m_obContentSize.width / 2, .25f, .1f);
    this->addChildAtPosition(
        versionLabel, Anchor::Left,
        ccp(h + title->getScaledContentWidth() + 3, h / 5), ccp(0, .5f)
    );

    auto developer = CCLabelBMFont::create(
        fmt::format("By {}", metadata.getDeveloper()).c_str(),
        "goldFont.fnt"
    );
    developer->limitLabelWidth(m_obContentSize.width / 2, .35f, .1f);
    this->addChildAtPosition(developer, Anchor::Left, ccp(h, -h / 5), ccp(0, .5f));
}

ModProblemItem* ModProblemItem::create() {
    auto ret = new ModProblemItem();
    if (ret && ret->init()) {
        ret->setError("Unknown Type");
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ModProblemItem* ModProblemItem::parse(LoadProblem const& problem) {
    return ModProblemItem::create();
}
