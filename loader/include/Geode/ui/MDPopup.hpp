#pragma once

#include "MDTextArea.hpp"
#include "Popup.hpp"

namespace geode {
    /**
     * FLAlertLayer with support for Markdown. See MDTextArea
     * for details on what features of MD are supported.
     */
    class MDPopup :
        public Popup<
            std::string const&, std::string const&, char const*, char const*,
            utils::MiniFunction<void(bool)>> {
    protected:
        utils::MiniFunction<void(bool)> m_onClick = nullptr;

        bool setup(
            std::string const& title, std::string const& info, char const* btn1, char const* btn2,
            utils::MiniFunction<void(bool)> onClick
        ) override;

        void onBtn(CCObject*);

        static float estimateHeight(std::string const& content);

    public:
        static MDPopup* create(
            std::string const& title, std::string const& content, char const* btn1,
            char const* btn2 = nullptr, utils::MiniFunction<void(bool)> onClick = nullptr
        );
    };
}
