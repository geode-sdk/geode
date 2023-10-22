#include "AddIDs.hpp"

#include <Geode/Bindings.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;

$register_ids(EditLevelLayer) {
    setIDs(
        this,
        0,
        "background",
        "bottom-left-art",
        "bottom-right-art",
        "level-name-background",
        "level-name-input",
        "description-background",
        "description-input",
        "description-text-area",
        "level-edit-menu",
        "level-length",
        "level-song",
        "level-verified",
        "version-label",
        "level-id-label",
        "level-actions-menu",
        "back-menu",
        "info-button-menu"
    );

    auto winSize = CCDirector::get()->getWinSize();
    auto descBG = this->getChildByID("description-background");

    auto descMenu = CCMenu::create();
    descMenu->setID("description-menu");
    descMenu->setLayout(ColumnLayout::create()); 
    descMenu->setPosition(
        descBG->getPositionX() - descBG->getScaledContentSize().width / 2 - 35.f,
        descBG->getPositionY()
    );
    descMenu->setContentSize({ 40.f, 80.f });
    this->addChild(descMenu);

    if (auto menu = this->getChildByID("level-edit-menu")) {
        setIDs(menu, 0, "edit-button", "play-button", "share-button");
        if (menu->getChildrenCount() == 4) {
            auto btn = static_cast<CCNode*>(menu->getChildren()->objectAtIndex(3));
            btn->setID("update-desc-button");
            btn->retain();
            btn->removeFromParent();
            descMenu->addChild(btn);
            btn->release();
            descMenu->updateLayout();
        }
        menu->setContentSize({ winSize.width - 160.f, 100.f });
        menu->setLayout(RowLayout::create()->setGap(25.f));
    }

    if (auto menu = this->getChildByID("level-actions-menu")) {
        setIDs(
            menu,
            0,
            "delete-button",
            "help-button",
            "duplicate-button",
            "move-to-top-button",
            "folder-button"
        );

        auto folderMenu = detachAndCreateMenu(
            this, "folder-menu",
            ColumnLayout::create(),
            menu->getChildByID("folder-button")
        );
        folderMenu->setContentSize({ 50.f, 215.f });
        folderMenu->updateLayout();

        menu->setPosition(
            menu->getPositionX() + static_cast<CCNode*>(
                menu->getChildren()->firstObject()
            )->getPositionX(),
            winSize.height / 2
        );
        menu->setContentSize({ 60.f, winSize.height - 15.f });
        menu->setLayout(
            ColumnLayout::create()
                ->setGap(7.f)
                ->setAxisAlignment(AxisAlignment::End)
                ->setAxisReverse(true)
        );
        menu->setZOrder(1);
    }

    if (auto menu = this->getChildByID("back-menu")) {
        auto backBtn = setIDSafe(menu, 0, "back-button");
        menu->setPositionX(
            menu->getPositionX() + 100.f / 2 - 
                getSizeSafe(backBtn).width / 2
        );
        menu->setContentSize({ 100.f, 50.f });
        menu->setLayout(
            RowLayout::create()
                ->setAxisAlignment(AxisAlignment::Start)
        );
    }

    if (auto menu = this->getChildByID("info-button-menu")) setIDSafe(menu, 0, "info-button");
}

struct EditLevelLayerIDs : Modify<EditLevelLayerIDs, EditLevelLayer> {
    static void onModify(auto& self) {
        if (!self.setHookPriority("EditLevelLayer::init", GEODE_ID_PRIORITY)) {
            log::warn("Failed to set EditLevelLayer::init hook priority, node IDs may not work properly");
        }
    }

    bool init(GJGameLevel* l) {
        if (!EditLevelLayer::init(l)) return false;

        NodeIDs::get()->provide(this);

        return true;
    }
};
