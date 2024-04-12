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
    std::unordered_map<std::string, FieldContainer*> m_classFieldContainers;
    std::string m_id = "";
    Ref<Layout> m_layout = nullptr;
    Ref<LayoutOptions> m_layoutOptions = nullptr;
    std::unordered_map<std::string, Ref<CCObject>> m_userObjects;
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
            meta->m_userObjects.insert({ "", old });
            // the old user object is now managed by Ref
            old->release();
        }
        return meta;
    }

    FieldContainer* getFieldContainer() {
        return m_fieldContainer;
    }

    FieldContainer* getFieldContainer(char const* forClass) {
        if (!m_classFieldContainers.count(forClass)) {
            m_classFieldContainers[forClass] = new FieldContainer();
        }
        return m_classFieldContainers[forClass];
    }
};

// proxy forwards
#include <Geode/modify/CCNode.hpp>
struct ProxyCCNode : Modify<ProxyCCNode, CCNode> {
    virtual CCObject* getUserObject() {
        if (auto asNode = typeinfo_cast<CCNode*>(this)) {
            return asNode->getUserObject("");
        }
        else {
            // apparently this function is the same as 
            // CCDirector::getNextScene so yeah
            return m_pUserObject;
        }
    }
    virtual void setUserObject(CCObject* obj) {
        if (auto asNode = typeinfo_cast<CCNode*>(this)) {
            asNode->setUserObject("", obj);
        }
        else {
            CC_SAFE_RELEASE(m_pUserObject);
            m_pUserObject = obj;
            CC_SAFE_RETAIN(m_pUserObject);
        }
    }
};

static inline std::unordered_map<std::string, size_t> s_nextIndex;
size_t modifier::getFieldIndexForClass(char const* name) {
	return s_nextIndex[name]++;
}

// not const because might modify contents
FieldContainer* CCNode::getFieldContainer() {
    return GeodeNodeMetadata::set(this)->getFieldContainer();
}

FieldContainer* CCNode::getFieldContainer(char const* forClass) {
    return GeodeNodeMetadata::set(this)->getFieldContainer(forClass);
}

std::string CCNode::getID() {
    return GeodeNodeMetadata::set(this)->m_id;
}

void CCNode::setID(std::string const& id) {
    GeodeNodeMetadata::set(this)->m_id = id;
}

CCNode* CCNode::getChildByID(std::string const& id) {
    for (auto child : CCArrayExt<CCNode*>(m_pChildren)) {
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
    for (auto child : CCArrayExt<CCNode*>(m_pChildren)) {
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
        for (auto child : CCArrayExt<CCNode*>(m_pChildren)) {
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
    GeodeNodeMetadata::set(this)->m_layoutOptions = options;
    if (apply && m_pParent) {
        m_pParent->updateLayout();
    }
}

LayoutOptions* CCNode::getLayoutOptions() {
    return GeodeNodeMetadata::set(this)->m_layoutOptions.data();
}

void CCNode::updateLayout(bool updateChildOrder) {
    if (updateChildOrder) {
        this->sortAllChildren();
    }
    if (auto layout = GeodeNodeMetadata::set(this)->m_layout.data()) {
        layout->apply(this);
    }
}

UserObjectSetEvent::UserObjectSetEvent(CCNode* node, std::string const& id, CCObject* value)
  : node(node), id(id), value(value) {}

ListenerResult AttributeSetFilter::handle(MiniFunction<Callback> fn, UserObjectSetEvent* event) {
    if (event->id == m_targetID) {
        fn(event);
    }
    return ListenerResult::Propagate;
}

AttributeSetFilter::AttributeSetFilter(std::string const& id) : m_targetID(id) {}

void CCNode::setUserObject(std::string const& id, CCObject* value) {
    auto meta = GeodeNodeMetadata::set(this);
    if (value) {
        meta->m_userObjects[id] = value;
    }
    else {
        meta->m_userObjects.erase(id);
    }
    UserObjectSetEvent(this, id, value).post();
}

CCObject* CCNode::getUserObject(std::string const& id) {
    auto meta = GeodeNodeMetadata::set(this);
    if (meta->m_userObjects.count(id)) {
        return meta->m_userObjects.at(id);
    }
    return nullptr;
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

void CCNode::addChildAtPosition(CCNode* child, Anchor anchor, CCPoint const& offset, bool useAnchorLayout) {
    auto layout = this->getLayout();
    if (!layout && useAnchorLayout) {
        this->setLayout(AnchorLayout::create());
    }
    child->setPosition(AnchorLayout::getAnchoredPosition(this, anchor, offset));
    if (useAnchorLayout) {
        child->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(anchor)->setOffset(offset));
    }
    this->addChild(child);
}

#ifdef GEODE_EXPORTING

void CCNode::setAttribute(std::string const& attr, matjson::Value const& value) {}
std::optional<matjson::Value> CCNode::getAttributeInternal(std::string const& attr) {
    return std::nullopt;
}

#endif

#pragma warning(pop)
