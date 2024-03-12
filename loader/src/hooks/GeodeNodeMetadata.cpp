#include <Geode/modify/Field.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/modify/CCNode.hpp>
#include <cocos2d.h>
#include <unordered_map>
#include <unordered_set>
#include <memory>

using namespace geode::prelude;
using namespace geode::modifier;
using namespace cocos2d;

constexpr auto METADATA_TAG = 0xB324ABC;

struct ProxyCCNode;

class GeodeNodeMetadata final : public CCObject {
private:
    FieldContainer* m_fieldContainer;
    std::string m_id;
    Ref<Layout> m_layout;
    Ref<LayoutOptions> m_layoutOptions;
    std::unordered_map<std::string, Ref<CCObject>> m_userObjects;
    std::unordered_set<EventListenerProtocol*> m_eventListeners;
    std::unordered_map<std::string, std::unique_ptr<EventListenerProtocol>> m_idEventListeners;

    friend class ProxyCCNode;
    friend class CCNode;

    GeodeNodeMetadata() : m_fieldContainer(new FieldContainer()) {}

    virtual ~GeodeNodeMetadata() {
        delete m_fieldContainer;
    }

public:
    static GeodeNodeMetadata* set(CCNode* target) {
        if (!target) return nullptr;

        auto old = target->getUserObject();
        if (old && old->getTag() == METADATA_TAG) {
            return static_cast<GeodeNodeMetadata*>(old);
        }

        auto meta = new GeodeNodeMetadata();
        meta->autorelease();
        meta->setTag(METADATA_TAG);
        target->setUserObject(meta);

        if (old) {
            meta->m_userObjects.insert({ "", old });
            old->release();
        }
        return meta;
    }

    FieldContainer* getFieldContainer() {
        return m_fieldContainer;
    }
};

struct ProxyCCNode : Modify<ProxyCCNode, CCNode> {
    virtual CCObject* getUserObject() {
        return m_userObject;
    }

    virtual void setUserObject(CCObject* obj) {
        CC_SAFE_RELEASE(m_userObject);
        m_userObject = obj;
        CC_SAFE_RETAIN(m_userObject);
    }
};

static inline std::unordered_map<std::string, size_t> s_nextIndex;
size_t modifier::getFieldIndexForClass(const char* name) {
    return s_nextIndex[name]++;
}

FieldContainer* CCNode::getFieldContainer() {
    return GeodeNodeMetadata::set(this)->getFieldContainer();
}

std::string CCNode::getID() {
    return GeodeNodeMetadata::set(this)->m_id;
}

void CCNode::setID(const std::string& id) {
    GeodeNodeMetadata::set(this)->m_id = id;
}

CCNode* CCNode::getChildByID(const std::string& id) {
    for (auto child : m_children) {
        auto ccChild = dynamic_cast<CCNode*>(child);
        if (ccChild && ccChild->getID() == id) {
            return ccChild;
        }
    }
    return nullptr;
}

CCNode* CCNode::getChildByIDRecursive(const std::string& id) {
    if (auto child = getChildByID(id)) {
        return child;
    }
    for (auto child : m_children) {
        auto ccChild = dynamic_cast<CCNode*>(child);
        if (ccChild) {
            auto grandChild = ccChild->getChildByIDRecursive(id);
            if (grandChild) return grandChild;
        }
    }
    return nullptr;
}

void CCNode::removeChildByID(const std::string& id) {
    if (auto child = getChildByID(id)) {
        removeChild(child);
    }
}

void CCNode::setLayout(Layout* layout, bool apply, bool respectAnchor) {
    if (respectAnchor && isIgnoreAnchorPointForPosition()) {
        for (auto child : m_children) {
            child->setPosition(child->getPosition() + getScaledContentSize());
        }
        ignoreAnchorPointForPosition(false);
    }
    GeodeNodeMetadata::set(this)->m_layout = layout;
    if (apply) {
        updateLayout();
    }
}

Layout* CCNode::getLayout() {
    return GeodeNodeMetadata::set(this)->m_layout.get();
}

void CCNode::setLayoutOptions(LayoutOptions* options, bool apply) {
    GeodeNodeMetadata::set(this)->m_layoutOptions = options;
    if (apply && m_parent) {
        m_parent->updateLayout();
    }
}

LayoutOptions* CCNode::getLayoutOptions() {
    return GeodeNodeMetadata::set(this)->m_layoutOptions.get();
}

void CCNode::updateLayout(bool updateChildOrder) {
    if (updateChildOrder) {
        sortAllChildren();
    }
    if (auto layout = GeodeNodeMetadata::set(this)->m_layout.get()) {
        layout->apply(this);
    }
}

void CCNode::setUserObject(const std::string& id, CCObject* value) {
    auto meta = GeodeNodeMetadata::set(this);
    if (value) {
        meta->m_userObjects[id] = value;
    }
    else {
        meta->m_userObjects.erase(id);
    }
    UserObjectSetEvent(this, id, value).post();
}

CCObject* CCNode::getUserObject(const std::string& id) {
    auto meta = GeodeNodeMetadata::set(this);
    auto it = meta->m_userObjects.find(id);
    if (it != meta->m_userObjects.end()) {
        return it->second.get();
    }
    return nullptr;
}

void CCNode::addEventListenerInternal(const std::string& id, EventListenerProtocol* listener) {
    auto meta = GeodeNodeMetadata::set(this);
    if (!id.empty()) {
        meta->m_idEventListeners[id].reset(listener);
    }
    else {
        meta->m_eventListeners.insert(listener);
    }
}

void CCNode::removeEventListener(EventListenerProtocol* listener) {
    auto meta = GeodeNodeMetadata::set(this);
    meta->m_eventListeners.erase(listener);
    for (auto& pair : meta->m_idEventListeners) {
        if (pair.second.get() == listener) {
            meta->m_idEventListeners.erase(pair.first);
            break;
        }
    }
}

void CCNode::removeEventListener(const std::string& id) {
    GeodeNodeMetadata::set(this)->m_idEventListeners.erase(id);
}

EventListenerProtocol* CCNode::getEventListener(const std::string& id) {
    auto meta = GeodeNodeMetadata::set(this);
    auto it = meta->m_idEventListeners.find(id);
    if (it != meta->m_idEventListeners.end()) {
        return it->second.get();
    }
    return nullptr;
}

size_t CCNode::getEventListenerCount() {
    auto meta = GeodeNodeMetadata::set(this);
    return meta->m_idEventListeners.size() + meta->m_eventListeners.size();
}

void CCNode::addChildAtPosition(CCNode* child, Anchor anchor, const CCPoint& offset, bool useAnchorLayout) {
    if (!child) return;

    auto layout = getLayout();
    if (!layout && useAnchorLayout) {
        setLayout(AnchorLayout::create());
    }

    child->setPosition(AnchorLayout::getAnchoredPosition(this, anchor, offset));
    if (useAnchorLayout) {
        child->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(anchor)->setOffset(offset));
    }
    addChild(child);
}
