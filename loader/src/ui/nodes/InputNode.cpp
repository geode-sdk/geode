#include <Geode/ui/InputNode.hpp>

USE_GEODE_NAMESPACE();

const char* InputNode::getString() {
    return m_input->getTextField()->getString();
}

void InputNode::setString(const char* _str) {
    m_input->getTextField()->setString(_str);
    m_input->refreshLabel();
}

CCTextInputNode* InputNode::getInputNode() const {
    return m_input;
}

CCScale9Sprite* InputNode::getBGSprite() const {
    return m_bgSprite;
}

void InputNode::setEnabled(bool enabled) {
    m_input->setMouseEnabled(enabled);
    m_input->setTouchEnabled(enabled);
}

bool InputNode::init(float _w, float _h, const char* _phtxt, const char* _fnt, const std::string & _awc, int _cc) {
    m_bgSprite = cocos2d::extension::CCScale9Sprite::create(
        "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
    );

    m_bgSprite->setScale(.5f);
    m_bgSprite->setColor({ 0, 0, 0 });
    m_bgSprite->setOpacity(75);
    m_bgSprite->setContentSize({ _w * 2, _h * 2 });

    this->addChild(m_bgSprite);

    m_input = CCTextInputNode::create(
        _w - 10.0f, 60.0f, _phtxt, _fnt
    );

    m_input->setLabelPlaceholderColor({ 150, 150, 150 });
    m_input->setLabelPlaceholderScale(.75f);
    m_input->setMaxLabelScale(.8f);
    m_input->setMaxLabelWidth(_cc);
    if (_awc.length())
        m_input->setAllowedChars(_awc);

    this->addChild(m_input);

    return true;
}

bool InputNode::init(float _w, const char* _phtxt, const char* _fnt, const std::string & _awc, int _cc) {
    return init(_w, 30.0f, _phtxt, _fnt, _awc, _cc);
}

InputNode* InputNode::create(float _w, const char* _phtxt, const char* _fnt, const std::string & _awc, int _cc) {
    auto pRet = new InputNode();

    if (pRet && pRet->init(_w, _phtxt, _fnt, _awc, _cc)) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

InputNode* InputNode::create(float _w, const char* _phtxt, const std::string & _awc) {
    return create(_w, _phtxt, "bigFont.fnt", _awc, 69);
}

InputNode* InputNode::create(float _w, const char* _phtxt, const std::string & _awc, int _cc) {
    return create(_w, _phtxt, "bigFont.fnt", _awc, _cc);
}

InputNode* InputNode::create(float _w, const char* _phtxt, const char* _fnt) {
    return create(_w, _phtxt, _fnt, "", 69);
}

InputNode* InputNode::create(float _w, const char* _phtxt) {
    return create(_w, _phtxt, "bigFont.fnt");
}
