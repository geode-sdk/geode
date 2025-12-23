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
            bool, char const*, std::string, char const*,
            char const*, geode::Function<void(bool)>> {
    protected:
        geode::Function<void(bool)> m_onClick = nullptr;

        bool setup(
            bool compatibilityMode, char const* title, std::string info, char const* btn1, char const* btn2,
            geode::Function<void(bool)> onClick
        ) override;

        void onBtn(CCObject*);

        static float estimateHeight(std::string_view content);

    public:
        static MDPopup* create(
            char const* title, std::string content, char const* btn1,
            char const* btn2 = nullptr, geode::Function<void(bool)> onClick = nullptr
        );

        static MDPopup* create(
            bool compatibilityMode, char const* title, std::string content,
            char const* btn1, char const* btn2 = nullptr, geode::Function<void(bool)> onClick = nullptr
        );
    };
}
