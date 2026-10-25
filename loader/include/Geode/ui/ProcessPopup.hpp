#pragma once

#include "Popup.hpp"

namespace geode {
    class GEODE_DLL ProcessPopup final : public Popup {
        class Impl;
        std::unique_ptr<Impl> m_impl;

    protected:
        ProcessPopup();
        ~ProcessPopup();

        void onClose(CCObject*) override;

        bool init(
            std::string text, ZStringView font, ZStringView bg, CircleBaseColor btnColor, float width
        );

    public:
        enum class State : uint8_t {
            Ongoing,
            Success,
            Failure,
        };

        using CloseCallback = Function<void(ProcessPopup*, State)>;

        /// Create a process popup
        /// @param text The label that will show first
        /// @param font Name of the label's font `.fnt` file
        /// @param width The width of the popup
        static ProcessPopup* create(
            std::string text, ZStringView font = "chatFont.fnt", float width = 200.f
        );

        /// Create a process popup with distinct theming
        /// @param text The label that will show first
        /// @param font Name of the label's font `.fnt` file
        /// @param bg Sprite name for the popup background
        /// @param btnColor The base color for the popup's close button's sprite
        /// @param width The width of the popup
        static ProcessPopup* create(
            std::string text, ZStringView font, ZStringView bg, CircleBaseColor btnColor,
            float width = 200.f
        );

        /// Set the callback that will fire when the user attempts to close the popup
        /// @param cb The lambda which will take a `ProcessPopup*` param
        /// @warning Upon setting a callback, you must handle closing the popup yourself
        void setCloseCallback(CloseCallback cb);

        /// Reset the callback that would've fired when the user attempted to close the popup
        void resetCloseCallback();

        /// Update the message on the popup while the process continues
        /// @param text The message to show
        void updateMessage(std::string text);

        /// Set the popup to a success state with an optional message
        /// @param text The message to show
        /// @warning After setting a finished state, you cannot update the popup again afterwards
        void setSuccess(std::string text = "Success");

        /// Set the popup to a failure state with an optional message
        /// @param text The message to show
        /// @warning After setting a finished state, you cannot update the popup again afterwards
        void setFailure(std::string text = "Failed");

        ZStringView getCurrentMessage() const noexcept;
        State getProcessState() const noexcept;
    };
};