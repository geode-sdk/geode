#pragma once

#include "BasedButtonSprite.hpp"
#include "Popup.hpp"

#include <Geode/utils/function.hpp>

namespace geode {
    class GEODE_DLL NumberInputPopup final : public Popup {
        class Impl;
        std::unique_ptr<Impl> m_impl;

    protected:
        NumberInputPopup();
        ~NumberInputPopup();

        bool init(double current, double min, double max, std::string title, ZStringView btnText, ZStringView btnBg, ZStringView bg, CircleBaseColor closeBtnColor, float inputWidth);

    public:
        using Callback = Function<void(NumberInputPopup*, double)>;

        /// Create a generic number input popup
        /// @param current Default number to appear first on the input
        /// @param min Lowest allowed number
        /// @param max Highest allowed number
        /// @param btnText Text to display on the submit button
        /// @param inputWidth Width of the number input node
        static NumberInputPopup* create(double current, double min, double max, std::string title, ZStringView btnText = "Go", float inputWidth = 55.f);

        /// Create a generic number input popup with distinct theming
        /// @param current Default number to appear first on the input
        /// @param min Lowest allowed number
        /// @param max Highest allowed number
        /// @param btnText Text to display on the submit button
        /// @param btnBg Sprite name for the button background
        /// @param bg Sprite name for the popup background
        /// @param closeBtnColor The base color for the popup's close button's sprite
        /// @param inputWidth Width of the number input node
        static NumberInputPopup* create(double current, double min, double max, std::string title, ZStringView btnText, ZStringView btnBg, ZStringView bg = "GJ_square02.png", CircleBaseColor closeBtnColor = CircleBaseColor::Green, float inputWidth = 55.f);

        /// Set the callback that fires when the user submits a number
        /// @param cb The lambda which will take a `NumberInputPopup*` param
        /// @warning Upon setting a callback, you must handle closing the popup yourself
        void setCallback(Callback cb);

        /// Set the value of the number input
        /// @param value The new value to set
        /// @note This will not fire the callback
        void setValue(double value);

        /// Set the step of the number input
        /// @param step The amount to increment/decrement the value by when using the arrow buttons
        void setArrowStep(double step);

        /// Set the precision of the number input
        /// @param precision The number of decimal places to allow
        void setPrecision(size_t precision);

        /// Reset the callback that would've fired when the user submit a number
        void resetCallback();

        double getValue() const;
        double getMin() const noexcept;
        double getMax() const noexcept;

        ZStringView getValueString() const;

        double getArrowStep() const noexcept;

        size_t getPrecision() const noexcept;
    };
};