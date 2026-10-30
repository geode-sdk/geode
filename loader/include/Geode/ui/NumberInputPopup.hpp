#pragma once

#include "Popup.hpp"
#include <Geode/utils/function.hpp>

namespace geode {
    class GEODE_DLL NumberInputPopup final : public Popup {
        class Impl;
        std::unique_ptr<Impl> m_impl;

    protected:
        NumberInputPopup();
        ~NumberInputPopup();

        bool init(int64_t current, int64_t min, int64_t max, std::string title, ZStringView btnText, ZStringView btnBg, ZStringView bg, CircleBaseColor closeBtnColor, float inputWidth);

    public:
        using Callback = Function<void(NumberInputPopup*, int64_t)>;

        static NumberInputPopup* create(int64_t current, int64_t min, int64_t max, std::string title, ZStringView btnText = "Go", float inputWidth = 55.f);

        static NumberInputPopup* create(int64_t current, int64_t min, int64_t max, std::string title, ZStringView btnText, ZStringView btnBg, ZStringView bg = "GJ_square02.png", CircleBaseColor closeBtnColor = CircleBaseColor::Green, float inputWidth = 55.f);

        void setCallback(Callback cb);
    };
};