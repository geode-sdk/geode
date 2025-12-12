#pragma once

#include "MDTextArea.hpp"
#include "Popup.hpp"

namespace geode {
    /**
     * FLAlertLayer with support for Markdown. See MDTextArea
     * for details on what features of MD are supported.
     */
    class GEODE_DLL MDPopup :
        public Popup<
            std::string const&, std::string const&, char const*, char const*,
            geode::Function<void(bool)>> {
    protected:
        geode::Function<void(bool)> m_onClick = nullptr;

        bool setup(
            std::string const& title, std::string const& info, char const* btn1, char const* btn2,
            geode::Function<void(bool)> onClick
        ) override;

        void onBtn(CCObject*);

        static float estimateHeight(std::string const& content);

    public:
        static MDPopup* create(
            std::string const& title, std::string const& content, char const* btn1,
            char const* btn2 = nullptr, geode::Function<void(bool)> onClick = nullptr
        );

        static MDPopup* create(
            bool compatibilityMode, std::string const& title, std::string const& content,
            char const* btn1, char const* btn2 = nullptr, geode::Function<void(bool)> onClick = nullptr
        );
    };
}
