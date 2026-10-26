#include <Geode/DefaultInclude.hpp>
#include <Geode/binding/BoomScrollLayer.hpp>
#include <Geode/binding/LevelEditorLayer.hpp>
#include <Geode/binding/TableView.hpp>
#include <Geode/binding/UILayer.hpp>
#include <Geode/mapping/CCLayer.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <vector>

using namespace geode::prelude;

#include <Geode/modify/CCTouchDispatcher.hpp>

struct ZOrderTouchDispatcher : Modify<ZOrderTouchDispatcher, CCTouchDispatcher> {
    static void onModify(auto& self) {
        if (!self.setHookPriority("cocos2d::CCTouchDispatcher::touches", Priority::Replace)) {
            log::warn(
                "Failed to set cocos2d::CCTouchDispatcher::touches hook priority, Z-order based "
                "touch dispatcher may not work properly"
            );
        }
        GEODE_FORWARD_COMPAT_DISABLE_HOOKS_INNER("Z Order based touch dispatcher");
    }

    template <class Handler>
    struct ParentPath {
        std::vector<CCNode*> path;
        Handler* handler = nullptr;
        mutable bool hasInvalidRoot = false;

        ParentPath(Handler* handler) : handler(handler) {}

        ParentPath(ParentPath&& other) = default;
        ParentPath& operator=(ParentPath&& other) = default;

        ParentPath(CCNode* node, Handler* handler) : handler(handler) {
            while (node) {
                path.push_back(node);
                node = node->getParent();
            }
        }

        static std::optional<ParentPath> filtered(CCNode* node, Handler* handler, CCNode* filter) {
            ParentPath ret{handler};
            bool confirmed = false;
            while (node) {
                ret.path.push_back(node);
                if (node == filter) confirmed = true;
                node = node->getParent();
            }
            if (confirmed) return ret;
            return std::nullopt;
        }

        CCNode* leaf() const {
            return path.empty() ? nullptr : path.front();
        }

        CCNode* root() const {
            return path.empty() ? nullptr : path.back();
        }

        CCNode* nth(size_t n) const {
            return (n < path.size()) ? path[path.size() - 1 - n] : nullptr;
        }

        bool swallows() const {
            if constexpr (std::is_same_v<Handler, CCTargetedTouchHandler>) {
                return handler->m_bSwallowsTouches;
            }
            return false;
        }

        bool isStealer(CCNode* node) const {
            // general compat
            if (node->getUserFlag("steals-touch"_spr)) return true;

            // known stealers
            if (typeinfo_cast<TableView*>(node)) return true;
            if (typeinfo_cast<BoomScrollLayer*>(node)) return true;

            if (auto scrollLayer = typeinfo_cast<ScrollLayer*>(node)) {
                return scrollLayer->isStealingTouches();
            }
            return false;
        }

        bool steals() const {
            // if it swallows, no need to check for stealing behavior
            if (this->swallows()) return false;

            if constexpr (std::is_same_v<Handler, CCTargetedTouchHandler>) {
                if (auto leaf = this->leaf()) {
                    if (this->isStealer(leaf)) return true;
                }
            }
            return false;
        }

        bool hasValidRoot() {
            if (this->hasInvalidRoot) return false;

            auto director = CCDirector::get();
            auto const rootOrder =
                std::array<CCNode*, 2>{director->m_pRunningScene, director->m_pNotificationNode};
            auto thisRootIt = std::find(rootOrder.begin(), rootOrder.end(), this->root());

            if (thisRootIt == rootOrder.end()) {
                this->hasInvalidRoot = true;
                return false;
            }
            return true;
        }

        bool compareRoots(ParentPath const& other) const {
            // same root, not less than
            if (this->root() == other.root()) return false;

            auto director = CCDirector::get();
            auto const rootOrder =
                std::array<CCNode*, 2>{director->m_pRunningScene, director->m_pNotificationNode};
            auto thisRootIt = std::find(rootOrder.begin(), rootOrder.end(), this->root());
            auto otherRootIt = std::find(rootOrder.begin(), rootOrder.end(), other.root());

            if (thisRootIt == rootOrder.end()) {
                // If either root is not in the known order, we can't compare them reliably
                this->hasInvalidRoot = true;
                return false;
            }
            if (otherRootIt == rootOrder.end()) {
                other.hasInvalidRoot = true;
                return false;
            }

            // if this is running scene and other is notification node, other will be 1 and this will be 0
            // meaning other should come first, so we return false
            // if this is notification node and other is running scene, other will be 0 and this will be 1
            // meaning this should come first, so we return true
            return thisRootIt > otherRootIt;
        }

        // assumes all nodes will converge at the same root
        bool operator<(ParentPath const& other) const {
            if (this->root() != other.root()) {
                return this->compareRoots(other);
            }

            size_t maxLength = std::max(path.size(), other.path.size());
            // nth(1) gives the child of the root, which is fine since we already compared the root
            for (size_t divergeIndex = 1; divergeIndex < maxLength; ++divergeIndex) {
                auto thisParent = this->nth(divergeIndex);
                auto otherParent = other.nth(divergeIndex);

                // okay so i know this is complicated
                // and yes it is but basically
                // lets have 2 nodes, both respond to touch
                // one is a parent to other, lets call parent and child
                // if parent doesnt swallow the touch, in gd usually what that means is
                // both parent and child can get the touch, and usually parent "steals"
                // the touch from the child whenever necessary
                // at least thats how scroll layers kind of work in gd
                // this is what its trying to emulate basically

                // swallows like a good boy >w<\n
                // https://github.com/geode-sdk/api/commit/01bcacde38a225edb51a06961ce6553851dd24c5

                if (!thisParent && otherParent) {
                    // other is deeper in compared to this, other should come first
                    // we are the parent, they are the child
                    // if we dont steal touches we can allow it to come after us though
                    // we steal: true, we are first
                    // if there are nested stealers we dont handle that right now,
                    // and im sure rob doesn't either, but if it ever happens
                    // shoot me a message
                    return this->steals();
                }
                else if (!otherParent && thisParent) {
                    // this is deeper in compared to other, this should come first
                    // they are the parent, we are the child
                    // if other doesnt steal touches we can allow it to come before us though
                    // they steal: false, they are first
                    return !other.steals();
                }
                else if (thisParent != otherParent) {
                    // otherwise we dont care anyway

                    // higher Z order should come first
                    if (thisParent->getZOrder() == otherParent->getZOrder()) {
                        // same Z order, use order of arrival
                        return thisParent->getOrderOfArrival() > otherParent->getOrderOfArrival();
                    }
                    return thisParent->getZOrder() > otherParent->getZOrder();
                }
            }
            // reached the leaves together without diverging, same node?
            return false;
        }
    };

    template <class Handler>
    std::vector<ParentPath<Handler>> getRegisteredPaths(
        CCArray* handlers, std::optional<CCNode*> filter, std::vector<ParentPath<Handler>>& invalidRoots
    ) const {
        std::vector<ParentPath<Handler>> paths;
        for (auto handler : CCArrayExt<Handler*>(handlers)) {
            if (!handler) continue;
            auto delegate = handler->getDelegate();
            if (!delegate) continue;
            auto node = typeinfo_cast<CCNode*>(delegate);
            if (!node) continue;

            if (filter) {
                auto filtered = ParentPath<Handler>::filtered(node, handler, *filter);
                if (filtered) {
                    if (filtered.value().hasValidRoot()) {
                        paths.push_back(std::move(filtered.value()));
                    }
                    else {
                        invalidRoots.push_back(std::move(filtered.value()));
                    }
                }
            }
            else {
                auto path = ParentPath<Handler>(node, handler);
                if (path.hasValidRoot()) {
                    paths.push_back(std::move(path));
                }
                else {
                    invalidRoots.push_back(std::move(path));
                }
            }
        }
        std::sort(paths.begin(), paths.end());
        return paths;
    }

    template <class Handler>
    bool handleSingleTargetedHandlers(
        CCSet* touches, CCTouch* touch, CCEvent* event, unsigned int index,
        std::vector<ParentPath<Handler>> const& registeredPaths
    ) {
        bool touchClaimed = false;

        for (auto& path : registeredPaths) {
            auto delegate = path.handler->getDelegate();
            auto claimedTouches = path.handler->m_pClaimedTouches;
            auto swallowsTouches = path.handler->m_bSwallowsTouches;

            if (path.hasInvalidRoot) continue;

            bool claimed = false;
            if (index == CCTOUCHBEGAN) {
                claimed = delegate->ccTouchBegan(touch, event);

                if (claimed) {
                    claimedTouches->addObject(touch);
                }
            }
            else if (claimedTouches->containsObject(touch)) {
                // moved ended canceled
                claimed = true;

                switch (m_sHandlerHelperData[index].m_type) {
                    case CCTOUCHMOVED: delegate->ccTouchMoved(touch, event); break;
                    case CCTOUCHENDED:
                        delegate->ccTouchEnded(touch, event);
                        claimedTouches->removeObject(touch);
                        break;
                    case CCTOUCHCANCELLED:
                        delegate->ccTouchCancelled(touch, event);
                        claimedTouches->removeObject(touch);
                        break;
                }
            }

            if (claimed && swallowsTouches) {
                touchClaimed = true;
                if (touches) touches->removeObject(touch);
                break;
            }
        }
        return touchClaimed;
    }

    template <class Handler>
    void logInvalidRoots(std::vector<ParentPath<Handler>> const& invalidRoots) {
        for (auto& path : invalidRoots) {
            log::error(
                "Handler {}({}) has an invalid root, detected leaked node!! Please report the "
                "issue to the owner of the node!!",
                path.leaf(),
                path.leaf()->getID()
            );
        }
    }

    void handleTargetedHandlers(
        CCSet* touches, CCEvent* event, unsigned int index, std::optional<CCNode*> filter = std::nullopt
    ) {
        std::vector<ParentPath<CCTargetedTouchHandler>> invalidRoots;
        Ref<CCArray> handlersCopy = m_pTargetedHandlers->shallowCopy();

        auto registeredPaths = this->getRegisteredPaths<CCTargetedTouchHandler>(
            m_pTargetedHandlers, filter, invalidRoots
        );

        if (index == CCTOUCHBEGAN) logInvalidRoots(invalidRoots);

        std::vector<CCTouch*> touchesCopy;
        for (auto touch : *touches) {
            touchesCopy.push_back(static_cast<CCTouch*>(touch));
        }

        for (auto touch : touchesCopy) {
            this->handleSingleTargetedHandlers(touches, touch, event, index, registeredPaths);
        }
    }

    void handleStandardHandlers(CCSet* touches, CCEvent* event, unsigned int index) {
        std::vector<ParentPath<CCStandardTouchHandler>> invalidRoots;
        Ref<CCArray> handlersCopy = m_pStandardHandlers->shallowCopy();

        auto registeredPaths = this->getRegisteredPaths<CCStandardTouchHandler>(
            m_pStandardHandlers, std::nullopt, invalidRoots
        );

        if (index == CCTOUCHBEGAN) logInvalidRoots(invalidRoots);

        for (auto& path : registeredPaths) {
            auto delegate = path.handler->getDelegate();

            switch (m_sHandlerHelperData[index].m_type) {
                case CCTOUCHBEGAN: delegate->ccTouchesBegan(touches, event); break;
                case CCTOUCHMOVED: delegate->ccTouchesMoved(touches, event); break;
                case CCTOUCHENDED: delegate->ccTouchesEnded(touches, event); break;
                case CCTOUCHCANCELLED: delegate->ccTouchesCancelled(touches, event); break;
            }
        }
    }

    $override
    void touches(CCSet* touches, CCEvent* event, unsigned int index) {
        m_bLocked = true;

        //
        // process the target handlers 1st
        //
        if (m_pTargetedHandlers->count() > 0) {
            this->handleTargetedHandlers(touches, event, index);
        }
        //
        // process standard handlers 2nd
        //
        if (m_pStandardHandlers->count() > 0 && touches->m_pSet->size() > 0) {
            this->handleStandardHandlers(touches, event, index);
        }

        //
        // Optimization. To prevent a [handlers copy] which is expensive
        // the add/removes/quit is done after the iterations
        //
        m_bLocked = false;
        if (m_bToRemove) {
            m_bToRemove = false;
            for (unsigned int i = 0; i < m_pHandlersToRemove->num; ++i) {
                for (auto handler : CCArrayExt<CCTargetedTouchHandler*>(m_pTargetedHandlers)) {
                    // crashes here indexing into m_pHandlesToRemove->arr (this+72 +16 +i*4)
                    // i == 0 in all crashlogs (r9)
                    if (handler->getDelegate() == m_pHandlersToRemove->arr[i]) {
                        m_pTargetedHandlers->removeObject(handler);
                        break;
                    }
                }
                for (auto handler : CCArrayExt<CCStandardTouchHandler*>(m_pStandardHandlers)) {
                    if (handler->getDelegate() == m_pHandlersToRemove->arr[i]) {
                        m_pStandardHandlers->removeObject(handler);
                        break;
                    }
                }
            }
            m_pHandlersToRemove->num = 0;
        }

        if (m_bToAdd) {
            m_bToAdd = false;
            for (auto handler : CCArrayExt<CCTouchHandler*>(m_pHandlersToAdd)) {
                if (!handler) continue;

                if (typeinfo_cast<CCTargetedTouchHandler*>(handler)) {
                    if (!m_pTargetedHandlers->containsObject(handler)) {
                        m_pTargetedHandlers->addObject(handler);
                    }
                }
                else {
                    if (!m_pStandardHandlers->containsObject(handler)) {
                        m_pStandardHandlers->addObject(handler);
                    }
                }
            }

            m_pHandlersToAdd->removeAllObjects();
        }

        if (m_bToQuit) {
            m_bToQuit = false;
            m_pStandardHandlers->removeAllObjects();
            m_pTargetedHandlers->removeAllObjects();
        }
    }

    bool handleSingleTargetedHandlersWithFilter(
        CCTouch* touch, CCEvent* event, unsigned int index, CCNode* filter
    ) {
        std::vector<ParentPath<CCTargetedTouchHandler>> invalidRoots;
        auto registeredPaths = this->getRegisteredPaths<CCTargetedTouchHandler>(
            m_pTargetedHandlers, filter, invalidRoots
        );

        if (index == CCTOUCHBEGAN) logInvalidRoots(invalidRoots);

        return this->handleSingleTargetedHandlers(nullptr, touch, event, index, registeredPaths);
    }
};

#include <Geode/modify/GJBaseGameLayer.hpp>

struct ZOrderUILayerHelper : Modify<ZOrderUILayerHelper, GJBaseGameLayer> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("Z Order based touch dispatcher");

    $override
    bool init() override {
        if (!GJBaseGameLayer::init()) return false;

        // This one exists to move ui layer in a sensible location that allows it to actually track touches while playing
        if (m_uiLayer) m_uiLayer->setZOrder(2);

        return true;
    }
};

#include <Geode/modify/EditorUI.hpp>

struct ZOrderEditorUIHelper : Modify<ZOrderEditorUIHelper, EditorUI> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("Z Order based touch dispatcher");

    struct Fields {
        bool m_inObjectsLayer = false;
    };

    $override
    bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
        if (dispatchToLayers(touch, event, CCTOUCHBEGAN)) {
            m_fields->m_inObjectsLayer = true;
            return true;
        }
        return EditorUI::ccTouchBegan(touch, event);
    }

    $override
    void ccTouchMoved(CCTouch* touch, CCEvent* event) override {
        if (m_fields->m_inObjectsLayer) {
            dispatchToLayers(touch, event, CCTOUCHMOVED);
            return;
        }
        EditorUI::ccTouchMoved(touch, event);
    }

    $override
    void ccTouchEnded(CCTouch* touch, CCEvent* event) override {
        auto fields = m_fields.self();
        if (fields->m_inObjectsLayer) {
            dispatchToLayers(touch, event, CCTOUCHENDED);
            fields->m_inObjectsLayer = false;
            return;
        }
        EditorUI::ccTouchEnded(touch, event);
    }

    bool dispatchToLayers(CCTouch* touch, CCEvent* event, int type) {
        auto dispatcher = static_cast<ZOrderTouchDispatcher*>(CCTouchDispatcher::get());
        for (auto layer :
             {m_editorLayer->m_inShaderObjectLayer,
              m_editorLayer->m_aboveShaderObjectLayer,
              m_editorLayer->m_objectLayer}) {
            if (dispatcher->handleSingleTargetedHandlersWithFilter(touch, event, type, layer)) {
                return true;
            }
        }
        return false;
    }
};
