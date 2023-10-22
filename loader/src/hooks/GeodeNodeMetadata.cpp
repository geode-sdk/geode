#include <Geode/modify/Field.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/modify/Field.hpp>
#include <Geode/modify/CCNode.hpp>
#include <cocos2d.h>

using namespace geode::prelude;
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
    Ref<Layout> m_layout = nullptr;
    std::unique_ptr<LayoutOptions> m_layoutOptions = nullptr;
    std::unordered_map<std::string, json::Value> m_attributes;
    std::unordered_set<std::unique_ptr<EventListenerProtocol>> m_eventListeners;
    std::unordered_map<std::string, std::unique_ptr<EventListenerProtocol>> m_idEventListeners;

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

static inline std::unordered_map<std::string, size_t> s_nextIndex;
size_t modifier::getFieldIndexForClass(char const* name) {
	return s_nextIndex[name]++;
}

size_t modifier::getFieldIndexForClass(size_t hash) {
	return s_nextIndex[std::to_string(hash)]++;
}

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

void CCNode::removeChildByID(std::string const& id) {
    if (auto child = this->getChildByID(id)) {
        this->removeChild(child);
    }
}

void CCNode::setLayout(Layout* layout, bool apply, bool respectAnchor) {
    if (respectAnchor && this->isIgnoreAnchorPointForPosition()) {
        for (auto child : CCArrayExt<CCNode>(m_pChildren)) {
            child->setPosition(child->getPosition() + this->getScaledContentSize());
        }
        this->ignoreAnchorPointForPosition(false);
    }
    GeodeNodeMetadata::set(this)->m_layout = layout;
    if (apply) {
        this->updateLayout();
    }
}

Layout* CCNode::getLayout() {
    return GeodeNodeMetadata::set(this)->m_layout.data();
}

void CCNode::setLayoutOptions(LayoutOptions* options, bool apply) {
    GeodeNodeMetadata::set(this)->m_layoutOptions.reset(options);
    if (apply && m_pParent) {
        m_pParent->updateLayout();
    }
}

LayoutOptions* CCNode::getLayoutOptions() {
    return GeodeNodeMetadata::set(this)->m_layoutOptions.get();
}

void CCNode::updateLayout(bool updateChildOrder) {
    if (updateChildOrder) {
        this->sortAllChildren();
    }
    if (auto layout = GeodeNodeMetadata::set(this)->m_layout.data()) {
        layout->apply(this);
    }
}

AttributeSetEvent::AttributeSetEvent(CCNode* node, std::string const& id, json::Value& value)
  : node(node), id(id), value(value) {}

ListenerResult AttributeSetFilter::handle(MiniFunction<Callback> fn, AttributeSetEvent* event) {
    if (event->id == m_targetID) {
        fn(event);
    }
    return ListenerResult::Propagate;
}

AttributeSetFilter::AttributeSetFilter(std::string const& id) : m_targetID(id) {}

void CCNode::setAttribute(std::string const& attr, json::Value const& value) {
    auto meta = GeodeNodeMetadata::set(this);
    meta->m_attributes[attr] = value;
    AttributeSetEvent(this, attr, meta->m_attributes.at(attr)).post();
}

std::optional<json::Value> CCNode::getAttributeInternal(std::string const& attr) {
    auto meta = GeodeNodeMetadata::set(this);
    if (meta->m_attributes.count(attr)) {
        return meta->m_attributes.at(attr);
    }
    return std::nullopt;
}

void CCNode::addEventListenerInternal(std::string const& id, EventListenerProtocol* listener) {
    auto meta = GeodeNodeMetadata::set(this);
    if (id.size()) {
        if (meta->m_idEventListeners.contains(id)) {
            meta->m_idEventListeners.at(id).reset(listener);
        }
        else {
            meta->m_idEventListeners.emplace(id, listener);
        }
    }
    else {
        std::erase_if(meta->m_eventListeners, [=](auto& l) {
            return l.get() == listener;
        });
        meta->m_eventListeners.emplace(listener);
    }
}

void CCNode::removeEventListener(EventListenerProtocol* listener) {
    auto meta = GeodeNodeMetadata::set(this);
    std::erase_if(meta->m_eventListeners, [=](auto& l) {
        return l.get() == listener;
    });
    std::erase_if(meta->m_idEventListeners, [=](auto& l) {
        return l.second.get() == listener;
    });
}

void CCNode::removeEventListener(std::string const& id) {
    GeodeNodeMetadata::set(this)->m_idEventListeners.erase(id);
}

EventListenerProtocol* CCNode::getEventListener(std::string const& id) {
    auto meta = GeodeNodeMetadata::set(this);
    if (meta->m_idEventListeners.contains(id)) {
        return meta->m_idEventListeners.at(id).get();
    }
    return nullptr;
}

size_t CCNode::getEventListenerCount() {
    return GeodeNodeMetadata::set(this)->m_idEventListeners.size() +
        GeodeNodeMetadata::set(this)->m_eventListeners.size();
}

#pragma warning(pop)
