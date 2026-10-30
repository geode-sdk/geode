#pragma once

#include "Popup.hpp"

namespace geode {
    class NumberInputPopup final : public Popup {
        class Impl;
        std::unique_ptr<Impl> m_impl;

    protected:
        NumberInputPopup();
        ~NumberInputPopup();

        bool init(int64_t current, int64_t min, int64_t max, std::string title, ZStringView button, float inputWidth);

    public:
        static NumberInputPopup* create(int64_t current, int64_t min, int64_t max, std::string title, ZStringView button = "Go", float inputWidth = 55.f);
    };
};