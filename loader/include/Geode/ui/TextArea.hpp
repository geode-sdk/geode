#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/function.hpp>
#include <cocos2d.h>

namespace geode {
    enum WrappingMode {
        NO_WRAP,
        WORD_WRAP,
        SPACE_WRAP,
        CUTOFF_WRAP
    };

    // abb2k wuz here :)

    class RichTextKeyBase;
    template <typename T>
    class RichTextKey;

    class RichTextKeyInstanceBase {
    public:
        virtual ~RichTextKeyInstanceBase() = default;
        virtual void applyChangesToSprite(cocos2d::CCFontSprite* spr) = 0;
        virtual std::string getKey() const = 0;
        virtual bool isCancellation() const = 0;
        virtual std::string runStrAddition() = 0;
    };

    template <typename T>
    class RichTextKeyInstance : public RichTextKeyInstanceBase {
    public:
        RichTextKeyInstance(RichTextKey<T>* key, T data, bool cancellation = false)
            : key(std::move(key)), value(std::move(data)), cancellation(std::move(cancellation)) {}

        RichTextKey<T>* key;
        T value;
        bool cancellation;

        void applyChangesToSprite(cocos2d::CCFontSprite* spr) override;

        std::string getKey() const override {
            return key->getKey();
        }

        bool isCancellation() const override {
            return cancellation;
        }

        std::string runStrAddition() override;
    };

    class RichTextKeyBase {
    public:
        virtual ~RichTextKeyBase() = default;
        virtual Result<std::shared_ptr<RichTextKeyInstanceBase>> createInstance(const std::string& value, bool cancellation) = 0;
        virtual std::string getKey() const = 0;
    };

    template <typename T>
    class RichTextKey : public RichTextKeyBase {
        public:
            /**
             * aaa
             * @param key The identifier name for this rich text key
             * @param validCheck Function to validate and parse the value string into type T (if an error is returned the key will not be processed)
             * @param applyToSprite Function to apply the parsed value to a font sprite (optional)
             * @param stringAddition Function to add a new string at the point where the key is (optional)
            */
            RichTextKey(std::string key, geode::Function<Result<T>(const std::string& value)> validCheck, geode::Function<void(const T& value, cocos2d::CCFontSprite* sprite)> applyToSprite = NULL, geode::Function<std::string(const T& value)> stringAddition = NULL)
                : key(std::move(key)), validCheck(std::move(validCheck)), applyToSprite(std::move(applyToSprite)), stringAddition(std::move(stringAddition)) {}
            
            Result<std::shared_ptr<RichTextKeyInstanceBase>> createInstance(const std::string& value, bool cancellation) override;

            std::string getKey() const override {
                return key;
            }
            
        private:
            std::string key;

        public:
            geode::Function<Result<T>(const std::string& value)> validCheck;
            geode::Function<void(const T& value, cocos2d::CCFontSprite* sprite)> applyToSprite;
            geode::Function<std::string(const T& value)> stringAddition;
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
        void setRichTextEnabled(bool enabled);
        bool isRichTextEnabled();

        template <typename T>
        void registerRichTextKey(std::shared_ptr<RichTextKey<T>> key);
    private:
        static SimpleTextArea* create(std::string font, std::string text, float scale, float width, const bool artificialWidth);

        bool init(std::string font, std::string text, float scale, float width, const bool artificialWidth);

        bool m_shouldUpdate = false;
        bool m_artificialWidth = false;
        cocos2d::CCMenu* m_container = nullptr;
        std::string m_font;
        std::string m_text;
        std::string m_textFormatted;
        std::vector<cocos2d::CCLabelBMFont*> m_lines;
        cocos2d::ccColor4B m_color = { 0xFF, 0xFF, 0xFF, 0xFF };
        cocos2d::CCTextAlignment m_alignment = cocos2d::kCCTextAlignmentLeft;
        WrappingMode m_wrappingMode = WORD_WRAP;
        size_t m_maxLines = 0;
        float m_scale = 1.f;
        float m_lineHeight = 0.f;
        float m_linePadding = 0.f;
        bool richTextEnabled = true;

        cocos2d::CCLabelBMFont* createLabel(char const* text, float top);
        float calculateOffset(cocos2d::CCLabelBMFont* label);
        void charIteration(geode::FunctionRef<cocos2d::CCLabelBMFont*(cocos2d::CCLabelBMFont* line, char c, float top)> overflowHandling);
        void updateLinesNoWrap();
        void updateLinesWordWrap(bool spaceWrap);
        void updateLinesCutoffWrap();
        void updateContainer();

        std::string* getCorrectText(){
            return richTextEnabled ? &m_textFormatted : &m_text;
        }

        void formatRichText();

        std::map<std::string, std::shared_ptr<RichTextKeyBase>> richTextKeys;
        std::map<int, std::vector<std::shared_ptr<RichTextKeyInstanceBase>>> richTextInstances;
    };
}
