#pragma once

#include <Geode/ui/TextArea.hpp>

namespace geode {

    // abb2k wuz here :)

    template <class T>
    class RichTextKey;

    class RichTextKeyInstanceBase {
    public:
        virtual ~RichTextKeyInstanceBase() = default;
        virtual void applyChangesToSprite(cocos2d::CCFontSprite* spr) = 0;
        virtual std::string getKey() const = 0;
        virtual bool isCancellation() const = 0;
        virtual std::string runStrAddition() = 0;
    };

    template <class T>
    class RichTextKeyInstance final : public RichTextKeyInstanceBase {
    public:
        RichTextKeyInstance(RichTextKey<T>* key, T data, bool cancellation)
            : m_key(std::move(key)), m_value(std::move(data)), m_cancellation(std::move(cancellation)) {}

        RichTextKey<T>* m_key = nullptr;
        T m_value;
        bool m_cancellation = false;

        void applyChangesToSprite(cocos2d::CCFontSprite* spr) override;

        std::string getKey() const override {
            return m_key->getKey();
        }

        bool isCancellation() const override {
            return m_cancellation;
        }

        std::string runStrAddition() override;
    };

    class RichTextKeyBase {
    public:
        virtual ~RichTextKeyBase() = default;
        virtual Result<std::shared_ptr<RichTextKeyInstanceBase>> createInstance(const std::string& value, bool cancellation) = 0;
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
            geode::Function<Result<T>(const std::string& value)> validCheck, 
            geode::Function<void(const T& value, cocos2d::CCFontSprite* sprite)> applyToSprite
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
            geode::Function<Result<T>(const std::string& value)> validCheck, 
            geode::Function<std::string(const T& value)> stringAddition
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
            geode::Function<Result<T>(const std::string& value)> validCheck, 
            geode::Function<void(const T& value, cocos2d::CCFontSprite* sprite)> applyToSprite,
            geode::Function<std::string(const T& value)> stringAddition
        ) : m_key(std::move(key)),
            m_validCheck(std::move(validCheck)),
            m_applyToSprite(std::move(applyToSprite)),
            m_stringAddition(std::move(stringAddition)) {}
        
        Result<std::shared_ptr<RichTextKeyInstanceBase>> createInstance(const std::string& value, bool cancellation) override;

        std::string getKey() const override {
            return m_key;
        }
        
    private:
        std::string m_key;

    public:
        geode::Function<Result<T>(const std::string& value)> m_validCheck = NULL;
        geode::Function<void(const T& value, cocos2d::CCFontSprite* sprite)> m_applyToSprite = NULL;
        geode::Function<std::string(const T& value)> m_stringAddition = NULL;
    };


    class GEODE_DLL RichTextArea : public SimpleTextArea {
    public:
        static RichTextArea* create(std::string text, std::string font = "chatFont.fnt", float scale = 1.0f);
        static RichTextArea* create(std::string text, std::string font, float scale, float width);

        void setText(std::string text) override;
    
    private:
        static RichTextArea* create(std::string font, std::string text, float scale, float width, const bool artificialWidth);

        bool init(std::string font, std::string text, float scale, float width, const bool artificialWidth);

        class RichImpl;
    };
}
