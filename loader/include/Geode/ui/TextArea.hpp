#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/function.hpp>
#include <memory>
#include <cocos2d.h>

namespace geode {
    class SimpleTextAreaImpl;

    enum WrappingMode {
        NO_WRAP,
        WORD_WRAP,
        SPACE_WRAP,
        CUTOFF_WRAP
    };

    /**
     * A class which provides a textarea with proper alignment and some extra features like:
     *
     * - Max lines
     * - Changing all aspects after creation
     * - Custom text alignment
     * - Configurable and automatic word wrapping
     * - Line padding
     *
     * Contact me on Discord (\@smjs) if you have any questions, suggestions or bugs.
    */

    class GEODE_DLL SimpleTextArea : public cocos2d::CCNode {
    public:
        static SimpleTextArea* create(std::string text, std::string font = "chatFont.fnt", float scale = 1.0f);
        static SimpleTextArea* create(std::string text, std::string font, float scale, float width);

        void setFont(std::string font);
        std::string getFont();
        void setColor(const cocos2d::ccColor4B& color);
        cocos2d::ccColor4B getColor();
        void setAlignment(cocos2d::CCTextAlignment alignment);
        cocos2d::CCTextAlignment getAlignment();
        void setWrappingMode(WrappingMode mode);
        WrappingMode getWrappingMode();
        void setText(std::string text);
        std::string getText();
        void setMaxLines(size_t maxLines);
        size_t getMaxLines();
        void setWidth(float width);
        float getWidth();
        void setScale(float scale) override;
        float getScale() override;
        void setLinePadding(float padding);
        float getLinePadding();
        std::vector<cocos2d::CCLabelBMFont*> getLines();
        float getHeight();
        float getLineHeight();

    protected:
        SimpleTextArea();
        ~SimpleTextArea() override;

        virtual std::unique_ptr<SimpleTextAreaImpl> createImpl();
        std::unique_ptr<SimpleTextAreaImpl> m_impl;

        bool init(std::string font, std::string text, float scale, float width, const bool artificialWidth);
    private:
        static SimpleTextArea* create(std::string font, std::string text, float scale, float width, const bool artificialWidth);
    };

    // abb2k wuz here :)

    template <class T>
    class RichTextKey;

    class RichTextKeyInstanceBase {
    public:
        virtual ~RichTextKeyInstanceBase() = default;
        virtual void applyChangesToSprite(cocos2d::CCFontSprite* spr, int localIndex, int index) = 0;
        virtual std::string getKey() const = 0;
        virtual bool isCancellation() const = 0;
        virtual std::string runStrAddition() = 0;
        virtual bool isButton() const = 0;
        virtual void callButton(bool keyDown, cocos2d::CCFontSprite* spr, std::set<cocos2d::CCFontSprite*> const& word) = 0;
    };

    template <class T>
    class RichTextKeyInstance final : public RichTextKeyInstanceBase {
    public:
        RichTextKeyInstance(RichTextKey<T>* key, T data, bool cancellation)
            : m_key(std::move(key)), m_value(std::move(data)), m_cancellation(std::move(cancellation)) {}

        RichTextKey<T>* m_key = nullptr;
        T m_value;
        bool m_cancellation = false;

        void applyChangesToSprite(cocos2d::CCFontSprite* spr, int localIndex, int index) override {
            if (m_key->m_applyToSprite != NULL)
                m_key->m_applyToSprite(m_value, spr, localIndex, index);
        }

        std::string getKey() const override {
            return m_key->getKey();
        }

        bool isCancellation() const override {
            return m_cancellation;
        }

        std::string runStrAddition() override {
            if (m_key->m_stringAddition != NULL)
                return m_key->m_stringAddition(m_value);
            return "";
        }

        bool isButton() const override {
            return m_key->m_buttonFunctionallity != NULL;
        }

        void callButton(bool keyDown, cocos2d::CCFontSprite* spr, std::set<cocos2d::CCFontSprite*> const& word){
            if (m_key->m_buttonFunctionallity != NULL)
                m_key->m_buttonFunctionallity(m_value, keyDown, spr, word);
        }
    };

    class RichTextKeyBase {
    public:
        virtual ~RichTextKeyBase() = default;
        virtual Result<std::shared_ptr<RichTextKeyInstanceBase>> createInstance(std::string const& value, bool cancellation) = 0;
        virtual std::string getKey() const = 0;
    };

    template <class T>
    class RichTextKey final : public RichTextKeyBase {
    public:
        /**
            @param key The identifier name for this rich text key
            @param validCheck Function to validate and parse the value string into type T (if an error is returned the key will not be processed)
            @param applyToSprite Function to apply the parsed value to a font sprite (optional)
        */
        RichTextKey(
            std::string key,
            geode::Function<Result<T>(std::string const& value)> validCheck,
            geode::Function<void(
                T const& value,
                bool keyDown,
                cocos2d::CCFontSprite* specificSpriteClicked,
                std::set<cocos2d::CCFontSprite*> const& wordClicked)> buttonFunctionallity
        ) : m_key(std::move(key)),
            m_validCheck(std::move(validCheck)),
            m_buttonFunctionallity(std::move(buttonFunctionallity)) {}
        /**
            @param key The identifier name for this rich text key
            @param validCheck Function to validate and parse the value string into type T (if an error is returned the key will not be processed)
            @param applyToSprite Function to apply the parsed value to a font sprite (optional)
        */
        RichTextKey(
            std::string key,
            geode::Function<Result<T>(std::string const& value)> validCheck,
            geode::Function<void(T const& value, cocos2d::CCFontSprite* sprite, int localIndex, int charIndex)> applyToSprite
        ) : m_key(std::move(key)),
            m_validCheck(std::move(validCheck)),
            m_applyToSprite(std::move(applyToSprite)) {}
        /**
            @param key The identifier name for this rich text key
            @param validCheck Function to validate and parse the value string into type T (if an error is returned the key will not be processed)
            @param stringAddition Function to add a new string at the point where the key is (optional)
        */
        RichTextKey(
            std::string key,
            geode::Function<Result<T>(std::string const& value)> validCheck,
            geode::Function<std::string(T const& value)> stringAddition
        ) : m_key(std::move(key)),
            m_validCheck(std::move(validCheck)),
            m_stringAddition(std::move(stringAddition)) {}
        /**
            @param key The identifier name for this rich text key
            @param validCheck Function to validate and parse the value string into type T (if an error is returned the key will not be processed)
            @param applyToSprite Function to apply the parsed value to a font sprite (optional)
            @param stringAddition Function to add a new string at the point where the key is (optional)
        */
        RichTextKey(
            std::string key,
            geode::Function<Result<T>(std::string const& value)> validCheck,
            geode::Function<void(T const& value, cocos2d::CCFontSprite* sprite, int localIndex, int charIndex)> applyToSprite,
            geode::Function<std::string(T const& value)> stringAddition
        ) : m_key(std::move(key)),
            m_validCheck(std::move(validCheck)),
            m_applyToSprite(std::move(applyToSprite)),
            m_stringAddition(std::move(stringAddition)) {}

        Result<std::shared_ptr<RichTextKeyInstanceBase>> createInstance(std::string const& value, bool cancellation) override {
            if (cancellation){
                if (value == ""){
                    return Ok(std::make_shared<RichTextKeyInstance<T>>(
                        RichTextKeyInstance<T>(this, T(), true))
                    );
                }
                else return Err("Cancellation tags cannot have values");
            }

            auto res = m_validCheck(value);

            if (res.isErr()) return Err(res.unwrapErr());

            return Ok(std::make_shared<RichTextKeyInstance<T>>(
                RichTextKeyInstance<T>(this, res.unwrap(), false))
            );
        }

        std::string getKey() const override {
            return m_key;
        }

    private:
        std::string m_key;

    public:
        geode::Function<Result<T>(std::string const& value)> m_validCheck = NULL;
        geode::Function<void(T const& value, cocos2d::CCFontSprite* sprite, int localIndex, int charIndex)> m_applyToSprite = NULL;
        geode::Function<std::string(T const& value)> m_stringAddition = NULL;
        geode::Function<void(
                T const& value,
                bool keyDown,
                cocos2d::CCFontSprite* specificSpriteClicked,
                std::set<cocos2d::CCFontSprite*> const& wordClicked)> m_buttonFunctionallity = NULL;
    };


    class GEODE_DLL RichTextArea : public SimpleTextArea, public cocos2d::CCTouchDelegate {
    public:
        static RichTextArea* create(std::string text, std::string font = "chatFont.fnt", float scale = 1.0f);
        static RichTextArea* create(std::string text, std::string font, float scale, float width);

        std::string getRawText();

        void registerRichTextKey(std::shared_ptr<RichTextKeyBase> key);
    protected:
        RichTextArea();
        ~RichTextArea();

        class RichImpl;
        std::unique_ptr<SimpleTextAreaImpl> createImpl() override;

    private:
        static RichTextArea* create(std::string font, std::string text, float scale, float width, const bool artificialWidth);

        bool init(std::string font, std::string text, float scale, float width, const bool artificialWidth);

        bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) override;

        void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) override;
        void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) override;

        void onEnter() override;
        void onExit() override;

        RichImpl* castedImpl();
    };
}
