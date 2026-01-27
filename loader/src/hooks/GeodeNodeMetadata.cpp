#include <Geode/modify/Field.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/modify/Field.hpp>
#include <Geode/modify/CCNode.hpp>
#include <Geode/utils/terminate.hpp>
#include <Geode/utils/StringMap.hpp>
#include <cocos2d.h>
#include <queue>
#include <stack>

using namespace geode::prelude;
using namespace geode::modifier;

#pragma warning(push)
#pragma warning(disable : 4273)

constexpr auto METADATA_TAG = 0xB324ABC;

struct ProxyCCNode;

class GeodeNodeMetadata final : public cocos2d::CCObject {
private:
    StringMap<FieldContainer*> m_classFieldContainers;
    std::string m_id = "";
    Ref<Layout> m_layout = nullptr;
    Ref<LayoutOptions> m_layoutOptions = nullptr;
    StringMap<Ref<CCObject>> m_userObjects;
    StringSet m_userFlags;
    std::unordered_set<std::unique_ptr<EventListenerProtocol>> m_eventListeners;
    StringMap<std::unique_ptr<EventListenerProtocol>> m_idEventListeners;

    friend class ProxyCCNode;
    friend class cocos2d::CCNode;

    GeodeNodeMetadata() {}

    virtual ~GeodeNodeMetadata() {
        for (auto& [_, container] : m_classFieldContainers) {
            delete container;
        }
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
            meta->setUserObject("", old);
            // the old user object is now managed by Ref
            old->release();
        }
        return meta;
    }

    FieldContainer* getFieldContainer(char const* forClass) {
        auto it = m_classFieldContainers.find(forClass);
        if (it != m_classFieldContainers.end()) {
            return it->second;
        }

        auto container = new FieldContainer();
        m_classFieldContainers.insert(it, std::make_pair(forClass, container));

        return container;
    }

    CCObject* getUserObject(std::string_view id) {
        auto it = m_userObjects.find(id);
        return it != m_userObjects.end() ? it->second : nullptr;
    }

    void setUserObject(std::string id, CCObject* object) {
        if (object) {
            auto it = m_userObjects.find(id);
            if (it == m_userObjects.end()) {
                m_userObjects.emplace(std::move(id), object);
            } else {
                it->second = object;
            }
        } else {
            m_userObjects.erase(id);
        }
    }

    bool getUserFlag(std::string_view id) {
        return m_userFlags.contains(id);
    }

    void setUserFlag(std::string id, bool state) {
        if (state) {
            m_userFlags.emplace(std::move(id));
        } else {
            m_userFlags.erase(id);
        }
    }

    EventListenerProtocol* getEventListener(std::string_view id) {
        auto it = m_idEventListeners.find(id);
        return it != m_idEventListeners.end() ? it->second.get() : nullptr;
    }

    void addEventListener(std::string id, EventListenerProtocol* protocol) {
        if (id.empty()) {
            m_eventListeners.emplace(protocol);
            return;
        }

        auto it = m_idEventListeners.find(id);
        if (it != m_idEventListeners.end()) {
            it->second.reset(protocol);
        } else {
            m_idEventListeners.emplace(std::move(id), protocol);
        }
    }

    void removeEventListener(std::string_view id) {
        auto it = m_idEventListeners.find(id);
        if (it != m_idEventListeners.end()) {
            m_idEventListeners.erase(it);
        }
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

// it is mostly safe to use string_view here to reduce heap allocations,
// since passed names are obtained by typed().name() which is static
static inline std::unordered_map<std::string_view, size_t> s_nextIndex;
size_t modifier::getFieldIndexForClass(char const* name) {
	return s_nextIndex[name]++;
}

FieldContainer* CCNode::getFieldContainer(char const* forClass) {
    return GeodeNodeMetadata::set(this)->getFieldContainer(forClass);
}

ZStringView CCNode::getID() {
    return GeodeNodeMetadata::set(this)->m_id;
}

void CCNode::setID(std::string id) {
    GeodeNodeMetadata::set(this)->m_id = std::move(id);
}

CCNode* CCNode::getChildByID(std::string_view id) {
    for (auto child : CCArrayExt<CCNode*>(this->getChildren())) {
        if (child->getID() == id) {
            return child;
        }
    }
    return nullptr;
}

CCNode* CCNode::getChildByIDRecursive(std::string_view id) {
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

class BFSNodeTreeCrawler final {
private:
    std::queue<CCNode*> m_queue;
    std::unordered_set<CCNode*> m_explored;

public:
    BFSNodeTreeCrawler(CCNode* target) {
        if (auto first = getChild(target, 0)) {
            m_explored.insert(first);
            m_queue.push(first);
        }
    }

    CCNode* next() {
        if (m_queue.empty()) {
            return nullptr;
        }
        auto node = m_queue.front();
        m_queue.pop();
        for (auto sibling : CCArrayExt<CCNode*>(node->getParent()->getChildren())) {
            if (!m_explored.contains(sibling)) {
                m_explored.insert(sibling);
                m_queue.push(sibling);
            }
        }
        for (auto child : CCArrayExt<CCNode*>(node->getChildren())) {
            if (!m_explored.contains(child)) {
                m_explored.insert(child);
                m_queue.push(child);
            }
        }
        return node;
    }
};

class NodeQuery final {
private:
    enum class Op {
        ImmediateChild,
        DescendantChild,
    };

    std::string m_targetID;
    Op m_nextOp;
    std::unique_ptr<NodeQuery> m_next = nullptr;

public:
    static Result<std::unique_ptr<NodeQuery>> parse(std::string_view query) {
        if (query.empty()) {
            return Err("Query may not be empty");
        }

        auto result = std::make_unique<NodeQuery>();
        NodeQuery* current = result.get();

        size_t i = 0;
        std::string collectedID;
        std::optional<Op> nextOp = Op::DescendantChild;
        while (i < query.size()) {
            auto c = query.at(i);
            if (c == ' ') {
                if (!nextOp) {
                    nextOp.emplace(Op::DescendantChild);
                }
            }
            else if (c == '>') {
                if (!nextOp || *nextOp == Op::DescendantChild) {
                    nextOp.emplace(Op::ImmediateChild);
                }
                // Double >> is syntax error
                else {
                    return Err("Can't have multiple child operators at once (index {})", i);
                }
            }
            // ID-valid characters
            else if (std::isalnum(c) || c == '-' || c == '_' || c == '/' || c == '.') {
                if (nextOp) {
                    current->m_next = std::make_unique<NodeQuery>();
                    current->m_nextOp = *nextOp;
                    current->m_targetID = collectedID;
                    current = current->m_next.get();

                    collectedID = "";
                    nextOp = std::nullopt;
                }
                collectedID.push_back(c);
            }
            // Any other character is syntax error due to needing to reserve
            // stuff for possible future features
            else {
                return Err("Unexpected character '{}' at index {}", c, i);
            }
            i += 1;
        }
        if (nextOp || collectedID.empty()) {
            return Err("Expected node ID but got end of query");
        }
        current->m_targetID = collectedID;

        return Ok(std::move(result));
    }

    CCNode* match(CCNode* node) const {
        // Make sure this matches the ID being looked for
        if (!m_targetID.empty() && node->getID() != m_targetID) {
            return nullptr;
        }
        // If this is the last thing to match, return the result
        if (!m_next) {
            return node;
        }
        switch (m_nextOp) {
            case Op::ImmediateChild: {
                for (auto c : CCArrayExt<CCNode*>(node->getChildren())) {
                    if (auto r = m_next->match(c)) {
                        return r;
                    }
                }
            } break;

            case Op::DescendantChild: {
                auto crawler = BFSNodeTreeCrawler(node);
                while (auto c = crawler.next()) {
                    if (auto r = m_next->match(c)) {
                        return r;
                    }
                }
            } break;
        }
        return nullptr;
    }

    std::string toString() const {
        auto str = m_targetID.empty() ? "&" : m_targetID;
        if (m_next) {
            switch (m_nextOp) {
                case Op::ImmediateChild: str += " > "; break;
                case Op::DescendantChild: str += " "; break;
            }
            str += m_next->toString();
        }
        return str;
    }
};

CCNode* CCNode::querySelector(std::string_view queryStr) {
    auto res = NodeQuery::parse(queryStr);
    if (!res) {
        log::error("Invalid CCNode::querySelector query '{}': {}", queryStr, res.unwrapErr());
        return nullptr;
    }
    auto query = std::move(res.unwrap());
    // log::info("parsed query: {}", query->toString());
    return query->match(this);
}

void CCNode::removeChildByID(std::string_view id) {
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
    if (updateChildOrder && m_pChildren) {
        this->sortAllChildren();
    }
    if (auto layout = GeodeNodeMetadata::set(this)->m_layout.data()) {
        layout->apply(this);
    }
}

UserObjectSetEvent::UserObjectSetEvent(CCNode* node, std::string id, CCObject* value)
  : node(node), id(std::move(id)), value(value) {}

ListenerResult AttributeSetFilter::handle(geode::Function<Callback>& fn, UserObjectSetEvent* event) {
    if (event->id == m_targetID) {
        fn(event);
    }
    return ListenerResult::Propagate;
}

AttributeSetFilter::AttributeSetFilter(std::string id) : m_targetID(std::move(id)) {}

void CCNode::setUserObject(std::string id, CCObject* value) {
    GeodeNodeMetadata::set(this)->setUserObject(id, value);
    UserObjectSetEvent(this, std::move(id), value).post();
}

CCObject* CCNode::getUserObject(std::string_view id) {
    return GeodeNodeMetadata::set(this)->getUserObject(id);
}

void CCNode::setUserFlag(std::string id, bool state) {
    GeodeNodeMetadata::set(this)->setUserFlag(std::move(id), state);
}

bool CCNode::getUserFlag(std::string_view id) {
    return GeodeNodeMetadata::set(this)->getUserFlag(id);
}

void CCNode::addEventListenerInternal(std::string id, EventListenerProtocol* listener) {
    GeodeNodeMetadata::set(this)->addEventListener(std::move(id), listener);
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

void CCNode::removeEventListener(std::string_view id) {
    GeodeNodeMetadata::set(this)->removeEventListener(id);
}

EventListenerProtocol* CCNode::getEventListener(std::string_view id) {
    return GeodeNodeMetadata::set(this)->getEventListener(id);
}

size_t CCNode::getEventListenerCount() {
    auto meta = GeodeNodeMetadata::set(this);
    return meta->m_idEventListeners.size() + meta->m_eventListeners.size();
}

void CCNode::addChildAtPosition(CCNode* child, Anchor anchor, CCPoint const& offset, bool useAnchorLayout) {
    return this->addChildAtPosition(child, anchor, offset, child->getAnchorPoint(), useAnchorLayout);
}

void CCNode::addChildAtPosition(CCNode* child, Anchor anchor, CCPoint const& offset, CCPoint const& nodeAnchor, bool useAnchorLayout) {
    auto layout = this->getLayout();
    if (!layout && useAnchorLayout) {
        this->setLayout(AnchorLayout::create());
    }
    // Set the position
    child->setPosition(AnchorLayout::getAnchoredPosition(this, anchor, offset));
    child->setAnchorPoint(nodeAnchor);
    // Set dynamic positioning
    if (useAnchorLayout) {
        child->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(anchor)->setOffset(offset));
    }
    this->addChild(child);
}

void CCNode::updateAnchoredPosition(Anchor anchor, CCPoint const& offset) {
    return this->updateAnchoredPosition(anchor, offset, this->getAnchorPoint());
}

void CCNode::updateAnchoredPosition(Anchor anchor, CCPoint const& offset, CCPoint const& nodeAnchor) {
    // Always require a parent
    if (!m_pParent) {
        return;
    }
    // Set the position
    this->setPosition(AnchorLayout::getAnchoredPosition(m_pParent, anchor, offset));
    this->setAnchorPoint(nodeAnchor);
    // Update dynamic positioning
    if (auto opts = typeinfo_cast<AnchorLayoutOptions*>(this->getLayoutOptions())) {
        opts->setAnchor(anchor);
        opts->setOffset(offset);
    }
}

namespace {
    template <class T, size_t N>
    struct LocalStack {
        std::array<T, N> m_stack;
        size_t m_index = 0;

        bool push(T value) {
            if (m_index >= N) return false;
            m_stack[m_index] = value;
            m_index++;
            return true;
        }

        bool pop() {
            if (m_index == 0) return false;
            m_index--;
            return true;
        }

        T top() {
            if (m_index == 0) return nullptr;
            return m_stack[m_index - 1];
        }

        bool empty() {
            return m_index == 0;
        }
    };

    static thread_local LocalStack<void*, 32> s_lockStack;
}

bool geode::DestructorLock::isLocked(void* self) {
    // only the top of the stack matters
    if (s_lockStack.empty()) return false;
    return s_lockStack.top() == self;
}
void geode::DestructorLock::addLock(void* self) {
    if (!s_lockStack.push(self)) {
        geode::utils::terminate("DestructorLock lock stack overflow (tried to add too many locks at once)");
    }
}
void geode::DestructorLock::removeLock(void* self) {
    if (s_lockStack.top() != self) {
        geode::utils::terminate("DestructorLock lock stack corruption (tried to unlock a destructor that was not the top of the stack)");
    }
    if (!s_lockStack.pop()) {
        geode::utils::terminate("DestructorLock lock stack underflow (tried to unlock a destructor that was never locked)");
    }
}

#pragma warning(pop)
