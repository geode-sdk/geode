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

        

        bool init(double current, std::string title, ZStringView bg, std::optional<double> min, std::optional<double> max);

    public:
        using SubmitCallback = Function<void(NumberInputPopup*, double)>;

        /// Create a generic number input popup
        /// @param current Default number to appear first on the input
        /// @param title Title to show on the popup
        /// @param min Lowest allowed number
        /// @param max Highest allowed number
        static NumberInputPopup* create(double current, std::string title, std::optional<double> min = std::nullopt, std::optional<double> max = std::nullopt);

        /// Create a generic number input popup with distinct theming
        /// @param current Default number to appear first on the input
        /// @param title Title to show on the popup
        /// @param bg Sprite name for the popup background
        /// @param min Lowest allowed number
        /// @param max Highest allowed number
        static NumberInputPopup* create(double current, std::string title, ZStringView bg, std::optional<double> min = std::nullopt, std::optional<double> max = std::nullopt);

        /// Set up the sprite for the submit button
        /// @param text The new text to set
        /// @param bg Name of the new base sprite to set
        void setupSubmitBtn(ZStringView text, ZStringView bg);

        /// Set the callback that fires when the user submits a number
        /// @param cb The lambda which will take a `NumberInputPopup*` param
        /// @warning Upon setting a callback, you must handle closing the popup yourself
        void setSubmitCallback(SubmitCallback cb);

        /// Set the value of the number input
        /// @param value The new value to set
        /// @note This will not fire the callback
        void setValue(double value);

        /// Set the step of the number input
        /// @param step The amount to increment/decrement the value by when using the arrow buttons
        void setArrowStep(double step);

        /// Set the precision of the number input
        /// @param precision The number of decimal places to allow
        /// @note This only affects the display of the number, not the underlying value
        void setPrecision(size_t precision);

        /// Set the width of the number input node
        /// @param width The new width to set, will be clamped to the popup's width
        void setInputWidth(float width);

        /// Set the color of the close button's background
        /// @param color Enum of the new base color to set
        void setCloseBtnColor(CircleBaseColor color);

        /// Show or hide the reset button
        /// @param show Whether to show the reset button or not
        void showResetButton(bool show);

        /// Reset the callback that would've fired when the user submit a number
        void resetSubmitCallback();

        double getValue() const;
        double getMin() const noexcept;
        double getMax() const noexcept;

        ZStringView getValueString() const;

        double getArrowStep() const noexcept;

        size_t getPrecision() const noexcept;
    };
};