#pragma once

#include "MDTextArea.hpp"
#include "Popup.hpp"

namespace geode {
    /**
     * FLAlertLayer with support for Markdown. See <Geode/ui/MDTextArea.hpp> 
     * for details on what features of MD are supported.
     */
    class MDPopup : public Popup<
        std::string const&,
        std::string const&,
        const char*,
        const char*,
        std::function<void(bool)>
    > {
    protected:
        std::function<void(bool)> m_onClick = nullptr;

        bool setup(
            std::string const& title,
            std::string const& info,
            const char* btn1,
            const char* btn2,
            std::function<void(bool)> onClick
        ) override;

        void onBtn(CCObject*);

        static float estimateHeight(std::string const& content);
    
    public:
        static MDPopup* create(
            std::string const& title,
            std::string const& content,
            const char* btn1,
            const char* btn2 = nullptr,
            std::function<void(bool)> onClick = nullptr
        );
    };
}
