#include <Geode/DefaultInclude.hpp>
#include <Geode/ui/TextArea.hpp>

namespace geode {
    using namespace geode::prelude;
    
    class SimpleTextAreaImpl {
    public:
        bool m_shouldUpdate = false;
        bool m_artificialWidth = false;
        cocos2d::CCMenu* m_container = nullptr;
        std::string m_font;
        std::string m_text;
        std::vector<cocos2d::CCLabelBMFont*> m_lines;
        cocos2d::ccColor4B m_color = { 0xFF, 0xFF, 0xFF, 0xFF };
        cocos2d::CCTextAlignment m_alignment = cocos2d::kCCTextAlignmentLeft;
        WrappingMode m_wrappingMode = WORD_WRAP;
        size_t m_maxLines = 0;
        float m_scale = 1.f;
        float m_lineHeight = 0.f;
        float m_linePadding = 0.f;

        SimpleTextArea* m_self = nullptr;

        SimpleTextAreaImpl(SimpleTextArea* self) : m_self(self) {}

        cocos2d::CCLabelBMFont* createLabel(char const* text, float top) {
            if (m_maxLines && m_lines.size() >= m_maxLines) {
                CCLabelBMFont* last = m_lines.at(m_maxLines - 1);
                std::string_view textv = last->getString();

                last->setString(fmt::format("{}...", textv.substr(0, textv.size() - 3)).c_str());

                return nullptr;
            } else {
                CCLabelBMFont* label = CCLabelBMFont::create(text, m_font.c_str());

                label->setScale(m_scale);
                label->setPosition({ 0, top });
                label->setColor({ m_color.r, m_color.g, m_color.b });
                label->setOpacity(m_color.a);

                return label;
            }
        }

        float calculateOffset(cocos2d::CCLabelBMFont* label) {
            return m_linePadding + label->getContentSize().height * m_scale;
        }

        virtual void charIteration(geode::FunctionRef<cocos2d::CCLabelBMFont*(cocos2d::CCLabelBMFont* line, char c, float top)> overflowHandling) {
            float top = 0;
            m_lines.clear();
            CCLabelBMFont* line = createLabel("", top);
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

        void updateLinesNoWrap() {
            std::stringstream stream(m_text);
            std::string part;
            float top = 0;
            m_lines.clear();

            while (std::getline(stream, part)) {
                CCLabelBMFont* line = createLabel(part.c_str(), top);

                if (line == nullptr) {
                    break;
                } else {
                    top -= calculateOffset(line);

                    m_lines.push_back(line);
                }
            }
        }

        void updateLinesWordWrap(bool spaceWrap) {
            charIteration([this, spaceWrap](CCLabelBMFont* line, char c, float top) {
                const std::string_view delimiters(spaceWrap ? " " : " `~!@#$%^&*()-_=+[{}];:'\",<.>/?\\|");

                if (delimiters.find(c) == std::string_view::npos) {
                    const std::string& text = line->getString();
                    const size_t position = text.find_last_of(delimiters) + 1;
                    CCLabelBMFont* newLine = createLabel((text.substr(position) + c).c_str(), top);

                    if (newLine != nullptr) {
                        line->setString(text.substr(0, position).c_str());
                    }

                    return newLine;
                } else {
                    return createLabel(std::string(c, c != ' ').c_str(), top);
                }
            });
        }

        void updateLinesCutoffWrap() {
            charIteration([this](CCLabelBMFont* line, char c, float top) {
                const std::string& text = line->getString();
                const char back = text.back();
                const bool lastIsSpace = back == ' ';
                CCLabelBMFont* newLine = createLabel(std::string(!lastIsSpace, back).append(std::string(c != ' ', c)).c_str(), top);

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

        void updateContainer() {
            switch (m_wrappingMode) {
                case NO_WRAP: {
                    updateLinesNoWrap();
                } break;
                case WORD_WRAP: {
                    updateLinesWordWrap(false);
                } break;
                case SPACE_WRAP: {
                    updateLinesWordWrap(true);
                } break;
                case CUTOFF_WRAP: {
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

            for (CCLabelBMFont* line : m_lines) {
                const float y = height + line->getPositionY();

                switch (m_alignment) {
                    case kCCTextAlignmentLeft: {
                        line->setAnchorPoint({ 0, 1 });
                        line->setPosition({ 0, y });
                    } break;
                    case kCCTextAlignmentCenter: {
                        line->setAnchorPoint({ 0.5f, 1 });
                        line->setPosition({ width / 2, y });
                    } break;
                    case kCCTextAlignmentRight: {
                        line->setAnchorPoint({ 1, 1 });
                        line->setPosition({ width, y });
                    } break;
                }

                m_container->addChild(line);
            }
        }
    };
}