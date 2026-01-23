#pragma once

#include "MDTextArea.hpp"
#include "Popup.hpp"

namespace geode {
    /**
     * FLAlertLayer with support for Markdown. See MDTextArea
     * for details on what features of MD are supported.
     */
    class GEODE_DLL MDPopup : public Popup {
    protected:
        geode::Function<void(bool)> m_onClick = nullptr;

        bool init(
            bool compatibilityMode, ZStringView title, std::string info, ZStringView btn1, ZStringView btn2,
            geode::Function<void(bool)> onClick
        );

        void onBtn(CCObject*);

        static float estimateHeight(std::string_view content);

    public:
        static MDPopup* create(
            ZStringView title, std::string content, ZStringView btn1,
            ZStringView btn2 = nullptr, geode::Function<void(bool)> onClick = nullptr
        );

        static MDPopup* create(
            bool compatibilityMode, ZStringView title, std::string content,
            ZStringView btn1, ZStringView btn2 = nullptr, geode::Function<void(bool)> onClick = nullptr
        );
    };
}
