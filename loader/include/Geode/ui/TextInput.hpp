#pragma once
#include "Label.hpp"
#include "NineSlice.hpp"

#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/cocos/robtop/mouse_dispatcher/CCMouseDelegate.h>
#include <Geode/cocos/text_input_node/CCIMEDelegate.h>
#include <Geode/cocos/touch_dispatcher/CCTouchDelegateProtocol.h>

namespace geode {
    enum class CommonFilter : uint32_t {
        None = 0, // No filtering

        Uint      = 1 << 0, // Allow an unsigned integer (0-9)
        Lowercase = 1 << 1, // a-z
        Uppercase = 1 << 2, // A-Z
        Latin     = 1 << 3, // full latin (a-z, A-Z + Latin Extended A/B)
        Cyrillic  = 1 << 4, // а-я, А-Я + ukrainian, belarusian, etc.
        Greek     = 1 << 5, // greek + few extended
        Emoji     = 1 << 6, // only vanilla-supported emojis
        Hex       = 1 << 7, // Allow a hexadecimal number (0-9, a-f, A-F)

        Hyphen     = 1 << 8,  // -
        Dot        = 1 << 9,  // .
        Underscore = 1 << 10, // _
        Space      = 1 << 11, // (space)
        Plus       = 1 << 12, // +
        Slash      = 1 << 13, // /
        Equals     = 1 << 14, // =

        Printable    = 1 << 15, // any non-word printable character (supported by bigFont.fnt)
        PrintableExt = 1 << 16, // any non-word printable character (supported by chatFont.fnt)

        Int          = Uint | Hyphen,                                // Allow a signed integer (0-9, -)
        UFloat       = Uint | Dot,                                   // Allow a non-negative floating point number (0-9, .)
        Float        = Int | Dot,                                    // Allow a floating point number (0-9, . -)
        Alphabetic   = Lowercase | Uppercase,                        // Allow letters (a-z, A-Z)
        Alphanumeric = Alphabetic | Uint,                            // Allow letters, numbers (a-z, A-Z, 0-9)
        ID           = Lowercase | Uint | Hyphen | Underscore | Dot, // Allow letters, numbers, dashes, underscores, and dots (a-z, 0-9, - _ .)
        Name         = Alphabetic | Underscore | Space,              // Allow word-like characters & spaces (a-z, A-Z, _ (space))
        Base64Normal = Alphanumeric | Plus | Slash | Equals,         // Allow a non-URL-safe Base64 number
        Base64URL    = Alphanumeric | Plus | Underscore | Equals,    // Allow a URL-safe Base64 number

        Text       = Alphanumeric | Latin | Cyrillic | Greek | PrintableExt,  // Any text that is supported by chatFont.fnt
        TextEmojis = Text | Emoji,                                            // Any text that is supported by chatFont.fnt, including emojis

        Any = Alphanumeric | Printable, // [Old] Allows basically anything possible to type in an input (supported by bigFont.fnt)
    };

    GEODE_DLL std::string const& getCommonFilterAllowedChars(CommonFilter filter);

    class GEODE_DLL TextInput :
        public cocos2d::CCNode,
        public cocos2d::CCIMEDelegate,
        public cocos2d::CCTouchDelegate,
        public cocos2d::CCMouseDelegate {
    public:
        TextInput();
        ~TextInput() override;

        enum class FocusState : uint8_t {
            Focused,
            Unfocused
        };

        enum class OverflowMode : uint8_t {
            Clip, // Overflow is clipped (default), the content can be scrolled
            Scale, // Overflow is scaled down to fit
        };

        struct FocusEvent : GlobalEvent<FocusEvent, bool(TextInput* sender, FocusState), bool(), TextInput*, FocusState> {
            using GlobalEvent::GlobalEvent;
        };

        struct TextChangedEvent : GlobalEvent<TextChangedEvent, bool(TextInput* sender, std::string const& value), bool(std::string const& value), TextInput*> {
            using GlobalEvent::GlobalEvent;
        };

        static TextInput* create(float width, std::string placeholder, ZStringView font = "bigFont.fnt");
        static TextInput* create(cocos2d::CCSize const& size, std::string placeholder, ZStringView font = "bigFont.fnt");
        static TextInput* createMultiline(cocos2d::CCSize const& size, std::string placeholder, ZStringView font = "bigFont.fnt");

        bool attachWithIME() override;
        bool detachWithIME() override;

        void onEnter() override;
        void onExit() override;

        void setContentSize(cocos2d::CCSize const& contentSize) override;

        bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
        bool scrollWheel(float y, float x) override;
        bool rightClick(float x, float y) override;

        virtual void registerWithTouchDispatcher();

        /// Set the placeholder label for this input.
        void setPlaceholder(std::string placeholder);

        /// Set the placeholder label color.
        void setPlaceholderColor(cocos2d::ccColor4B color);

        /// Set the background color for selection box.
        void setSelectionColor(cocos2d::ccColor4B color);

        /// Set the color of the caret (the blinking text cursor)
        void setCaretColor(cocos2d::ccColor4B color);

        /// Set the width of the caret in points.
        void setCaretWidth(float width);

        /// Set the color of the text the user types.
        void setTextColor(cocos2d::ccColor4B color);

        /// Set the padding between the input's borders and the text.
        void setPadding(cocos2d::CCPoint padding);
        cocos2d::CCPoint getPadding() const;

        /// Set the horizontal alignment of the text.
        /// @note Gets forced to left if multiline text has disabled wrap and overflows horizontally.
        void setAlignment(Label::Alignment alignment);
        Label::Alignment getAlignment() const;

        /// Set the character filter applied to user input. Empty string disables filtering.
        void setFilter(std::string filter);

        /// Set the character filter from one of the default presets.
        void setCommonFilter(CommonFilter filter);

        /// Get the currently active filter characters.
        std::string const& getFilter() const;

        /// Set a label on this input that shows up on the top, above the input.
        /// Set an empty string to remove the label.
        void setLabel(std::string label);

        /// Set the maximum amount of characters for this input. Use 0 for infinite length
        void setMaxCharCount(size_t length);
        size_t getMaxCharCount() const;

        /// Enable/disable password mode (all input characters are rendered as
        /// dots rather than the actual characters)
        void setPasswordMode(bool enable);
        bool isPasswordMode() const;

        /// Enable/disable numeric mode (makes mobile keyboard only show numeric characters, does not affect input validation)
        void setNumericMode(bool enable);
        bool isNumeric() const;

        /// Enable/disable multiline mode
        void setMultiline(bool multiline);
        bool isMultiline() const;

        /// Enable/disable the input
        void setEnabled(bool enabled);
        bool isEnabled() const;

        /// Enable/disable automatic line wrapping for multiline mode
        void setWrap(bool wrap);
        bool isWrap() const;

        void setOverflowMode(OverflowMode mode);
        OverflowMode getOverflowMode() const;

        void setFontScale(float scale);
        float getFontScale() const;

        void setPlaceholderScale(float scale);
        float getPlaceholderScale() const;

        /// Enable/disable conversion of :emoji: names and unicode emoji characters into sprites
        void setDisplayEmojis(bool allow);

        /// Hides the background of this input. Shorthand for
        /// `input->getBGSprite()->setVisible(false)`
        void hideBG();

        /// Set a callback function that is called when the user types in the input.
        void setCallback(Function<void(std::string const& value)> onInput);

        /// Set a callback function that is called when Enter is pressed.
        void setEnterCallback(Function<void(std::string const& value)> onEnter);

        /// Set a callback function that is called when text input is focused or unfocused.
        void setFocusCallback(Function<void(FocusState state)> onFocusChange);

        /// Set a callback function that is called when text input may be covered by floating keyboard and get the offset.
        void setKeyboardOffsetCallback(Function<void(float offset)> onKeyboardOffset);

        /// Set a callback function that is called when the floating keyboard is hidden.
        void setKeyboardHideCallback(Function<void()> onKeyboardHide);

        template <class Callback>
        void addTextChangedCallback(ZStringView id, Callback&& callback, int priority = 0) {
            this->addEventListener(id, TextChangedEvent(this), std::forward<Callback>(callback), priority);
        }

        template <class Callback>
        void addFocusCallback(ZStringView id, Callback&& callback, int priority = 0) {
            this->addEventListener(id, FocusEvent(this, FocusState::Focused), std::forward<Callback>(callback), priority);
        }

        template <class Callback>
        void addUnfocusCallback(ZStringView id, Callback&& callback, int priority = 0) {
            this->addEventListener(id, FocusEvent(this, FocusState::Unfocused), std::forward<Callback>(callback), priority);
        }

        /// Set the value of the input
        /// @param str The new text of the input
        /// @param triggerCallback Whether this should trigger the callback or not
        void setString(std::string_view str, bool triggerCallback = false);

        /// Get the current value of the input
        std::string const& getString() const;

        /// Focus this input (activate the cursor)
        void focus();

        /// Blurs this input (deactivate the cursor)
        void unfocus();

        bool isFocused() const;

        Label* getContentLabel() const;
        Label* getPlaceholderLabel() const;
        Label* getCaptionLabel() const;

        NineSlice* getBGSprite() const;

    protected:
        bool canAttachWithIME() override;
        bool canDetachWithIME() override;
        void didAttachWithIME() override;
        void didDetachWithIME() override;
        void insertText(char const* text, int len, cocos2d::enumKeyCodes key) override;
        void deleteBackward() override;
        void deleteForward() override;
        char const* getContentText() override;

        void keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo& info) override;
        void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo& info) override;

        bool initWithWidth(float width, std::string placeholder, ZStringView font);
        bool initWithSize(cocos2d::CCSize const& size, std::string placeholder, ZStringView font, bool multiline);

    private:
        void applyKeyboardOffset(cocos2d::CCRect keyboardRect);
        void returnKeyboardOffset();

        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };

    class GEODE_DLL ContextMenu : public NineSlice, public cocos2d::CCTouchDelegate {
    public:
        ContextMenu();
        ~ContextMenu() override;

        static ContextMenu* create();

        void onEnter() override;
        void onExit() override;

        void addItem(ZStringView name, Function<void()> callback, ZStringView description = "");
        void addSeparator();

        void show(cocos2d::CCPoint pos);

        static void closeAll();

        bool init() override;
        bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}