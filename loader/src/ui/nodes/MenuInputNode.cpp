#include <Geode/ui/MenuInputNode.hpp>

USE_GEODE_NAMESPACE();

bool MenuInputNode::init(
    float width, float height, const char* placeholder, const char* fontPath, bool bg
) {
    if (!CCMenuItem::init())
        return false;
    
    if (bg) {
        m_bgSprite = cocos2d::extension::CCScale9Sprite::create(
            "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
        );
        m_bgSprite->setScale(.5f);
        m_bgSprite->setColor({ 0, 0, 0 });
        m_bgSprite->setOpacity(75);
        m_bgSprite->setContentSize({ width * 2, height * 2 });
        this->addChild(m_bgSprite);
    }

    this->setContentSize({ width, height });
    this->setAnchorPoint({ .5f, .5f });
    m_input = CCTextInputNode::create(width, height, placeholder, fontPath);
    m_input->setPosition(width / 2, height / 2);
    this->addChild(m_input);
    
    this->setEnabled(true);

    return true;
}

MenuInputNode* MenuInputNode::create(
    float width, float height, const char* placeholder, const char* fontPath, bool bg
) {
    auto ret = new MenuInputNode;
    if (ret && ret->init(width, height, placeholder, fontPath, bg)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void MenuInputNode::selected() {
    m_input->onClickTrackNode(true);
}

CCTextInputNode* MenuInputNode::getInput() const {
    return m_input;
}
