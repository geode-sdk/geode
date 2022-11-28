#include <Geode/modify/Field.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/modify/Field.hpp>
#include <Geode/modify/CCNode.hpp>
#include <cocos2d.h>

USE_GEODE_NAMESPACE();
using namespace geode::modifier;

#pragma warning(push)
#pragma warning(disable : 4273)

constexpr auto METADATA_TAG = 0xB324ABC;

struct ProxyCCNode;

class GeodeNodeMetadata final : public cocos2d::CCObject {
private:
    FieldContainer* m_fieldContainer;
    Ref<cocos2d::CCObject> m_userObject;
    std::string m_id = "";
    std::unique_ptr<Layout> m_layout = nullptr;
    PositionHint m_positionHint = PositionHint::Default;
    std::unordered_map<std::string, std::any> m_attributes;

    friend class ProxyCCNode;
    friend class cocos2d::CCNode;

    GeodeNodeMetadata() : m_fieldContainer(new FieldContainer()) {}

    virtual ~GeodeNodeMetadata() {
        delete m_fieldContainer;
    }

public:
    static GeodeNodeMetadata* set(CCNode* target) {
        if (!target) return nullptr;

        auto old = target->m_pUserObject;
        // faster than dynamic_cast, technically can
        // but extremely unlikely to fail
        if (old && old->getTag() == METADATA_TAG) {
            return static_cast<GeodeNodeMetadata*>(old);
        }
        auto meta = new GeodeNodeMetadata();
        meta->autorelease();
        meta->setTag(METADATA_TAG);

        // set user object
        target->m_pUserObject = meta;
        meta->retain();

        if (old) {
            meta->m_userObject = old;
            // the old user object has been retained by CCNode
            old->release();
        }
        return meta;
    }

    FieldContainer* getFieldContainer() {
        return m_fieldContainer;
    }
};

// proxy forwards
// clang-format off
#include <Geode/modify/CCNode.hpp>
struct ProxyCCNode : Modify<ProxyCCNode, CCNode> {
    virtual CCObject* getUserObject() {
        if (static_cast<CCObject*>(this) == static_cast<CCObject*>(CCDirector::get())) {
            // apparently this function is the same as 
            // CCDirector::getNextScene so yeah
            return m_pUserObject;
        }
        return GeodeNodeMetadata::set(this)->m_userObject;
    }
    virtual void setUserObject(CCObject* obj) {
        GeodeNodeMetadata::set(this)->m_userObject = obj;
    }
};

// clang-format on

// not const because might modify contents
FieldContainer* CCNode::getFieldContainer() {
    return GeodeNodeMetadata::set(this)->getFieldContainer();
}

std::string CCNode::getID() {
    return GeodeNodeMetadata::set(this)->m_id;
}

void CCNode::setID(std::string const& id) {
    GeodeNodeMetadata::set(this)->m_id = id;
}

CCNode* CCNode::getChildByID(std::string const& id) {
    for (auto child : CCArrayExt<CCNode>(m_pChildren)) {
        if (child->getID() == id) {
            return child;
        }
    }
    return nullptr;
}

CCNode* CCNode::getChildByIDRecursive(std::string const& id) {
    if (auto child = this->getChildByID(id)) {
        return child;
    }
    for (auto child : CCArrayExt<CCNode>(m_pChildren)) {
        if ((child = child->getChildByIDRecursive(id))) {
            return child;
        }
    }
    return nullptr;
}

void CCNode::setLayout(Layout* layout, bool apply) {
    GeodeNodeMetadata::set(this)->m_layout.reset(layout);
    if (apply) {
        this->updateLayout();
    }
}

Layout* CCNode::getLayout() {
    return GeodeNodeMetadata::set(this)->m_layout.get();
}

void CCNode::updateLayout() {
    if (auto layout = GeodeNodeMetadata::set(this)->m_layout.get()) {
        // nodes with absolute position should never be rearranged
        auto filtered = CCArray::create();
        for (auto& child : CCArrayExt<CCNode>(m_pChildren)) {
            if (child->getPositionHint() != PositionHint::Absolute) {
                filtered->addObject(child);
            }
        }
        layout->apply(filtered, m_obContentSize);
        filtered->release();
    }
}

void CCNode::setPositionHint(PositionHint hint) {
    GeodeNodeMetadata::set(this)->m_positionHint = hint;
}

PositionHint CCNode::getPositionHint() {
    return GeodeNodeMetadata::set(this)->m_positionHint;
}

void CCNode::setAttribute(std::string const& attr, std::any value) {
    GeodeNodeMetadata::set(this)->m_attributes[attr] = value;
}

std::optional<std::any> CCNode::getAttributeInternal(std::string const& attr) {
    auto meta = GeodeNodeMetadata::set(this);
    if (meta->m_attributes.count(attr)) {
        return meta->m_attributes.at(attr);
    }
    return std::nullopt;
}

#pragma warning(pop)
