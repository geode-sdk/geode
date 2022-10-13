#pragma once

#include <cocos2d.h>

USE_GEODE_NAMESPACE();

enum class CategoryNodeStyle {
    Tag,
    Dot,
};

class CategoryNode : public CCNode {
protected:
    bool init(
        std::string const& category,
        CategoryNodeStyle style
    );

public:
    static CategoryNode* create(
        std::string const& category,
        CategoryNodeStyle style = CategoryNodeStyle::Tag
    );

    static ccColor3B categoryToColor(std::string const& category);
};
