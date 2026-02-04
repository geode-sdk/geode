#include "SimpleTextAreaImpl.hpp"
#include <Geode/ui/TextArea.hpp>

using namespace geode::prelude;
class SimpleTextArea::Impl : public SimpleTextAreaImpl {
public:
    SimpleTextArea* m_self = nullptr;
    Impl(SimpleTextArea* self) : SimpleTextAreaImpl(self) {}
};

SimpleTextArea::SimpleTextArea() : m_impl(createImpl()) {}
SimpleTextArea::~SimpleTextArea() = default;

SimpleTextArea* SimpleTextArea::create(std::string text, std::string font, float scale) {
    return SimpleTextArea::create(
        std::move(font), std::move(text), scale,
        CCDirector::sharedDirector()->getWinSize().width / 2,
        false
    );
}

SimpleTextArea* SimpleTextArea::create(std::string text, std::string font, float scale, float width) {
    return SimpleTextArea::create(
        std::move(font), std::move(text), scale, width, true
    );
}

SimpleTextArea* SimpleTextArea::create(std::string font, std::string text, float scale, float width, bool artificialWidth) {
    SimpleTextArea* instance = new SimpleTextArea();
    instance->m_impl = instance->createImpl();

    if (instance->init(std::move(font), std::move(text), scale, width, artificialWidth)) {
        instance->autorelease();
        return instance;
    }

    delete instance;
    return nullptr;
}

bool SimpleTextArea::init(std::string font, std::string text, float scale, float width, bool artificialWidth) {
    m_impl->m_font = std::move(font);
    m_impl->m_text = std::move(text);
    m_impl->m_scale = scale;
    m_impl->m_artificialWidth = artificialWidth;
    m_impl->m_container = CCMenu::create();

    this->setAnchorPoint({ 0.5f, 0.5f });
    m_impl->m_container->setPosition({ 0, 0 });
    m_impl->m_container->setAnchorPoint({ 0, 1 });
    m_impl->m_container->setContentSize({ width, 0 });
    this->addChild(m_impl->m_container);
    m_impl->updateContainer();

    return true;
}

void SimpleTextArea::setFont(std::string font) {
    m_impl->m_font = std::move(font);
    m_impl->updateContainer();
}

std::string SimpleTextArea::getFont() {
    return m_impl->m_font;
}

void SimpleTextArea::setColor(const ccColor4B& color) {
    m_impl->m_color = color;
    m_impl->updateContainer();
}

ccColor4B SimpleTextArea::getColor() {
    return m_impl->m_color;
}

void SimpleTextArea::setAlignment(CCTextAlignment alignment) {
    m_impl->m_alignment = alignment;
    m_impl->updateContainer();
}

CCTextAlignment SimpleTextArea::getAlignment() {
    return m_impl->m_alignment;
}

void SimpleTextArea::setWrappingMode(WrappingMode mode) {
    m_impl->m_wrappingMode = mode;
    m_impl->updateContainer();
}

WrappingMode SimpleTextArea::getWrappingMode() {
    return m_impl->m_wrappingMode;
}

void SimpleTextArea::setText(std::string text) {
    m_impl->m_text = std::move(text);
    m_impl->updateContainer();
}

std::string SimpleTextArea::getText() {
    return m_impl->m_text;
}

void SimpleTextArea::setMaxLines(size_t maxLines) {
    m_impl->m_maxLines = maxLines;
    m_impl->updateContainer();
}

size_t SimpleTextArea::getMaxLines() {
    return m_impl->m_maxLines;
}

void SimpleTextArea::setWidth(float width) {
    m_impl->m_artificialWidth = true;
    m_impl->updateContainer();

    this->setContentSize({ width, this->getContentSize().height });
    m_impl->m_container->setContentSize(this->getContentSize());
}

float SimpleTextArea::getWidth() {
    return m_impl->m_container->getContentSize().width;
}

void SimpleTextArea::setScale(float scale) {
    m_impl->m_scale = scale;
    m_impl->updateContainer();
}

float SimpleTextArea::getScale() {
    return m_impl->m_scale;
}

void SimpleTextArea::setLinePadding(float padding) {
    m_impl->m_linePadding = padding;
    m_impl->updateContainer();
}

float SimpleTextArea::getLinePadding() {
    return m_impl->m_linePadding;
}

std::vector<CCLabelBMFont*> SimpleTextArea::getLines() {
    return m_impl->m_lines;
}

float SimpleTextArea::getHeight() {
    return m_impl->m_container->getContentSize().height;
}

float SimpleTextArea::getLineHeight() {
    return m_impl->m_lineHeight;
}

std::unique_ptr<SimpleTextAreaImpl> SimpleTextArea::createImpl(){
    return std::make_unique<SimpleTextArea::Impl>(this);
}