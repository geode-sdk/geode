#pragma once

#include <Geode/ui/Popup.hpp>

USE_GEODE_NAMESPACE();

class DevProfilePopup : public Popup<std::string const&> {
protected:
    bool setup(std::string const& developer) override;

public:
    static DevProfilePopup* create(std::string const& developer);
};
