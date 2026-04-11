#include <Geode/ui/TextArea.hpp>
#include <regex>
#include <fmt/chrono.h>

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

    cocos2d::CCLabelBMFont* createLabel(char const* text, float top);

    float calculateOffset(cocos2d::CCLabelBMFont* label);

    virtual void charIteration(geode::FunctionRef<cocos2d::CCLabelBMFont*(cocos2d::CCLabelBMFont* line, char c, float top)> overflowHandling);

    void updateLinesNoWrap();

    void updateLinesWordWrap(bool spaceWrap);

    void updateLinesCutoffWrap();

    void updateContainer();
};

cocos2d::CCLabelBMFont* geode::SimpleTextAreaImpl::createLabel(char const* text, float top) {
    if (m_maxLines && m_lines.size() >= m_maxLines) {
        cocos2d::CCLabelBMFont* last = m_lines.at(m_maxLines - 1);
        std::string_view textv = last->getString();

        last->setString(fmt::format("{}...", textv.substr(0, textv.size() - 3)).c_str());

        return nullptr;
    } else {
        cocos2d::CCLabelBMFont* label = cocos2d::CCLabelBMFont::create(text, m_font.c_str());

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
        cocos2d::CCLabelBMFont* line = createLabel(part.c_str(), top);

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

        if (delimiters.find(c) == std::string_view::npos) {
            const std::string& text = line->getString();
            const size_t position = text.find_last_of(delimiters) + 1;
            cocos2d::CCLabelBMFont* newLine = createLabel((text.substr(position) + c).c_str(), top);

            if (newLine != nullptr) {
                line->setString(text.substr(0, position).c_str());
            }

            return newLine;
        } else {
            return createLabel(std::string(c, c != ' ').c_str(), top);
        }
    });
}

void geode::SimpleTextAreaImpl::updateLinesCutoffWrap() {
    charIteration([this](cocos2d::CCLabelBMFont* line, char c, float top) {
        const std::string& text = line->getString();
        const char back = text.back();
        const bool lastIsSpace = back == ' ';
        cocos2d::CCLabelBMFont* newLine = createLabel(std::string(!lastIsSpace, back).append(std::string(c != ' ', c)).c_str(), top);

        if (newLine == nullptr && !lastIsSpace) {
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
    m_impl->m_text = std::move(text);
    m_impl->updateContainer();
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

class RichTextArea::RichImpl : public SimpleTextAreaImpl {
public:
    RichTextArea* m_self = nullptr;
    RichImpl(RichTextArea* self)  : m_self(self), SimpleTextAreaImpl(self) {}

    std::map<std::string, std::shared_ptr<RichTextKeyBase>> m_richTextKeys;
    std::map<int, std::vector<std::shared_ptr<RichTextKeyInstanceBase>>> m_richTextInstances;
    std::map<std::shared_ptr<RichTextKeyInstanceBase>, std::set<CCFontSprite*>> m_charactersForButton{};
    std::shared_ptr<RichTextKeyInstanceBase> m_currentlyHeldButton = nullptr;

    std::map<CCFontSprite*, ccColor3B> m_ogColorForLink{};

    void charIteration(geode::FunctionRef<cocos2d::CCLabelBMFont*(cocos2d::CCLabelBMFont* line, char c, float top)> overflowHandling) override;
    void formatRichText();

    void processLinkClick(
        std::string const& link,
        bool keyDown,
        cocos2d::CCFontSprite* specificSpriteClicked,
        std::set<cocos2d::CCFontSprite*> const& wordClicked
    );
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
    CCTouchDispatcher::get()->addTargetedDelegate(this, 0, true);

    if (!SimpleTextArea::init(font, text, scale, width, artificialWidth)) return false;

    registerRichTextKey<ccColor3B>(std::make_shared<RichTextKey<ccColor3B>>(
        "color",
        [](std::string value) -> Result<ccColor3B> {
            auto colorRes = cc3bFromHexString(value);
            if (colorRes.isErr()) return Err(colorRes.unwrapErr());

            return Ok(colorRes.unwrap());
        },
        [](ccColor3B const& value, cocos2d::CCFontSprite* sprite, int charIndex) {
            sprite->setColor({ value.r, value.g, value.b });
        }
    ));

    registerRichTextKey<bool>(std::make_shared<RichTextKey<bool>>(
        "flip",
        [](std::string value) -> Result<bool> {
            if (value == "") return Ok(true);

            if (value != "true" && value != "false") {
                return Err("Value must be 'true' or 'false'");
            }

            return Ok(value == "true");
        },
        [](bool const& value, cocos2d::CCFontSprite* sprite, int charIndex) {
            sprite->setFlipY(value);
        }
    ));

    registerRichTextKey<bool>(std::make_shared<RichTextKey<bool>>(
        "mirror",
        [](std::string value) -> Result<bool> {
            if (value == "") return Ok(true);

            if (value != "true" && value != "false") {
                return Err("Value must be 'true' or 'false'");
            }

            return Ok(value == "true");
        },
        [](bool const& value, cocos2d::CCFontSprite* sprite, int charIndex) {
            sprite->setFlipX(value);
        }
    ));

    registerRichTextKey<std::time_t>(std::make_shared<RichTextKey<std::time_t>>(
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

    registerRichTextKey<float>(std::make_shared<RichTextKey<float>>(
        "size",
        [](std::string value) -> Result<float> {
            auto numRes = geode::utils::numFromString<float>(value);
            if (numRes.isErr()) return Err("size invalid!");

            return Ok(numRes.unwrap());
        },
        [](float const& value, cocos2d::CCFontSprite* sprite, int charIndex) {
            sprite->setScale(value);
        }
    ));

    // maybe one day someone will make this work but its fine :(
    // registerRichTextKey<std::string>(std::make_shared<RichTextKey<std::string>>(
    //     "font",
    //     [](std::string value) -> Result<std::string> {
    //         auto temp = CCLabelBMFont::create("Test", value.c_str());
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

    registerRichTextKey<std::string>(std::make_shared<RichTextKey<std::string>>(
        "link",
        [](std::string value) -> Result<std::string> {
            return Ok(value);
        },
        [&](std::string const& value, bool keyDown, cocos2d::CCFontSprite* specificSpriteClicked, std::set<cocos2d::CCFontSprite*> const& wordClicked) {
            this->castedImpl()->processLinkClick(value, keyDown, specificSpriteClicked, wordClicked);
        }
    ));

    castedImpl()->formatRichText();
    m_impl->updateContainer();

    return true;
}

void RichTextArea::RichImpl::charIteration(geode::FunctionRef<cocos2d::CCLabelBMFont*(cocos2d::CCLabelBMFont* line, char c, float top)> overflowHandling) {
    float top = 0;
    m_lines.clear();
    CCLabelBMFont* line = this->createLabel("", top);
    m_lines = { line };

    std::map<std::string, std::shared_ptr<RichTextKeyInstanceBase>> appliedRichTextInstances{};

    int index = 0;
    for (const char& c : m_text) {
        if (m_richTextInstances.contains(index)){
            for (const auto& instancePtr : m_richTextInstances[index]) {
                if (appliedRichTextInstances.contains(instancePtr->getKey())) {
                    if (instancePtr->isCancellation()){
                        appliedRichTextInstances.erase(instancePtr->getKey());
                    }
                    else{
                        appliedRichTextInstances[instancePtr->getKey()] = instancePtr;
                    }
                } else {
                    appliedRichTextInstances.insert({instancePtr->getKey(), instancePtr});
                }
            }
        }

        index++;

        if (c == '\n') {
            line = this->createLabel("", top -= this->calculateOffset(line));

            if (line == nullptr) {
                break;
            } else {
                m_lines.push_back(line);
            }
        } else if (m_artificialWidth && line->getContentWidth() * m_scale >= m_self->getWidth()) {
            line = overflowHandling(line, c, top -= this->calculateOffset(line));

            if (line == nullptr) {
                break;
            } else {
                m_lines.push_back(line);
            }
        } else {
            line->setString((std::string(line->getString()) + c).c_str());
        }

        if (line->getChildren()->lastObject() != nullptr){
            for (const auto& [key, instancePtr] : appliedRichTextInstances) {
                auto lastChar = static_cast<cocos2d::CCFontSprite*>(line->getChildren()->lastObject());
                
                instancePtr->applyChangesToSprite(lastChar, index);

                if (instancePtr->isButton()){
                    if (!instancePtr->isCancellation()){
                        if (m_charactersForButton.contains(instancePtr)){
                            m_charactersForButton[instancePtr].insert(lastChar);
                        }
                        else{
                            m_charactersForButton.insert({instancePtr, {lastChar}});
                        }
                    }
                }

            }
        }
    }
}

void RichTextArea::RichImpl::formatRichText() {
    std::regex pattern(R"(<(\/)?([^=<>]+)(?:\s*=\s*([^<>]+))?>)");
    std::smatch match;

    m_richTextInstances.clear();

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

    auto begin = m_text.cbegin();
    auto end = m_text.cend();

    int offset = 0;

    while (std::regex_search(begin, end, match, pattern)) {
        int matchStartPos = std::distance(m_text.cbegin(), match[0].first);

        std::string value = "";
        if (match.size() >= 4 && match[3].matched) value = match[3];

        matches.push_back({matchStartPos, match[0].length(), match[2], value, offset, match[1] == '/'});
        offset += match[0].length();

        begin = match.suffix().first;
    }

    std::map<int, std::vector<std::shared_ptr<RichTextKeyInstanceBase>>> richTextInstancesBeforeExtraOffset{};

    for (auto it = matches.rbegin(); it != matches.rend(); ++it) {
        const auto& m = *it;

        if (!m_richTextKeys.contains(m.key)) continue;

        auto result = m_richTextKeys[m.key]->createInstance(m.value, m.cancellation);

        if (result.isErr()) continue;

        int effectIndex = m.position - m.overallOffset;

        auto& keyRef = result.unwrap();

        if (richTextInstancesBeforeExtraOffset.contains(effectIndex)) {
            richTextInstancesBeforeExtraOffset[effectIndex].push_back(keyRef);
        } else {
            richTextInstancesBeforeExtraOffset[effectIndex] = {keyRef};
        }

        m_text.erase(m.position, m.length);
    }

    int textAdditionOverallOffset = 0;
    int prevExtraOffset = 0;

    for (const auto& [index, keys] : richTextInstancesBeforeExtraOffset) {
        for (const auto& keyRef : keys)
        {
            auto currentAddition = keyRef->runStrAddition();
            if (currentAddition == "") continue;
            m_text.insert(index + textAdditionOverallOffset, currentAddition);
            textAdditionOverallOffset += currentAddition.length();
        }

        m_richTextInstances[index + prevExtraOffset] = std::move(keys);

        prevExtraOffset = textAdditionOverallOffset;
    }
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
            this->m_ogColorForLink.insert({linkCharacter, linkCharacter->getColor()});

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

template <class T>
void RichTextKeyInstance<T>::applyChangesToSprite(cocos2d::CCFontSprite* spr, int index) {
    if (m_key->m_applyToSprite != NULL)
        m_key->m_applyToSprite(m_value, spr, index);
}

template <class T>
std::string RichTextKeyInstance<T>::runStrAddition() {
    if (m_key->m_stringAddition != NULL)
        return m_key->m_stringAddition(m_value);
    return "";
}

template <class T>
Result<std::shared_ptr<RichTextKeyInstanceBase>> RichTextKey<T>::createInstance(std::string const& value, bool cancellation) {
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

void RichTextArea::setText(std::string text) {
    m_impl->m_text = std::move(text);
    castedImpl()->formatRichText();
    m_impl->updateContainer();
}

template <class T>
void RichTextArea::registerRichTextKey(std::shared_ptr<RichTextKey<T>> key){
    if (this->castedImpl()->m_richTextKeys.contains(key->getKey())) return;

    this->castedImpl()->m_richTextKeys.insert({key->getKey(), key});
}

bool RichTextArea::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    for (const auto& [btnKey, textForBtn] : this->castedImpl()->m_charactersForButton)
    {
        for (const auto& fontSpr : textForBtn)
        {
            auto touchInSpace = fontSpr->getParent()->convertTouchToNodeSpace(pTouch);
            if (fontSpr->boundingBox().containsPoint(touchInSpace)){
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
            auto touchInSpace = fontSpr->getParent()->convertTouchToNodeSpace(pTouch);
            if (fontSpr->boundingBox().containsPoint(touchInSpace)){
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

RichTextArea::~RichTextArea() {
    CCTouchDispatcher::get()->removeDelegate(this);
}

inline std::unique_ptr<SimpleTextAreaImpl> RichTextArea::createImpl() {
    return std::make_unique<RichTextArea::RichImpl>(this);
}

RichTextArea::RichImpl* RichTextArea::castedImpl() {
    return static_cast<RichTextArea::RichImpl*>(m_impl.get());
}