#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/function.hpp>
#include <cocos2d.h>
#include <string>
#include <vector>

class DropdownOverlay;

namespace geode {
    class GEODE_DLL Dropdown : public cocos2d::CCNode {
        class Impl;
        std::unique_ptr<Impl> m_impl;

        friend class ::DropdownOverlay;

    protected:
        Dropdown();
        ~Dropdown() override;

        bool init(
            float width, std::vector<std::string> options,
            Function<void(std::string const&, size_t)> callback
        );

        void onOpen(cocos2d::CCObject*);

    public:
        static Dropdown* create(
            float width, std::vector<std::string> options,
            Function<void(std::string const&, size_t)> callback
        );

        void setSelected(size_t index);
        void setSelected(std::string_view value);
        size_t getSelectedIndex() const;
        std::string getSelectedValue() const;

        void setEnabled(bool enabled);
        bool isEnabled() const;

        void setItems(std::vector<std::string> options);
    };
}
