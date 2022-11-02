#pragma once

#include <Geode/DefaultInclude.hpp>
#include <cocos2d.h>

namespace geode {
    class GEODE_DLL InputNode : public cocos2d::CCMenuItem {
    protected:
        cocos2d::extension::CCScale9Sprite* m_bgSprite;
        CCTextInputNode* m_input;

        bool init(float, float, char const*, char const*, std::string const&, int);
        bool init(float, char const*, char const*, std::string const&, int);

    public:
        static InputNode* create(
            float width, char const* placeholder, char const* fontFile, std::string const& filter,
            int limit
        );
        static InputNode* create(
            float width, char const* placeholder, std::string const& filter, int limit
        );
        static InputNode* create(float width, char const* placeholder, std::string const& filter);
        static InputNode* create(float width, char const* placeholder, char const* fontFile);
        static InputNode* create(float width, char const* placeholder);

        void activate() override;

        CCTextInputNode* getInput() const;
        cocos2d::extension::CCScale9Sprite* getBG() const;

        void setEnabled(bool enabled) override;

        void setString(std::string const&);
        char const* getString();
    };
}
