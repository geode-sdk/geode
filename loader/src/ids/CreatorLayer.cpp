#include <Geode/Modify.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/ui/EnterLayerEvent.hpp>
#include "AddIDs.hpp"

USE_GEODE_NAMESPACE();

class $modify(CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init())
            return false;
        
        this->setID("CreatorLayer");

        if (auto menu = getChildOfType<CCMenu>(this, 0)) {
            menu->setID("creator-buttons-menu");

            setIDSafe(menu, 0, "create-button");
            setIDSafe(menu, 1, "saved-button");
            setIDSafe(menu, 2, "scores-button");
            setIDSafe(menu, 3, "quests-button");
            setIDSafe(menu, 4, "daily-button");
            setIDSafe(menu, 5, "weekly-button");
            setIDSafe(menu, 6, "featured-button");
            setIDSafe(menu, 7, "hall-of-fame-button");
            setIDSafe(menu, 8, "map-packs-button");
            setIDSafe(menu, 9, "search-button");
            setIDSafe(menu, 10,"gauntlets-button");

            // move vault button to its own menu

            if (auto lockBtn = setIDSafe(menu, -2, "vault-button")) {
                lockBtn->retain();
                lockBtn->removeFromParent();

                auto lockMenu = CCMenu::create();
                lockMenu->setPosition(menu->convertToWorldSpace(lockBtn->getPosition()));
                lockMenu->setID("top-right-menu");
                lockBtn->setPosition(0, 0);
                lockMenu->addChild(lockBtn);
                lockMenu->setLayout(ColumnLayout::create(5.f, 0.f)->setAlignment(Alignment::Begin));
                this->addChild(lockMenu);

                lockBtn->release();
            }

            // move treasure room button to its own menu

            if (auto roomBtn = setIDSafe(menu, -1, "treasure-room-button")) {
                roomBtn->retain();
                roomBtn->removeFromParent();

                auto roomMenu = CCMenu::create();
                roomMenu->setPosition(menu->convertToWorldSpace(roomBtn->getPosition()));
                roomMenu->setID("bottom-right-menu");
                roomBtn->setPosition(0, 0);
                roomMenu->addChild(roomBtn);
                roomMenu->setLayout(ColumnLayout::create(5.f, 0.f)->setAlignment(Alignment::End));
                this->addChild(roomMenu);

                roomBtn->release();
            }
        }

        if (auto menu = getChildOfType<CCMenu>(this, 1)) {
            menu->setID("go-back-menu");
            setIDSafe(menu, 0, "back-button");
        }

        return true;
    }
};
