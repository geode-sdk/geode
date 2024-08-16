#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/binding/TextInputDelegate.hpp>
#include <Geode/binding/CCTextInputNode.hpp>
#include <cocos2d.h>

namespace geode {
    enum class CommonFilter {
        // Allow an unsigned integer
        Uint,
        // Allow a signed integer
        Int,
        // Allow a floating point number
        Float,
        // Allow letters, numbers, dashes, underscores, and dots
        ID,
        // Allow word-like characters & spaces
        Name,
        // Allows basically anything possible to type in an input
        Any,
        // Allow a hexadecimal number
        Hex,
        // Allow a non-URL-safe Base64 number
        Base64Normal,
        // Allow a URL-safe Base64 number
        Base64URL,
    };

    GEODE_DLL const char* getCommonFilterAllowedChars(CommonFilter filter);

    enum class TextInputAlign {
        Center,
        Left,
    };

    /**
     * A single-line text input node
     */
    class GEODE_DLL TextInput : public cocos2d::CCNode, public TextInputDelegate {
    protected:
        cocos2d::extension::CCScale9Sprite* m_bgSprite;
        CCTextInputNode* m_input;
        std::function<void(std::string const&)> m_onInput = nullptr;
        cocos2d::CCLabelBMFont* m_label = nullptr;

        bool init(float width, std::string const& placeholder, std::string const& font);

        void textChanged(CCTextInputNode* input) override;

    public:
        /**
         * Create a single-line text input with a background.
         * Can either be used in delegate or callback mode; 
         * with callback mode, you don't need to deal with adding 
         * TextInputDelegate to your class' base list, you just install a 
         * callback function directly to the input itself
         * @param width The width of the input
         * @param placeholder Placeholder text for the input
         * @param font The font to use 
         */
        static TextInput* create(float width, std::string const& placeholder, std::string const& font = "bigFont.fnt");

        /**
         * Set the placeholder label for this input
         */
        void setPlaceholder(std::string const& placeholder);
        /**
         * Set a label on this input that shows up on the top. Set an empty 
         * string to remove the label
         */
        void setLabel(std::string const& label);
        /**
         * Set the filter (allowed characters) for this input
         * @param allowedChars String of allowed characters; each character in 
         * the string represents one allowed character
         */
        void setFilter(std::string const& allowedChars);
        /**
         * Set a commonly used filter (number, text, etc.)
         */
        void setCommonFilter(CommonFilter filter);
        /**
         * Set the maximum amount of characters for this input. Use 0 for 
         * infinite length
         */
        void setMaxCharCount(size_t length);
        /**
         * Enable/disable password mode (all input characters are rendered as 
         * dots rather than the actual characters)
         */
        void setPasswordMode(bool enable);
        /**
         * Set the width of the label. This does not set the maximum character 
         * count; use `setMaxCharCount` for that
         */
        void setWidth(float width);
        /**
         * Install a delegate that handles input events. Removes any currently 
         * set direct callbacks
         * @param delegate The delegate to install
         * @param tag Some legacy delegates use a tag to distinguish between 
         * inputs; this is a convenience parameter for setting the tag of the 
         * internal CCTextInputNode for those cases
         */
        void setDelegate(TextInputDelegate* delegate, std::optional<int> tag = std::nullopt);
        /**
         * Set a direct callback function that is called when the user types in 
         * the input. Overrides any delegate that is currently installed
         * @param onInput Function to call when the user changes the value of 
         * the text input
         */
        void setCallback(std::function<void(std::string const&)> onInput);
        /**
         * Enable/disable the input
         */
        void setEnabled(bool enabled);
        /**
         * Align the button's content to the left. If false, aligns to the center
         */
        void setTextAlign(TextInputAlign align);

        /**
         * Hides the background of this input. Shorthand for 
         * `input->getBGSprite()->setVisible(false)`
         */
        void hideBG();

        /**
         * Set the value of the input
         * @param str The new text of the input
         * @param triggerCallback Whether this should trigger the callback 
         * function / delegate's textChanged event or not
         */
        void setString(std::string const& str, bool triggerCallback = false);
        /**
         * Get the current value of the input
         */
        std::string getString() const;

        /**
         * Focus this input (activate the cursor)
         */
        void focus();
        /**
         * Defocus this input (deactivate the cursor)
         */
        void defocus();

        CCTextInputNode* getInputNode() const;
        cocos2d::extension::CCScale9Sprite* getBGSprite() const;
    };
}
