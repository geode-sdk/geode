#include <Geode/ui/RichTextArea.hpp>
#include <regex>
#include <chrono>
#include <fmt/chrono.h>

using namespace geode::prelude;
class RichTextArea::RichImpl : public SimpleTextArea::Impl {
public:
    template <class T>
    void registerRichTextKey(std::shared_ptr<RichTextKey<T>> key);

    void charIteration(geode::FunctionRef<cocos2d::CCLabelBMFont*(cocos2d::CCLabelBMFont* line, char c, float top)> overflowHandling) override;

    void formatRichText();

    RichImpl(RichTextArea* self) : SimpleTextArea::Impl(self) {}

    std::map<std::string, std::shared_ptr<RichTextKeyBase>> m_richTextKeys;
    std::map<int, std::vector<std::shared_ptr<RichTextKeyInstanceBase>>> m_richTextInstances;
    
};

RichTextArea* RichTextArea::create(std::string text, std::string font, float scale) {
    return RichTextArea::create(std::move(font), std::move(text), scale, CCDirector::sharedDirector()->getWinSize().width / 2, false);
}

RichTextArea* RichTextArea::create(std::string text, std::string font, float scale, float width) {
    return RichTextArea::create(std::move(font), std::move(text), scale, width, true);
}

RichTextArea* RichTextArea::create(std::string font, std::string text, float scale, float width, bool artificialWidth) {
    RichTextArea* instance = new RichTextArea();
    instance->m_impl = std::make_unique<RichTextArea::RichImpl>(instance);

    if (instance->init(std::move(font), std::move(text), scale, width, artificialWidth)) {
        instance->autorelease();
        return instance;
    }

    delete instance;
    return nullptr;
}

bool RichTextArea::init(std::string font, std::string text, float scale, float width, bool artificialWidth) {
    if (!SimpleTextArea::init(font, text, scale, width, artificialWidth)) return false;

    static_cast<RichTextArea::RichImpl*>(m_impl.get())->registerRichTextKey<ccColor3B>(std::make_shared<RichTextKey<ccColor3B>>(
        "color",
        [](std::string value) -> Result<ccColor3B> {
            auto colorRes = cc3bFromHexString(value);
            if (colorRes.isErr()) return Err(colorRes.unwrapErr());

            return Ok(colorRes.unwrap());
        },
        [](const ccColor3B& value, cocos2d::CCFontSprite* sprite) {
            sprite->setColor({ value.r, value.g, value.b });
        }
    ));

    static_cast<RichTextArea::RichImpl*>(m_impl.get())->registerRichTextKey<bool>(std::make_shared<RichTextKey<bool>>(
        "flip",
        [](std::string value) -> Result<bool> {
            if (value == "") return Ok(true);

            if (value != "true" && value != "false") {
                return Err("Value must be 'true' or 'false'");
            }

            return Ok(value == "true");
        },
        [](const bool& value, cocos2d::CCFontSprite* sprite) {
            sprite->setFlipY(value);
        }
    ));

    static_cast<RichTextArea::RichImpl*>(m_impl.get())->registerRichTextKey<bool>(std::make_shared<RichTextKey<bool>>(
        "mirror",
        [](std::string value) -> Result<bool> {
            if (value == "") return Ok(true);
            
            if (value != "true" && value != "false") {
                return Err("Value must be 'true' or 'false'");
            }

            return Ok(value == "true");
        },
        [](const bool& value, cocos2d::CCFontSprite* sprite) {
            sprite->setFlipX(value);
        }
    ));

    static_cast<RichTextArea::RichImpl*>(m_impl.get())->registerRichTextKey<std::time_t>(std::make_shared<RichTextKey<std::time_t>>(
        "workingTime",
        [](std::string value) -> Result<std::time_t> {
            auto timeRes = geode::utils::numFromString<time_t>(value);
            if (timeRes.isErr()) return Ok(std::time(nullptr));

            return Ok(timeRes.unwrap());
        },
        [](const std::time_t& value) -> std::string {
            return "fmt::format(\"{:%Y-%m-%d %H:%M:%S}\", fmt::localtime(value))";
        }
    ));

    static_cast<RichTextArea::RichImpl*>(m_impl.get())->formatRichText();

    return true;
}

void RichTextArea::RichImpl::charIteration(geode::FunctionRef<cocos2d::CCLabelBMFont*(cocos2d::CCLabelBMFont* line, char c, float top)> overflowHandling) {
    float top = 0;
    m_lines.clear();
    CCLabelBMFont* line = this->createLabel("", top);
    m_lines = { line };

    log::info("char it text");

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
                instancePtr->applyChangesToSprite(static_cast<cocos2d::CCFontSprite*>(line->getChildren()->lastObject()));
            }
        }
    }
}

void RichTextArea::RichImpl::formatRichText() {
    std::regex pattern(R"(<(\/)?([^=<>]+)(?:\s*=\s*([^<>]+))?>)");
    std::smatch match;

    log::info("formatting text");

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

template <class T>
void RichTextArea::RichImpl::registerRichTextKey(std::shared_ptr<RichTextKey<T>> key){
    if (m_richTextKeys.contains(key->getKey())) return;

    m_richTextKeys.insert({key->getKey(), key});
}

template <class T>
void RichTextKeyInstance<T>::applyChangesToSprite(cocos2d::CCFontSprite* spr) {
    if (m_key->m_applyToSprite != NULL)
        m_key->m_applyToSprite(m_value, spr);
}

template <class T>
std::string RichTextKeyInstance<T>::runStrAddition() {
    if (m_key->m_stringAddition != NULL)
        return m_key->m_stringAddition(m_value);
    return "";
}

template <class T>
Result<std::shared_ptr<RichTextKeyInstanceBase>> RichTextKey<T>::createInstance(const std::string& value, bool cancellation) {
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
    static_cast<RichTextArea::RichImpl*>(m_impl.get())->formatRichText();
    m_impl->updateContainer();
}
