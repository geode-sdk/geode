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
        std::string const&
    > {
    protected:
        bool setup(
            std::string const& title,
            std::string const& info,
            std::string const& btn
        ) override;

        static float estimateHeight(std::string const& content);
    
    public:
        static MDPopup* create(
            std::string const& title,
            std::string const& content,
            std::string const& button
        );
    };
}
