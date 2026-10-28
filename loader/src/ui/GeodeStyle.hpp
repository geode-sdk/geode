#pragma once

#include <Geode/ui/Style.hpp>
#include <Geode/DefaultInclude.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/ui/General.hpp>
#include <Geode/ui/IconButtonSprite.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/ZStringView.hpp>
#include <server/Server.hpp>

ButtonSprite* createGeodeTagLabel(server::ServerTag const& tag);
std::pair<cocos2d::ccColor3B, cocos2d::ccColor3B> geodeTagColors(server::ServerTag const& tag);

ButtonSprite* createTagLabel(
    ZStringView text, std::pair<cocos2d::ccColor3B, cocos2d::ccColor3B> const& color
);
IconButtonSprite* createTagLabelWithIcon(
    cocos2d::CCNode* icon, ZStringView text,
    std::pair<cocos2d::ccColor3B, cocos2d::ccColor3B> const& color
);