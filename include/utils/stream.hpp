#pragma once

#include <Geode.hpp>

namespace geode::stream {
    GEODE_DLL std::ostream& operator<<(std::ostream& stream, cocos2d::CCPoint const& pos);
    GEODE_DLL std::ostream& operator<<(std::ostream& stream, cocos2d::CCSize  const& size);
    GEODE_DLL std::ostream& operator<<(std::ostream& stream, cocos2d::CCRect  const& rect);
}
