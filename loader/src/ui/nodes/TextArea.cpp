#include <Geode/ui/TextArea.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/cocos/robtop/sprite_nodes/CCFontSprite.h>
#include <regex>
#include <fmt/chrono.h>
#include <asp/iter.hpp>

class geode::SimpleTextAreaImpl {
public:
    bool m_shouldUpdate = false;
    bool m_artificialWidth = false;
    cocos2d::CCMenu* m_container = nullptr;
    std::string m_font;
    std::string m_text;
    std::vector<cocos2d::CCLabelBMFont*> m_lines;
    cocos2d::ccColor4B m_color = { 0xFF, 0xFF, 0xFF, 0xFF };
    cocos2d::CCTextAlignment m_alignment = cocos2d::kCCTextAlignmentLeft;
    geode::WrappingMode m_wrappingMode = geode::WrappingMode::WORD_WRAP;
    size_t m_maxLines = 0;
    float m_scale = 1.f;
    float m_lineHeight = 0.f;
    float m_linePadding = 0.f;

    geode::SimpleTextArea* m_self = nullptr;

    SimpleTextAreaImpl(geode::SimpleTextArea* self) : m_self(self) {}

    cocos2d::CCLabelBMFont* createLabel(std::string text, float top);

    float calculateOffset(cocos2d::CCLabelBMFont* label);

    virtual void charIteration(geode::FunctionRef<cocos2d::CCLabelBMFont*(cocos2d::CCLabelBMFont* line, char c, float top)> overflowHandling);

    void updateLinesNoWrap();

    void updateLinesWordWrap(bool spaceWrap);

    void updateLinesCutoffWrap();

    void updateContainer();

    virtual void setTextImpl(std::string text);
};

cocos2d::CCLabelBMFont* geode::SimpleTextAreaImpl::createLabel(std::string text, float top) {
    if (m_maxLines && m_lines.size() >= m_maxLines) {
        cocos2d::CCLabelBMFont* last = m_lines.at(m_maxLines - 1);
        std::string_view textv = last->getString();

        last->setString(fmt::format("{}...", textv.substr(0, textv.size() - 3)).c_str());

        return nullptr;
    } else {
        cocos2d::CCLabelBMFont* label = cocos2d::CCLabelBMFont::create(std::move(text).c_str(), m_font.c_str());

        label->setScale(m_scale);
        label->setPosition({ 0, top });
        label->setColor({ m_color.r, m_color.g, m_color.b });
        label->setOpacity(m_color.a);

        return label;
    }
}

float geode::SimpleTextAreaImpl::calculateOffset(cocos2d::CCLabelBMFont* label) {
    return m_linePadding + label->getContentSize().height * m_scale;
}

void geode::SimpleTextAreaImpl::charIteration(geode::FunctionRef<cocos2d::CCLabelBMFont*(cocos2d::CCLabelBMFont* line, char c, float top)> overflowHandling) {
    float top = 0;
    m_lines.clear();
    cocos2d::CCLabelBMFont* line = createLabel("", top);
    m_lines = { line };

    for (const char c : m_text) {
        if (c == '\n') {
            line = createLabel("", top -= calculateOffset(line));

            if (line == nullptr) {
                break;
            } else {
                m_lines.push_back(line);
            }
        } else if (m_artificialWidth && line->getContentWidth() * m_scale >= m_self->getWidth()) {
            line = overflowHandling(line, c, top -= calculateOffset(line));

            if (line == nullptr) {
                break;
            } else {
                m_lines.push_back(line);
            }
        } else {
            line->setString((std::string(line->getString()) + c).c_str());
        }
    }
}

void geode::SimpleTextAreaImpl::updateLinesNoWrap() {
    std::stringstream stream(m_text);
    std::string part;
    float top = 0;
    m_lines.clear();

    while (std::getline(stream, part)) {
        cocos2d::CCLabelBMFont* line = createLabel(std::move(part), top);
        part.clear();

        if (line == nullptr) {
            break;
        } else {
            top -= calculateOffset(line);

            m_lines.push_back(line);
        }
    }
}

void geode::SimpleTextAreaImpl::updateLinesWordWrap(bool spaceWrap) {
    charIteration([this, spaceWrap](cocos2d::CCLabelBMFont* line, char c, float top) {
        const std::string_view delimiters(spaceWrap ? " " : " `~!@#$%^&*()-_=+[{}];:'\",<.>/?\\|");

        if (!delimiters.contains(c)) {
            const std::string& text = line->getString();
            const size_t position = text.find_last_of(delimiters) + 1;
            cocos2d::CCLabelBMFont* newLine = createLabel(text.substr(position) + c, top);

            if (newLine != nullptr) {
                line->setString(text.substr(0, position).c_str());
            }

            return newLine;
        } else {
            return createLabel(std::string(c != ' ', c).c_str(), top);
        }
    });
}

void geode::SimpleTextAreaImpl::updateLinesCutoffWrap() {
    charIteration([this](cocos2d::CCLabelBMFont* line, char c, float top) {
        const std::string& text = line->getString();
        const char back = text.back();
        const bool lastIsSpace = back == ' ';
        cocos2d::CCLabelBMFont* newLine = createLabel(std::string(!lastIsSpace, back).append(1, c).c_str(), top);

        if (newLine != nullptr && !lastIsSpace) {
            line->setString(text.substr(0, text.size() - 1).c_str());
        } else if (newLine == nullptr && !lastIsSpace) {
            if (text[text.size() - 2] == ' ') {
                line->setString(text.substr(0, text.size() - 1).c_str());
            } else {
                line->setString((text.substr(0, text.size() - 1) + '-').c_str());
            }
        }

        return newLine;
});

}

void geode::SimpleTextAreaImpl::updateContainer() {
    switch (m_wrappingMode) {
        case geode::NO_WRAP: {
            updateLinesNoWrap();
        } break;
        case geode::WORD_WRAP: {
            updateLinesWordWrap(false);
        } break;
        case geode::SPACE_WRAP: {
            updateLinesWordWrap(true);
        } break;
        case geode::CUTOFF_WRAP: {
            updateLinesCutoffWrap();
        } break;
    }

    const size_t lineCount = m_lines.size();
    const float width = m_self->getWidth();

    if (lineCount > 0) {
        m_lineHeight = m_lines.back()->getContentSize().height * m_scale;
    } else {
        m_lineHeight = 0;
    }

    const float height = m_lineHeight * lineCount + m_linePadding * (lineCount - 1);

    m_self->setContentSize({ width, height });
    m_container->setContentSize(m_self->getContentSize());
    m_container->removeAllChildren();

    for (cocos2d::CCLabelBMFont* line : m_lines) {
        const float y = height + line->getPositionY();

        switch (m_alignment) {
            case cocos2d::kCCTextAlignmentLeft: {
                line->setAnchorPoint({ 0, 1 });
                line->setPosition({ 0, y });
            } break;
            case cocos2d::kCCTextAlignmentCenter: {
                line->setAnchorPoint({ 0.5f, 1 });
                line->setPosition({ width / 2, y });
            } break;
            case cocos2d::kCCTextAlignmentRight: {
                line->setAnchorPoint({ 1, 1 });
                line->setPosition({ width, y });
            } break;
        }

        m_container->addChild(line);
    }
}

void geode::SimpleTextAreaImpl::setTextImpl(std::string text){
    m_text = std::move(text);
    updateContainer();
}

geode::SimpleTextArea::SimpleTextArea() = default;
geode::SimpleTextArea::~SimpleTextArea() = default;

geode::SimpleTextArea* geode::SimpleTextArea::create(std::string text, std::string font, float scale) {
    return SimpleTextArea::create(
        std::move(font), std::move(text), scale,
        cocos2d::CCDirector::sharedDirector()->getWinSize().width / 2,
        false
    );
}

geode::SimpleTextArea* geode::SimpleTextArea::create(std::string text, std::string font, float scale, float width) {
    return SimpleTextArea::create(
        std::move(font), std::move(text), scale, width, true
    );
}

geode::SimpleTextArea* geode::SimpleTextArea::create(std::string font, std::string text, float scale, float width, bool artificialWidth) {
    SimpleTextArea* instance = new SimpleTextArea();
    instance->m_impl = instance->createImpl();

    if (instance->init(std::move(font), std::move(text), scale, width, artificialWidth)) {
        instance->autorelease();
        return instance;
    }

    delete instance;
    return nullptr;
}

bool geode::SimpleTextArea::init(std::string font, std::string text, float scale, float width, bool artificialWidth) {
    m_impl->m_font = std::move(font);
    m_impl->m_text = std::move(text);
    m_impl->m_scale = scale;
    m_impl->m_artificialWidth = artificialWidth;
    m_impl->m_container = cocos2d::CCMenu::create();

    this->setAnchorPoint({ 0.5f, 0.5f });
    m_impl->m_container->setPosition({ 0, 0 });
    m_impl->m_container->setAnchorPoint({ 0, 1 });
    m_impl->m_container->setContentSize({ width, 0 });
    this->addChild(m_impl->m_container);
    m_impl->updateContainer();

    return true;
}

void geode::SimpleTextArea::setFont(std::string font) {
    m_impl->m_font = std::move(font);
    m_impl->updateContainer();
}

std::string geode::SimpleTextArea::getFont() {
    return m_impl->m_font;
}

void geode::SimpleTextArea::setColor(const cocos2d::ccColor4B& color) {
    m_impl->m_color = color;
    m_impl->updateContainer();
}

cocos2d::ccColor4B geode::SimpleTextArea::getColor() {
    return m_impl->m_color;
}

void geode::SimpleTextArea::setAlignment(cocos2d::CCTextAlignment alignment) {
    m_impl->m_alignment = alignment;
    m_impl->updateContainer();
}

cocos2d::CCTextAlignment geode::SimpleTextArea::getAlignment() {
    return m_impl->m_alignment;
}

void geode::SimpleTextArea::setWrappingMode(WrappingMode mode) {
    m_impl->m_wrappingMode = mode;
    m_impl->updateContainer();
}

geode::WrappingMode geode::SimpleTextArea::getWrappingMode() {
    return m_impl->m_wrappingMode;
}

void geode::SimpleTextArea::setText(std::string text) {
    m_impl->setTextImpl(std::move(text));
}

std::string geode::SimpleTextArea::getText() {
    return m_impl->m_text;
}

void geode::SimpleTextArea::setMaxLines(size_t maxLines) {
    m_impl->m_maxLines = maxLines;
    m_impl->updateContainer();
}

size_t geode::SimpleTextArea::getMaxLines() {
    return m_impl->m_maxLines;
}

void geode::SimpleTextArea::setWidth(float width) {
    m_impl->m_artificialWidth = true;
    m_impl->updateContainer();

    this->setContentSize({ width, this->getContentSize().height });
    m_impl->m_container->setContentSize(this->getContentSize());
}

float geode::SimpleTextArea::getWidth() {
    return m_impl->m_container->getContentSize().width;
}

void geode::SimpleTextArea::setScale(float scale) {
    m_impl->m_scale = scale;
    m_impl->updateContainer();
}

float geode::SimpleTextArea::getScale() {
    return m_impl->m_scale;
}

void geode::SimpleTextArea::setLinePadding(float padding) {
    m_impl->m_linePadding = padding;
    m_impl->updateContainer();
}

float geode::SimpleTextArea::getLinePadding() {
    return m_impl->m_linePadding;
}

std::vector<cocos2d::CCLabelBMFont*> geode::SimpleTextArea::getLines() {
    return m_impl->m_lines;
}

float geode::SimpleTextArea::getHeight() {
    return m_impl->m_container->getContentSize().height;
}

float geode::SimpleTextArea::getLineHeight() {
    return m_impl->m_lineHeight;
}

inline std::unique_ptr<geode::SimpleTextAreaImpl> geode::SimpleTextArea::createImpl() {
    return std::make_unique<geode::SimpleTextAreaImpl>(this);
}

using namespace geode::prelude;

class geode::RichTextArea::RichImpl : public SimpleTextAreaImpl {
public:
    geode::RichTextArea* m_self = nullptr;
    RichImpl(RichTextArea* self) : m_self(self), SimpleTextAreaImpl(self) {}

    std::map<std::string, std::shared_ptr<RichTextKeyBase>> m_richTextKeys;
    std::map<int, std::vector<std::shared_ptr<RichTextKeyInstanceBase>>> m_richTextInstances;
    std::map<int, std::set<std::string>> m_specialExitCancellations;
    std::map<std::shared_ptr<RichTextKeyInstanceBase>, std::set<CCFontSprite*>> m_charactersForButton{};
    std::shared_ptr<RichTextKeyInstanceBase> m_currentlyHeldButton = nullptr;

    std::string m_rawText;

    std::map<CCFontSprite*, ccColor3B> m_ogColorForLink{};

    void charIteration(geode::FunctionRef<cocos2d::CCLabelBMFont*(cocos2d::CCLabelBMFont* line, char c, float top)> overflowHandling) override;
    void formatRichText();

    void processLinkClick(
        std::string const& link,
        bool keyDown,
        cocos2d::CCFontSprite* specificSpriteClicked,
        std::set<cocos2d::CCFontSprite*> const& wordClicked
    );

    void setTextImpl(std::string text) override;
};

RichTextArea* RichTextArea::create(std::string text, std::string font, float scale) {
    return RichTextArea::create(std::move(font), std::move(text), scale, CCDirector::sharedDirector()->getWinSize().width / 2, false);
}

RichTextArea* RichTextArea::create(std::string text, std::string font, float scale, float width) {
    return RichTextArea::create(std::move(font), std::move(text), scale, width, true);
}

RichTextArea* RichTextArea::create(std::string font, std::string text, float scale, float width, bool artificialWidth) {
    RichTextArea* instance = new RichTextArea();
    instance->m_impl = instance->createImpl();

    if (instance->init(std::move(font), std::move(text), scale, width, artificialWidth)) {
        instance->autorelease();
        return instance;
    }

    delete instance;
    return nullptr;
}

bool RichTextArea::init(std::string font, std::string text, float scale, float width, bool artificialWidth) {
    if (!SimpleTextArea::init(font, text, scale, width, artificialWidth)) return false;

    registerRichTextKey(std::make_shared<RichTextKey<ccColor4B>>(
        "color",
        [](std::string value) -> Result<ccColor4B> {
            auto colorRes = cc4bFromHexString(value);
            if (colorRes.isErr()) return Err(colorRes.unwrapErr());

            return Ok(colorRes.unwrap());
        },
        [](ccColor4B const& value, cocos2d::CCFontSprite* sprite, int localIndex, int charIndex) {
            sprite->setColor({ value.r, value.g, value.b });
            sprite->setOpacity(value.a);
        }
    ));

    registerRichTextKey(std::make_shared<RichTextKey<bool>>(
        "flip",
        [](std::string value) -> Result<bool> {
            if (value == "") return Ok(true);

            if (value != "true" && value != "false") {
                return Err("Value must be 'true' or 'false'");
            }

            return Ok(value == "true");
        },
        [](bool const& value, cocos2d::CCFontSprite* sprite, int localIndex, int charIndex) {
            sprite->setFlipY(value);
        }
    ));

    registerRichTextKey(std::make_shared<RichTextKey<bool>>(
        "mirror",
        [](std::string value) -> Result<bool> {
            if (value == "") return Ok(true);

            if (value != "true" && value != "false") {
                return Err("Value must be 'true' or 'false'");
            }

            return Ok(value == "true");
        },
        [](bool const& value, cocos2d::CCFontSprite* sprite, int localIndex, int charIndex) {
            sprite->setFlipX(value);
        }
    ));

    registerRichTextKey(std::make_shared<RichTextKey<std::time_t>>(
        "workingTime",
        [](std::string value) -> Result<std::time_t> {
            auto timeRes = geode::utils::numFromString<time_t>(value);
            if (timeRes.isErr()) return Ok(std::time(nullptr));

            return Ok(timeRes.unwrap());
        },
        [](std::time_t const& value) -> std::string {
            return fmt::format("{:%Y-%m-%d %H:%M:%S}", geode::localtime(value));
        }
    ));

    registerRichTextKey(std::make_shared<RichTextKey<float>>(
        "size",
        [](std::string value) -> Result<float> {
            auto numRes = geode::utils::numFromString<float>(value);
            if (numRes.isErr()) return Err("size invalid!");

            return Ok(numRes.unwrap());
        },
        [](float const& value, cocos2d::CCFontSprite* sprite, int localIndex, int charIndex) {
            sprite->setScale(value);
        }
    ));

    auto makeQuickColorKey = [&](std::string const& key, cocos2d::ccColor3B const& color) {
        auto colorKey = std::make_shared<RichTextKey<std::monostate>>(
            key,
            [](std::string value) -> Result<std::monostate> {
                if (!value.empty()) return Err("Must be empty!");

                return Ok(std::monostate());
            },
            [color](std::monostate const& value, cocos2d::CCFontSprite* sprite, int localIndex, int charIndex) {
                sprite->setColor(color);
            }
        );
        colorKey->setSpecialExits({"c"});
        
        registerRichTextKey(colorKey);
    };

    makeQuickColorKey("cb", { 74, 82, 225 });
    makeQuickColorKey("cg", { 64, 227, 72 });
    makeQuickColorKey("cl", { 96, 171, 239 });
    makeQuickColorKey("cj", { 50, 200, 255 });
    makeQuickColorKey("cy", { 255, 255, 0 });
    makeQuickColorKey("co", { 255, 90, 75 });
    makeQuickColorKey("cr", { 255, 90, 90 });
    makeQuickColorKey("cp", { 255, 0, 255 });
    makeQuickColorKey("ca", { 150, 50, 255 });
    makeQuickColorKey("cd", { 255, 150, 255 });
    makeQuickColorKey("cc", { 255, 255, 150 });
    makeQuickColorKey("cf", { 150, 255, 255 });
    makeQuickColorKey("cs", { 255, 220, 65 });
    makeQuickColorKey("c_", { 255, 0, 0 });

    // maybe one day someone will make this work but its fine :(
    // registerRichTextKey(std::make_shared<RichTextKey<std::string>>(
    //     "font",
    //     [](std::string value) -> Result<std::string> {
    //         auto temp = cocos2d::CCLabelBMFont::create("Test", value.c_str());
    //         if (temp == nullptr) return Err("Font file isn't valid!");

    //         return Ok(value);
    //     },
    //     [&](std::string const& value, CCFontSprite* const applyToSpr) {
    //         applyToSpr->retain();
    //         applyToSpr->removeFromParent();
    //         this->addChild(applyToSpr);
    //         applyToSpr->release();
    //     }
    // ));

    registerRichTextKey(std::make_shared<RichTextKey<std::string>>(
        "link",
        [](std::string value) -> Result<std::string> {
            return Ok(value);
        },
        [&](std::string const& value, bool keyDown, cocos2d::CCFontSprite* specificSpriteClicked, std::set<cocos2d::CCFontSprite*> const& wordClicked) {
            this->castedImpl()->processLinkClick(value, keyDown, specificSpriteClicked, wordClicked);
        }
    ));

    castedImpl()->m_rawText = m_impl->m_text;

    castedImpl()->formatRichText();
    m_impl->updateContainer();

    return true;
}

std::string RichTextArea::getRawText(){
    return castedImpl()->m_rawText;
}

void RichTextArea::updateContent(){
    castedImpl()->formatRichText();
    castedImpl()->updateContainer();
}

std::vector<std::shared_ptr<RichTextKeyBase>> RichTextArea::getTextKeys(){
    if (!castedImpl()->m_richTextKeys.size()) return {};

    std::vector<std::shared_ptr<RichTextKeyBase>> toReturn{};
    toReturn.reserve(castedImpl()->m_richTextKeys.size());

    for (auto const& [key, val] : castedImpl()->m_richTextKeys) {
        toReturn.push_back(val);
    }

    return std::move(toReturn);
}

void RichTextArea::RichImpl::charIteration(geode::FunctionRef<cocos2d::CCLabelBMFont*(cocos2d::CCLabelBMFont* line, char c, float top)> overflowHandling) {
    float top = 0;
    m_lines.clear();
    cocos2d::CCLabelBMFont* line = this->createLabel("", top);
    m_lines = { line };

    std::map<std::string, std::shared_ptr<RichTextKeyInstanceBase>> appliedRichTextInstances{};
    std::map<std::string, size_t> activationOrder;
    size_t nextActivationOrder = 0;

    std::map<std::shared_ptr<RichTextKeyInstanceBase>, int> m_charIndexForInstance{};

    using CharacterEffect = std::pair<std::shared_ptr<RichTextKeyInstanceBase>, int>;

    std::vector<std::vector<CharacterEffect>> effectsForCharacter;
    std::map<cocos2d::CCLabelBMFont*, std::vector<size_t>> characterIndicesForLine{};
    characterIndicesForLine[line] = {};

    m_charactersForButton.clear();

    size_t index = 0;
    for (const char& c : m_text) {
        if (m_richTextInstances.contains(index)) {
            auto instances = m_richTextInstances[index];

            for (const auto& instance : instances) {
                auto key = instance->getKey();

                if (instance->isCancellation()) {
                    auto active = appliedRichTextInstances.find(key);
                    
                    if (active != appliedRichTextInstances.end()) {
                        m_charIndexForInstance.erase(active->second);
                        appliedRichTextInstances.erase(active);
                        activationOrder.erase(key);
                    }

                    continue;
                }

                appliedRichTextInstances[key] = instance;
                m_charIndexForInstance[instance] = 0;
                activationOrder[key] = nextActivationOrder++;
            }
        }

        if (m_specialExitCancellations.contains(index)) {
            const std::string* latestKey = nullptr;
            size_t latestOrder = 0;
            
            for (const auto& key : m_specialExitCancellations[index]) {
                auto active = appliedRichTextInstances.find(key);
                auto order = activationOrder.find(key);
                
                if (active != appliedRichTextInstances.end() && order != activationOrder.end() && (latestKey == nullptr || order->second > latestOrder)) {
                    latestKey = &key;
                    latestOrder = order->second;
                }
            }

            if (latestKey != nullptr) {
                auto active = appliedRichTextInstances.find(*latestKey);
                m_charIndexForInstance.erase(active->second);
                appliedRichTextInstances.erase(active);
                activationOrder.erase(*latestKey);
            }
        }

        std::vector<CharacterEffect> characterEffects{};
        characterEffects.reserve(appliedRichTextInstances.size());

        for (const auto& [key, instancePtr] : appliedRichTextInstances) {
            characterEffects.emplace_back(instancePtr, m_charIndexForInstance[instancePtr]);
        }
        
        effectsForCharacter.push_back(std::move(characterEffects));
        auto characterIndex = index;
        index++;

        if (c == '\n') {
            line = this->createLabel("", top -= this->calculateOffset(line));

            if (line == nullptr) {
                break;
            }
            else {
                m_lines.push_back(line);
                characterIndicesForLine[line] = {};
            }
        }
        else if (m_artificialWidth && line->getContentWidth() * m_scale >= m_self->getWidth()) {
            cocos2d::CCLabelBMFont* oldLine = line;
            std::string oldText = line->getString();
            auto oldCharacterIndices = characterIndicesForLine[line];
            line = overflowHandling(line, c, top -= this->calculateOffset(line));

            if (line == nullptr) {
                break;
            }
            else {
                m_lines.push_back(line);

                std::string newText = line->getString();
                bool currentCharacterWasRendered = c != ' ' && !newText.empty() && newText.back() == c;

                auto movedText = newText;
                
                if (currentCharacterWasRendered) {
                    movedText.pop_back();
                }

                auto movedCharacterCount = std::min(oldText.size(), movedText.size());
                while (movedCharacterCount > 0 && oldText.compare(oldText.size() - movedCharacterCount, movedCharacterCount, movedText, 0, movedCharacterCount) != 0) {
                    movedCharacterCount--;
                }
                movedCharacterCount = std::min(movedCharacterCount, oldCharacterIndices.size());

                std::vector<size_t>& remainingCharacterIndices = characterIndicesForLine[oldLine];
                remainingCharacterIndices.resize(std::min(oldCharacterIndices.size(), std::string(oldLine->getString()).size()));

                std::vector<size_t>& newCharacterIndices = characterIndicesForLine[line];
                newCharacterIndices.insert(
                    newCharacterIndices.end(),
                    oldCharacterIndices.end() - movedCharacterCount,
                    oldCharacterIndices.end()
                );

                if (currentCharacterWasRendered) {
                    newCharacterIndices.push_back(characterIndex);
                }

                size_t characterIndexInLine = 0;
                for (auto sprite : CCArrayExt<cocos2d::CCFontSprite*>(line->getChildren())) {
                    if (characterIndexInLine >= newCharacterIndices.size()) {
                        break;
                    }

                    auto sourceCharacterIndex = newCharacterIndices[characterIndexInLine++];
                    
                    for (const auto& [instancePtr, localIndex] : effectsForCharacter[sourceCharacterIndex]) {
                        instancePtr->applyChangesToSprite(sprite, localIndex, static_cast<int>(sourceCharacterIndex + 1));

                        if (instancePtr->isButton() && !instancePtr->isCancellation()) {
                            m_charactersForButton[instancePtr].insert(sprite);
                        }
                    }
                }

                if (currentCharacterWasRendered) {
                    for (const auto& [key, instancePtr] : appliedRichTextInstances) {
                        m_charIndexForInstance[instancePtr]++;
                    }
                }
            }
        }
        else {
            line->setString((std::string(line->getString()) + c).c_str());

            characterIndicesForLine[line].push_back(characterIndex);
            if (line->getChildren()->lastObject() != nullptr) {
                auto lastChar = static_cast<cocos2d::CCFontSprite*>(line->getChildren()->lastObject());

                for (const auto& [instancePtr, localIndex] : effectsForCharacter[characterIndex]) {
                    instancePtr->applyChangesToSprite(lastChar, localIndex, static_cast<int>(characterIndex + 1));

                    if (instancePtr->isButton() && !instancePtr->isCancellation()) {
                        m_charactersForButton[instancePtr].insert(lastChar);
                    }
                }

                for (const auto& [key, instancePtr] : appliedRichTextInstances) {
                    m_charIndexForInstance[instancePtr]++;
                }
            }
        }
    }
}

void RichTextArea::RichImpl::formatRichText() {
    std::regex pattern(R"(<(\/)?([^=<>]+)(?:\s*=\s*([^<>]+))?>)");
    std::smatch match;

    m_richTextInstances.clear();
    m_specialExitCancellations.clear();

    // theres probably a better way to do this lol idk aa
    struct MatchInfo {
        int position;
        ptrdiff_t length;
        std::string key;
        std::string value;
        int overallOffset;
        bool cancellation;
    };
    std::vector<MatchInfo> matches;

    auto textToModify = m_rawText;

    auto begin = textToModify.cbegin();
    auto end = textToModify.cend();

    int offset = 0;

    while (std::regex_search(begin, end, match, pattern)) {
        int matchStartPos = std::distance(textToModify.cbegin(), match[0].first);

        std::string value = "";
        if (match.size() >= 4 && match[3].matched) value = match[3];

        matches.push_back({matchStartPos, match[0].length(), match[2], value, offset, match[1] == '/'});
        offset += match[0].length();

        begin = match.suffix().first;
    }

    std::map<int, std::vector<std::shared_ptr<RichTextKeyInstanceBase>>> richTextInstancesBeforeExtraOffset{};
    std::map<int, std::set<std::string>> specialExitCancellationsBeforeExtraOffset{};

    for (auto& m : asp::iter::fromReverse(matches)) {
        bool keyIsContained = m_richTextKeys.contains(m.key);

        if (!keyIsContained && m.cancellation) {
            const int effectIndex = m.position - m.overallOffset;
            for (const auto& [keyStr, keyBaseRef] : m_richTextKeys) {
                if (keyBaseRef && keyBaseRef->getSpecialExits().contains(m.key)) {
                    specialExitCancellationsBeforeExtraOffset[effectIndex].insert(keyStr);
                }
            }
        }
        else if (!keyIsContained) {
            continue;
        }
        else {
            int effectIndex = m.position - m.overallOffset;
            auto it = m_richTextKeys.find(m.key);
            if (it != m_richTextKeys.end() && it->second) {
                auto result = it->second->createInstance(m.value, m.cancellation);
                if (result.isOk()) {
                    richTextInstancesBeforeExtraOffset[effectIndex].push_back(result.unwrap());
                }
            }
        }

        textToModify.erase(m.position, m.length);
    }

    int textAdditionOverallOffset = 0;
    int prevExtraOffset = 0;

    std::set<int> effectIndices;
    for (const auto& [index, keys] : richTextInstancesBeforeExtraOffset) {
        effectIndices.insert(index);
    }
    for (const auto& [index, keys] : specialExitCancellationsBeforeExtraOffset) {
        effectIndices.insert(index);
    }

    for (const int index : effectIndices) {
        auto keyInstances = richTextInstancesBeforeExtraOffset.find(index);
        if (keyInstances != richTextInstancesBeforeExtraOffset.end()) {
            for (const auto& keyRef : keyInstances->second) {
                auto currentAddition = keyRef->runStrAddition();
                if (currentAddition.empty()) continue;
                textToModify.insert(index + textAdditionOverallOffset, currentAddition);
                textAdditionOverallOffset += currentAddition.length();
            }

            m_richTextInstances[index + prevExtraOffset] = std::move(keyInstances->second);
        }

        auto specialExit = specialExitCancellationsBeforeExtraOffset.find(index);
        if (specialExit != specialExitCancellationsBeforeExtraOffset.end()) {
            m_specialExitCancellations[index + prevExtraOffset] = std::move(specialExit->second);
        }

        prevExtraOffset = textAdditionOverallOffset;
    }

    m_text = std::move(textToModify);
}

void RichTextArea::RichImpl::processLinkClick(
    std::string const& link,
    bool keyDown,
    cocos2d::CCFontSprite* specificSpriteClicked,
    std::set<cocos2d::CCFontSprite*> const& wordClicked
){
    if (keyDown){
        for (const auto& linkCharacter : wordClicked)
        {
            this->m_ogColorForLink.insert({
                linkCharacter,
                linkCharacter->getColor() == ccColor3B{255, 255, 255} ?
                    ccColor3B{m_self->getColor().r, m_self->getColor().g, m_self->getColor().b} :
                    linkCharacter->getColor()
                }
            );

            linkCharacter->setColor({ 78, 78, 255 });
        }
    }
    else {
        for (const auto& linkCharacter : wordClicked) {
            if (this->m_ogColorForLink.contains(linkCharacter)) {
                linkCharacter->setColor(this->m_ogColorForLink[linkCharacter]);
            }
        }

        if (specificSpriteClicked != nullptr)
            web::openLinkInBrowser(link);
    }
}

void RichTextArea::RichImpl::setTextImpl(std::string text) {
    m_text = std::move(text);
    m_rawText = m_text;
    formatRichText();
    updateContainer();
}

void RichTextArea::registerRichTextKey(std::shared_ptr<RichTextKeyBase> key){
    if (this->castedImpl()->m_richTextKeys.contains(key->getKey())) return;

    this->castedImpl()->m_richTextKeys.insert({key->getKey(), key});

    this->castedImpl()->formatRichText();
    this->castedImpl()->updateContainer();
}

void RichTextArea::unregisterRichTextKey(std::string const& keyName) {
    if (!this->castedImpl()->m_richTextKeys.contains(keyName)) return;

    this->castedImpl()->m_richTextKeys.erase(keyName);
    
    this->castedImpl()->formatRichText();
    this->castedImpl()->updateContainer();
}

bool RichTextArea::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    for (const auto& [btnKey, textForBtn] : this->castedImpl()->m_charactersForButton)
    {
        for (const auto& fontSpr : textForBtn)
        {
            if (fontSpr == nullptr) continue;

            auto touchInSpace = fontSpr->convertTouchToNodeSpace(pTouch);
            if (touchInSpace.x > 0 && touchInSpace.y > 0 && touchInSpace.x <= fontSpr->getContentWidth() && touchInSpace.y <= fontSpr->getContentHeight()){
                btnKey->callButton(true, fontSpr, textForBtn);
                this->castedImpl()->m_currentlyHeldButton = btnKey;
                return true;
            }
        }
    }

    return false;
}

void RichTextArea::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (this->castedImpl()->m_charactersForButton.contains(this->castedImpl()->m_currentlyHeldButton)){
        auto textForBtn = this->castedImpl()->m_charactersForButton[this->castedImpl()->m_currentlyHeldButton];

        for (const auto& fontSpr : textForBtn)
        {
            if (fontSpr == nullptr) continue;

            auto touchInSpace = fontSpr->convertTouchToNodeSpace(pTouch);
            if (touchInSpace.x > 0 && touchInSpace.y > 0 && touchInSpace.x <= fontSpr->getContentWidth() && touchInSpace.y <= fontSpr->getContentHeight()){
                this->castedImpl()->m_currentlyHeldButton->callButton(false, fontSpr, textForBtn);
                this->castedImpl()->m_currentlyHeldButton = nullptr;
                return;
            }
        }

        this->castedImpl()->m_currentlyHeldButton->callButton(false, nullptr, textForBtn);
    }

    this->castedImpl()->m_currentlyHeldButton = nullptr;
}
void RichTextArea::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
    RichTextArea::ccTouchEnded(pTouch, pEvent);
}

RichTextArea::RichTextArea() : SimpleTextArea() {}

RichTextArea::~RichTextArea() = default;


inline std::unique_ptr<SimpleTextAreaImpl> RichTextArea::createImpl() {
    return std::make_unique<RichTextArea::RichImpl>(this);
}

RichTextArea::RichImpl* RichTextArea::castedImpl() {
    return static_cast<RichTextArea::RichImpl*>(m_impl.get());
}

void RichTextArea::onEnter(){
    CCNode::onEnter();
    CCTouchDispatcher::get()->addTargetedDelegate(this, 0, true);
}

void RichTextArea::onExit(){
    CCTouchDispatcher::get()->removeDelegate(this);
    CCNode::onExit();
}