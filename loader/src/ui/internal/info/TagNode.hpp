#pragma once

#include <cocos2d.h>

USE_GEODE_NAMESPACE();

enum class TagNodeStyle {
    Tag,
    Dot,
};

class TagNode : public CCNode {
protected:
    bool init(std::string const& category, TagNodeStyle style);

public:
    static TagNode* create(
        std::string const& category, TagNodeStyle style = TagNodeStyle::Tag
    );

    static ccColor3B tagToColor(std::string const& category);
};
