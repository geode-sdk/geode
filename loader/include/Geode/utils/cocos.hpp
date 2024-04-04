#pragma once

#include <matjson.hpp>
#include "casts.hpp"
#include "general.hpp"
#include "../DefaultInclude.hpp"
#include <cocos2d.h>
#include <functional>
#include <type_traits>
#include "../loader/Event.hpp"
#include "MiniFunction.hpp"

// support converting ccColor3B / ccColor4B to / from json

template <>
struct matjson::Serialize<cocos2d::ccColor3B> {
    static matjson::Value GEODE_DLL to_json(cocos2d::ccColor3B const& color);
    static cocos2d::ccColor3B GEODE_DLL from_json(matjson::Value const& color);
    static bool GEODE_DLL is_json(matjson::Value const& json);
};

template <>
struct matjson::Serialize<cocos2d::ccColor4B> {
    static matjson::Value GEODE_DLL to_json(cocos2d::ccColor4B const& color);
    static cocos2d::ccColor4B GEODE_DLL from_json(matjson::Value const& color);
    static bool GEODE_DLL is_json(matjson::Value const& json);
};

// operators for CC geometry
namespace cocos2d {
    static cocos2d::CCPoint& operator*=(cocos2d::CCPoint& pos, float mul) {
        pos.x *= mul;
        pos.y *= mul;
        return pos;
    }

    static cocos2d::CCSize& operator*=(cocos2d::CCSize& size, float mul) {
        size.width *= mul;
        size.height *= mul;
        return size;
    }

    static cocos2d::CCSize operator*(cocos2d::CCSize const& size, cocos2d::CCPoint const& point) {
        return {
            size.width * point.x,
            size.height * point.y,
        };
    }

    static cocos2d::CCRect operator*=(cocos2d::CCRect& rect, float mul) {
        rect.origin *= mul;
        rect.size *= mul;
        return rect;
    }

    static cocos2d::CCRect operator*(cocos2d::CCRect const& rect, float mul) {
        return {
            rect.origin.x * mul,
            rect.origin.y * mul,
            rect.size.width * mul,
            rect.size.height * mul,
        };
    }

    static cocos2d::CCPoint operator/=(cocos2d::CCPoint& pos, float div) {
        pos.x /= div;
        pos.y /= div;
        return pos;
    }

    static cocos2d::CCSize operator/=(cocos2d::CCSize& size, float div) {
        size.width /= div;
        size.height /= div;
        return size;
    }

    static cocos2d::CCRect operator/=(cocos2d::CCRect& rect, float div) {
        rect.origin /= div;
        rect.size /= div;
        return rect;
    }

    static cocos2d::CCPoint operator+=(cocos2d::CCPoint& pos, cocos2d::CCPoint const& add) {
        pos.x += add.x;
        pos.y += add.y;
        return pos;
    }

    static cocos2d::CCSize operator+=(cocos2d::CCSize& size, cocos2d::CCPoint const& add) {
        size.width += add.x;
        size.height += add.y;
        return size;
    }

    static cocos2d::CCSize operator+=(cocos2d::CCSize& size, cocos2d::CCSize const& add) {
        size.width += add.width;
        size.height += add.height;
        return size;
    }

    static cocos2d::CCRect operator+=(cocos2d::CCRect& rect, cocos2d::CCPoint const& add) {
        rect.origin += add;
        return rect;
    }

    static cocos2d::CCRect operator+=(cocos2d::CCRect& rect, cocos2d::CCSize const& add) {
        rect.size += add;
        return rect;
    }

    static cocos2d::CCRect operator+=(cocos2d::CCRect& rect, cocos2d::CCRect const& add) {
        rect.origin += add.origin;
        rect.size += add.size;
        return rect;
    }

    static cocos2d::CCPoint operator-=(cocos2d::CCPoint& pos, cocos2d::CCPoint const& add) {
        pos.x -= add.x;
        pos.y -= add.y;
        return pos;
    }

    static cocos2d::CCSize operator-=(cocos2d::CCSize& size, cocos2d::CCPoint const& add) {
        size.width -= add.x;
        size.height -= add.y;
        return size;
    }

    static cocos2d::CCSize operator-=(cocos2d::CCSize& size, cocos2d::CCSize const& add) {
        size.width -= add.width;
        size.height -= add.height;
        return size;
    }

    static cocos2d::CCRect operator-=(cocos2d::CCRect& rect, cocos2d::CCPoint const& add) {
        rect.origin -= add;
        return rect;
    }

    static cocos2d::CCRect operator-=(cocos2d::CCRect& rect, cocos2d::CCSize const& add) {
        rect.size -= add;
        return rect;
    }

    static cocos2d::CCRect operator-=(cocos2d::CCRect& rect, cocos2d::CCRect const& add) {
        rect.origin -= add.origin;
        rect.size -= add.size;
        return rect;
    }

    static cocos2d::CCSize operator-(cocos2d::CCSize const& size, float f) {
        return {size.width - f, size.height - f};
    }

    static cocos2d::CCSize operator-(cocos2d::CCSize const& size) {
        return {-size.width, -size.height};
    }

    static bool operator==(cocos2d::CCPoint const& p1, cocos2d::CCPoint const& p2) {
        return p1.x == p2.x && p1.y == p2.y;
    }

    static bool operator!=(cocos2d::CCPoint const& p1, cocos2d::CCPoint const& p2) {
        return p1.x != p2.x || p1.y != p2.y;
    }

    static bool operator==(cocos2d::CCSize const& s1, cocos2d::CCSize const& s2) {
        return s1.width == s2.width && s1.height == s2.height;
    }

    static bool operator!=(cocos2d::CCSize const& s1, cocos2d::CCSize const& s2) {
        return s1.width != s2.width || s1.height != s2.height;
    }

    static bool operator==(cocos2d::CCRect const& r1, cocos2d::CCRect const& r2) {
        return r1.origin == r2.origin && r1.size == r2.size;
    }

    static bool operator!=(cocos2d::CCRect const& r1, cocos2d::CCRect const& r2) {
        return r1.origin != r2.origin || r1.size != r2.size;
    }

    static bool operator==(cocos2d::ccColor4B const& c1, cocos2d::ccColor4B const& c2) {
        return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
    }

    static bool operator!=(cocos2d::ccColor4B const& c1, cocos2d::ccColor4B const& c2) {
        return c1.r != c2.r || c1.g != c2.g || c1.b != c2.b || c1.a != c2.a;
    }

    static bool operator==(cocos2d::ccColor3B const& c1, cocos2d::ccColor3B const& c2) {
        return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
    }

    static bool operator!=(cocos2d::ccColor3B const& c1, cocos2d::ccColor3B const& c2) {
        return c1.r != c2.r || c1.g != c2.g || c1.b != c2.b;
    }

    static bool operator==(cocos2d::ccHSVValue const& c1, cocos2d::ccHSVValue const& c2) {
        return c1.h == c2.h && c1.s == c2.s && c1.v == c2.v && 
            c1.absoluteSaturation == c2.absoluteSaturation && 
            c1.absoluteBrightness == c2.absoluteBrightness;
    }

    static bool operator!=(cocos2d::ccHSVValue const& c1, cocos2d::ccHSVValue const& c2) {
        return !(c1 == c2);
    }
}

// Ref & Bug
namespace geode {
    /**
     * A smart pointer to a managed CCObject-deriving class. Retains shared
     * ownership over the managed instance. Releases the object when the Ref
     * is destroyed, or assigned another object or nullptr.
     *
     * Use-cases include, for example, non-CCNode class members, or nodes that
     * are not always in the scene tree.
     * 
     * @tparam T A type that inherits from CCObject.
     *
     * @example
     * class MyNode : public CCNode {
     * protected:
     *      // no need to manually call retain or
     *      // release on this array; Ref manages it
     *      // for you :3
     *      Ref<CCArray> m_list = CCArray::create();
     * 
     *      bool init() {
     *          if (!CCNode::init())
     *              return false;
     * 
     *          // No need to do m_list = CCArray::create()
     *          // or m_list->retain() :3
     * 
     *          return true;
     *      }
     * };
     *
     * @example
     * // Save a child from the current layer into a menu
     * Ref<CCMenu> menu = static_cast<CCMenu*>(this->getChildByID("main-menu"));
     *          
     * // Remove the menu from its parent
     * menu->removeFromParent();
     *          
     * // Menu will still point to a valid CCMenu as long as the menu variable exist
     */
    template <class T>
    class Ref final {
        static_assert(
            std::is_base_of_v<cocos2d::CCObject, T>,
            "Ref can only be used with a CCObject-inheriting class!"
        );

        T* m_obj = nullptr;

    public:
        /**
         * Construct a Ref of an object. The object will be retained and
         * managed until Ref goes out of scope
         * @param obj Object to construct the Ref from
         */
        Ref(T* obj) : m_obj(obj) {
            CC_SAFE_RETAIN(obj);
        }

        Ref(Ref<T> const& other) : Ref(other.data()) {}

        Ref(Ref<T>&& other) : m_obj(other.m_obj) {
            other.m_obj = nullptr;
        }

        /**
         * Construct an empty Ref (the managed object will be null)
         */
        Ref() = default;

        ~Ref() {
            CC_SAFE_RELEASE(m_obj);
        }

        /**
         * Swap the managed object with another object. The managed object
         * will be released, and the new object retained
         * @param other The new object to swap to
         */
        void swap(T* other) {
            CC_SAFE_RELEASE(m_obj);
            m_obj = other;
            CC_SAFE_RETAIN(other);
        }

        /**
         * Return the managed object
         * @returns The managed object
         */
        T* data() const {
            return m_obj;
        }

        operator T*() const {
            return m_obj;
        }

        T* operator*() const {
            return m_obj;
        }

        T* operator->() const {
            return m_obj;
        }

        T* operator=(T* obj) {
            this->swap(obj);
            return obj;
        }

        Ref<T>& operator=(Ref<T> const& other) {
            this->swap(other.data());
            return *this;
        }

        Ref<T>& operator=(Ref<T>&& other) {
            this->swap(other.data());
            return *this;
        }

        bool operator==(T* other) const {
            return m_obj == other;
        }

        bool operator==(Ref<T> const& other) const {
            return m_obj == other.m_obj;
        }

        bool operator!=(T* other) const {
            return m_obj != other;
        }

        bool operator!=(Ref<T> const& other) const {
            return m_obj != other.m_obj;
        }

        // for containers
        bool operator<(Ref<T> const& other) const {
            return m_obj < other.m_obj;
        }
        bool operator<=(Ref<T> const& other) const {
            return m_obj <= other.m_obj;
        }
        bool operator>(Ref<T> const& other) const {
            return m_obj > other.m_obj;
        }
        bool operator>=(Ref<T> const& other) const {
            return m_obj >= other.m_obj;
        }
    };

    class WeakRefPool;

    class GEODE_DLL WeakRefController final {
    private:
        cocos2d::CCObject* m_obj;

        WeakRefController(WeakRefController const&) = delete;
        WeakRefController(WeakRefController&&) = delete;

        friend class WeakRefPool;
    
    public:
        WeakRefController() = default;
        
        bool isManaged();
        void swap(cocos2d::CCObject* other);
        cocos2d::CCObject* get() const;
    };

    class GEODE_DLL WeakRefPool final {
        std::unordered_map<cocos2d::CCObject*, std::shared_ptr<WeakRefController>> m_pool;
    
        void check(cocos2d::CCObject* obj);

        friend class WeakRefController;

    public:
        static WeakRefPool* get();
        
        std::shared_ptr<WeakRefController> manage(cocos2d::CCObject* obj);
    };

    /**
     * A smart pointer to a managed CCObject-deriving class. Like Ref, except 
     * only holds a weak reference to the targeted object. When all non-weak 
     * references (Refs, manual retain() calls) to the object are dropped, so 
     * are all weak references.
     * 
     * In essence, WeakRef is like a raw pointer, except that you can know if 
     * the pointer is still valid or not, as WeakRef::lock() returns nullptr if 
     * the pointed-to-object has already been freed.
     *
     * Note that an object pointed to by WeakRef is only released once some 
     * WeakRef pointing to it checks for it after all other references to the 
     * object have been dropped. If you store WeakRefs in a global map, you may 
     * want to periodically lock all of them to make sure any memory that should 
     * be freed is freed.
     * 
     * @tparam T A type that inherits from CCObject.
     */
    template <class T>
    class WeakRef final {
        static_assert(
            std::is_base_of_v<cocos2d::CCObject, T>,
            "WeakRef can only be used with a CCObject-inheriting class!"
        );

        std::shared_ptr<WeakRefController> m_controller;

        WeakRef(std::shared_ptr<WeakRefController> obj) : m_controller(obj) {}

    public:
        /**
         * Construct a WeakRef of an object. A weak reference is one that will 
         * be valid as long as the object is referenced by other strong 
         * references (such as Ref or manual retain calls), but once all strong 
         * references are dropped, so are all weak references. The object is 
         * freed once no strong references exist to it, and any WeakRef pointing 
         * to it is freed or locked
         * @param obj Object to construct the WeakRef from
         */
        WeakRef(T* obj) : m_controller(WeakRefPool::get()->manage(obj)) {}

        WeakRef(WeakRef<T> const& other) : WeakRef(other.m_controller) {}

        WeakRef(WeakRef<T>&& other) : m_controller(std::move(other.m_controller)) {
            other.m_controller = nullptr;
        }

        /**
         * Construct an empty WeakRef (the object will be null)
         */
        WeakRef() = default;
        ~WeakRef() {
            // If the WeakRef is moved, m_controller is null
            if (m_controller) {
                m_controller->isManaged();
            }
        }

        /**
         * Lock the WeakRef, returning a Ref if the pointed object is valid or 
         * a null Ref if the object has been freed
         */
        Ref<T> lock() const {
            if (m_controller->isManaged()) {
                return Ref(static_cast<T*>(m_controller->get()));
            }
            return Ref<T>(nullptr);
        }

        /**
         * Check if the WeakRef points to a valid object
         */
        bool valid() const {
            return m_controller->isManaged();
        }

        /**
         * Swap the managed object with another object. The managed object
         * will be released, and the new object retained
         * @param other The new object to swap to
         */
        void swap(T* other) {
            m_controller->swap(other);
        }

        Ref<T> operator=(T* obj) {
            this->swap(obj);
            return this->lock();
        }

        WeakRef<T>& operator=(WeakRef<T> const& other) {
            this->swap(static_cast<T*>(other.m_controller->get()));
            return *this;
        }

        WeakRef<T>& operator=(WeakRef<T>&& other) {
            this->swap(static_cast<T*>(other.m_controller->get()));
            return *this;
        }

        explicit operator bool() const noexcept {
            return this->valid();
        }

        bool operator==(T* other) const {
            return m_controller->get() == other;
        }

        bool operator==(WeakRef<T> const& other) const {
            return m_controller->get() == other.m_controller->get();
        }

        bool operator!=(T* other) const {
            return m_controller->get() != other;
        }

        bool operator!=(WeakRef<T> const& other) const {
            return m_controller->get() != other.m_controller->get();
        }

        // for containers
        bool operator<(WeakRef<T> const& other) const {
            return m_controller->get() < other.m_controller->get();
        }
        bool operator<=(WeakRef<T> const& other) const {
            return m_controller->get() <= other.m_controller->get();
        }
        bool operator>(WeakRef<T> const& other) const {
            return m_controller->get() > other.m_controller->get();
        }
        bool operator>=(WeakRef<T> const& other) const {
            return m_controller->get() >= other.m_controller->get();
        }
    };

    template <class Filter>
    class EventListenerNode : public cocos2d::CCNode {
    protected:
        EventListener<Filter> m_listener;

        EventListenerNode(EventListener<Filter>&& listener)
          : m_listener(std::move(listener)) {}
    
    public:
        static EventListenerNode* create(EventListener<Filter> listener) {
            auto ret = new EventListenerNode(std::move(listener));
            if (ret && ret->init()) {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }

        static EventListenerNode* create(typename Filter::Callback callback, Filter filter = Filter()) {
            auto ret = new EventListenerNode(EventListener<Filter>(callback, filter));
            if (ret && ret->init()) {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }

        template <class C>
        static EventListenerNode* create(
            C* cls, typename EventListener<Filter>::template MemberFn<C> callback
        ) {
            // for some reason msvc won't let me just call EventListenerNode::create...
            // it claims no return value...
            // despite me writing return EventListenerNode::create()......
            auto ret = new EventListenerNode(EventListener<Filter>(cls, callback));
            if (ret && ret->init()) {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }
    };
}

// Cocos2d utils
namespace geode::cocos {
    /**
     * Get child at index. Checks bounds. A negative
     * index will get the child starting from the end
     * @returns Child at index cast to the given type,
     * or nullptr if index exceeds bounds
     */
    template <class T = cocos2d::CCNode>
    static T* getChild(cocos2d::CCNode* x, int i) {
        // start from end for negative index
        if (i < 0) i = x->getChildrenCount() + i;
        // check if backwards index is out of bounds
        if (i < 0) return nullptr;
        // check if forwards index is out of bounds
        if (static_cast<int>(x->getChildrenCount()) <= i) return nullptr;
        return static_cast<T*>(x->getChildren()->objectAtIndex(i));
    }

    /**
     * Get nth child that is a given type. Checks bounds.
     * @returns Child at index cast to the given type,
     * or nullptr if index exceeds bounds
     */
    template <class Type = cocos2d::CCNode>
    static Type* getChildOfType(cocos2d::CCNode* node, int index) {
        size_t indexCounter = 0;
        if (node->getChildrenCount() == 0) return nullptr;
        // start from end for negative index
        if (index < 0) {
            index = -index - 1;
            for (size_t i = node->getChildrenCount() - 1; i >= 0; i--) {
                auto obj = cast::typeinfo_cast<Type*>(node->getChildren()->objectAtIndex(i));
                if (obj != nullptr) {
                    if (indexCounter == index) {
                        return obj;
                    }
                    ++indexCounter;
                }
                if (i == 0) break;
            }
        }
        else {
            for (size_t i = 0; i < node->getChildrenCount(); i++) {
                auto obj = cast::typeinfo_cast<Type*>(node->getChildren()->objectAtIndex(i));
                if (obj != nullptr) {
                    if (indexCounter == index) {
                        return obj;
                    }
                    ++indexCounter;
                }
            }
        }

        return nullptr;
    }

    /**
     * Return a node, or create a default one if it's
     * nullptr. Syntactic sugar function
     */
    template <class T, class... Args>
    static T* nodeOrDefault(T* node, Args... args) {
        return node ? node : T::create(args...);
    }

    /**
     * Get bounds for a set of nodes. Based on content
     * size
     * @param nodes Nodes to calculate coverage of
     * @returns Rectangle fitting all nodes. Origin
     * will be <= 0 and size will be >= 0
     */
    GEODE_DLL cocos2d::CCRect calculateNodeCoverage(std::vector<cocos2d::CCNode*> const& nodes);
    /**
     * Get bounds for a set of nodes. Based on content
     * size
     * @param nodes Nodes to calculate coverage of
     * @returns Rectangle fitting all nodes. Origin
     * will be <= 0 and size will be >= 0
     */
    GEODE_DLL cocos2d::CCRect calculateNodeCoverage(cocos2d::CCArray* nodes);
    /**
     * Get bounds for a set of nodes. Based on content
     * size
     * @param parent Parent whose children to calculate
     * coverage of
     * @returns Rectangle fitting all the parent's children.
     * Origin will be <= 0 and size will be >= 0
     */
    GEODE_DLL cocos2d::CCRect calculateChildCoverage(cocos2d::CCNode* parent);

    /**
     * Create a CCScene from a layer and switch to it with the default fade
     * transition
     * @param layer Layer to create a scene from
     * @returns Created scene (not the fade transition)
     */
    GEODE_DLL cocos2d::CCScene* switchToScene(cocos2d::CCLayer* layer);

    using CreateLayerFunc = utils::MiniFunction<cocos2d::CCLayer*()>;

    /**
     * Reload textures, overwriting the scene to return to after the loading
     * screen is finished
     * @param returnTo A function that returns a new layer. After loading is
     * finished, the game switches to the given layer instead of MenuLayer.
     * Leave nullptr to enable default behaviour
     */
    GEODE_DLL void reloadTextures(CreateLayerFunc returnTo = nullptr);

    /**
     * Rescale node to fit inside given size
     * @param node Node to rescale
     * @param size Size to fit inside
     * @param def Default size
     * @param min Minimum size
     */
    GEODE_DLL void limitNodeSize(cocos2d::CCNode* node, cocos2d::CCSize const& size, float def, float min);

    /**
     * Checks if a node is visible (recursively
     * checks parent visibility)
     * @param node Node to check if visible
     * @returns True if node is visibile. Does
     * not take into account if node is off-screen
     */
    GEODE_DLL bool nodeIsVisible(cocos2d::CCNode* node);

    /**
     * Gets a node by tag by traversing
     * children recursively
     *
     * @param node Parent node
     * @param tag Target tag
     * @return Child node with specified tag, or
     * null if there is none
     */
    GEODE_DLL cocos2d::CCNode* getChildByTagRecursive(cocos2d::CCNode* node, int tag);

    /**
     *  Get first node that conforms to the predicate
     *  by traversing children recursively
     *
     *  @param node Parent node
     *  @param predicate Predicate used to evaluate nodes
     * @return Child node if one is found, or null if
     * there is none
     */
    template <class Type = cocos2d::CCNode>
    Type* findFirstChildRecursive(cocos2d::CCNode* node, utils::MiniFunction<bool(Type*)> predicate) {
        if (cast::safe_cast<Type*>(node) && predicate(static_cast<Type*>(node)))
            return static_cast<Type*>(node);

        auto children = node->getChildren();
        if (!children) return nullptr;

        for (int i = 0; i < children->count(); ++i) {
            auto newParent = static_cast<cocos2d::CCNode*>(children->objectAtIndex(i));
            auto child = findFirstChildRecursive(newParent, predicate);
            if (child) return child;
        }

        return nullptr;
    }

    /**
     * Checks if a node has the given sprite frame
     * name either in the sprite or in the sprite inside
     * the button.
     * 
     * @param node Node to check
     * @param name Name of the sprite frame to search for
     * @returns True if the node has the given sprite frame
     * name
     */
    GEODE_DLL bool isSpriteFrameName(cocos2d::CCNode* node, const char* name);

    /**
     * Get the first child that has the given sprite frame
     * name either in the sprite or in the sprite inside
     * the button.
     *
     * @param parent Parent node to search in
     * @param name Name of the sprite frame to search for
     * @returns Child with the given sprite frame name, or
     * nullptr if there is none
     */
    GEODE_DLL cocos2d::CCNode* getChildBySpriteFrameName(cocos2d::CCNode* parent, const char* name);

    /**
     * Checks if a given file exists in CCFileUtils
     * search paths.
     * @param filename File to check
     * @returns True if file exists
     * @example if (fileExistsInSearchPaths("mySprite.png"_spr)) {
     *      CCSprite::create("mySprite.png"_spr);
     * } else {
     *      CCSprite::create("fallback.png");
     * }
     */
    GEODE_DLL bool fileExistsInSearchPaths(char const* filename);

    inline void ccDrawColor4B(cocos2d::ccColor4B const& color) {
        cocos2d::ccDrawColor4B(color.r, color.g, color.b, color.a);
    }

    inline cocos2d::ccColor4B invert4B(cocos2d::ccColor4B const& color) {
        return {
            static_cast<GLubyte>(255 - color.r),
            static_cast<GLubyte>(255 - color.g),
            static_cast<GLubyte>(255 - color.b),
            color.a};
    }

    inline cocos2d::ccColor3B invert3B(cocos2d::ccColor3B const& color) {
        return {
            static_cast<GLubyte>(255 - color.r),
            static_cast<GLubyte>(255 - color.g),
            static_cast<GLubyte>(255 - color.b)};
    }

    inline cocos2d::ccColor3B lighten3B(cocos2d::ccColor3B const& color, int amount) {
        return {
            static_cast<GLubyte>(utils::clamp(color.r + amount, 0, 255)),
            static_cast<GLubyte>(utils::clamp(color.g + amount, 0, 255)),
            static_cast<GLubyte>(utils::clamp(color.b + amount, 0, 255)),
        };
    }

    inline cocos2d::ccColor3B darken3B(cocos2d::ccColor3B const& color, int amount) {
        return lighten3B(color, -amount);
    }

    inline cocos2d::ccColor3B to3B(cocos2d::ccColor4B const& color) {
        return {color.r, color.g, color.b};
    }

    inline cocos2d::ccColor4B to4B(cocos2d::ccColor3B const& color, GLubyte alpha = 255) {
        return {color.r, color.g, color.b, alpha};
    }

    inline cocos2d::ccColor4F to4F(cocos2d::ccColor4B const& color) {
        return {color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f};
    }

    constexpr cocos2d::ccColor3B cc3x(int hexValue) {
        if (hexValue <= 0xf)
            return cocos2d::ccColor3B{
                static_cast<GLubyte>(hexValue * 17),
                static_cast<GLubyte>(hexValue * 17),
                static_cast<GLubyte>(hexValue * 17)};
        if (hexValue <= 0xff)
            return cocos2d::ccColor3B{
                static_cast<GLubyte>(hexValue),
                static_cast<GLubyte>(hexValue),
                static_cast<GLubyte>(hexValue)};
        if (hexValue <= 0xfff)
            return cocos2d::ccColor3B{
                static_cast<GLubyte>((hexValue >> 8 & 0xf) * 17),
                static_cast<GLubyte>((hexValue >> 4 & 0xf) * 17),
                static_cast<GLubyte>((hexValue >> 0 & 0xf) * 17)};
        else
            return cocos2d::ccColor3B{
                static_cast<GLubyte>(hexValue >> 16 & 0xff),
                static_cast<GLubyte>(hexValue >> 8 & 0xff),
                static_cast<GLubyte>(hexValue >> 0 & 0xff)};
    }

    /**
     * Parse a ccColor3B from a hexadecimal string. The string may contain 
     * a leading '#'
     * @param hexValue The string to parse into a color
     * @param permissive If true, strings like "f" are considered valid 
     * representations of the color white. Useful for UIs that allow entering 
     * a hex color. Empty strings evaluate to pure white
     * @returns A ccColor3B if it could be succesfully parsed, or an error 
     * indicating the failure reason
     */
    GEODE_DLL Result<cocos2d::ccColor3B> cc3bFromHexString(std::string const& hexValue, bool permissive = false);
    /**
     * Parse a ccColor4B from a hexadecimal string. The string may contain 
     * a leading '#'
     * @param hexValue The string to parse into a color
     * @param requireAlpha Require the alpha component to be passed. If false, 
     * alpha defaults to 255
     * @param permissive If true, strings like "f" are considered valid 
     * representations of the color white. Useful for UIs that allow entering 
     * a hex color. Empty strings evaluate to pure white
     * @returns A ccColor4B if it could be succesfully parsed, or an error 
     * indicating the failure reason
     */
    GEODE_DLL Result<cocos2d::ccColor4B> cc4bFromHexString(std::string const& hexValue, bool requireAlpha = false, bool permissive = false);
    GEODE_DLL std::string cc3bToHexString(cocos2d::ccColor3B const& color);
    GEODE_DLL std::string cc4bToHexString(cocos2d::ccColor4B const& color);

    template <typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
    static cocos2d::CCArray* vectorToCCArray(std::vector<T> const& vec) {
        auto res = cocos2d::CCArray::createWithCapacity(vec.size());
        for (auto const& item : vec)
            res->addObject(item);
        return res;
    }

    template <typename T, typename C, typename = std::enable_if_t<std::is_pointer_v<C>>>
    static cocos2d::CCArray* vectorToCCArray(std::vector<T> const& vec, utils::MiniFunction<C(T)> convFunc) {
        auto res = cocos2d::CCArray::createWithCapacity(vec.size());
        for (auto const& item : vec)
            res->addObject(convFunc(item));
        return res;
    }

    template <typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
    std::vector<T> ccArrayToVector(cocos2d::CCArray* arr) {
        return std::vector<T>(
            reinterpret_cast<T*>(arr->data->arr), reinterpret_cast<T*>(arr->data->arr) + arr->data->num
        );
    }

    template <
        typename K, typename V,
        typename = std::enable_if_t<std::is_same_v<K, std::string> || std::is_same_v<K, intptr_t>>>
    static cocos2d::CCDictionary* mapToCCDict(std::map<K, V> const& map) {
        auto res = cocos2d::CCDictionary::create();
        for (auto const& [key, value] : map)
            res->setObject(value, key);
        return res;
    }

    template <
        typename K, typename V, typename C,
        typename = std::enable_if_t<std::is_same_v<C, std::string> || std::is_same_v<C, intptr_t>>>
    static cocos2d::CCDictionary* mapToCCDict(std::map<K, V> const& map, utils::MiniFunction<C(K)> convFunc) {
        auto res = cocos2d::CCDictionary::create();
        for (auto const& [key, value] : map)
            res->setObject(value, convFunc(key));
        return res;
    }

    /**
     * Gets the mouse position in cocos2d coordinates.
     * On mobile platforms this will probably return (0, 0)
     * @returns The mouse position
     */
    GEODE_DLL cocos2d::CCPoint getMousePos();
}

// std specializations
namespace std {
    // enables using Ref as the key in unordered_map etc.
    template <class T>
    struct hash<geode::Ref<T>> {
        size_t operator()(geode::Ref<T> const& ref) const {
            return std::hash<T*>()(ref.data());
        }
    };
}

// more utils
namespace geode::cocos {
    struct GEODE_DLL CCArrayInserter {
    public:
        CCArrayInserter(cocos2d::CCArray* p) : m_array(p) {}

        cocos2d::CCArray* m_array;

        auto& operator=(cocos2d::CCObject* value) {
            m_array->addObject(value);
            return *this;
        }

        auto& operator*() {
            return *this;
        }

        auto& operator++() {
            return *this;
        }
    };

    template <class T>
    concept CocosObjectPtr = std::is_pointer_v<T> && std::is_convertible_v<T, cocos2d::CCObject const*>;

    template <class K>
    concept CocosDictionaryKey = std::same_as<K, int> || std::same_as<K, intptr_t> || std::same_as<K, gd::string> || std::same_as<K, std::string>;

    /**
     * A templated wrapper over CCArray, providing easy iteration and indexing.
     * This will keep ownership of the given CCArray*.
     * 
     * @tparam Type Pointer to a type that inherits CCObject.
     *
     * @example
     * CCArrayExt<GameObject*> objects = PlayLayer::get()->m_objects;
     * // Easy indexing, giving you the type you assigned
     * GameObject* myObj = objects[2];
     * 
     * // Easy iteration using C++ range-based for loops
     * for (auto* obj : objects) {
     *   log::info("{}", obj->m_objectID);
     * }
     */
    template <CocosObjectPtr Type>
    class CCArrayExt {
    protected:
        Ref<cocos2d::CCArray> m_arr;
        using T = std::remove_pointer_t<Type>;

    public:
        using value_type = T;
        using iterator = T**;
        using const_iterator = const T**;

        CCArrayExt() : m_arr(cocos2d::CCArray::create()) {}

        CCArrayExt(cocos2d::CCArray* arr)
          : m_arr(arr) {}

        CCArrayExt(CCArrayExt const& a) : m_arr(a.m_arr) {}

        CCArrayExt(CCArrayExt&& a) : m_arr(a.m_arr) {
            a.m_arr = nullptr;
        }

        ~CCArrayExt() {}

        T** begin() const {
            if (!m_arr) {
                return nullptr;
            }
            return reinterpret_cast<T**>(m_arr->data->arr);
        }

        T** end() const {
            if (!m_arr) {
                return nullptr;
            }
            return reinterpret_cast<T**>(m_arr->data->arr) + m_arr->count();
        }

        auto rbegin() const {
            return std::reverse_iterator(this->end());
        }

        auto rend() const {
            return std::reverse_iterator(this->begin());
        }

        size_t size() const {
            return m_arr ? m_arr->count() : 0;
        }

        T* operator[](size_t index) {
            return static_cast<T*>(m_arr->objectAtIndex(index));
        }

        void push_back(T* item) {
            m_arr->addObject(item);
        }

        T* pop_back() {
            T ret = m_arr->lastObject();
            m_arr->removeLastObject();
            return ret;
        }

        cocos2d::CCArray* inner() {
            return m_arr;
        }
    };

    template <typename K, typename T>
    struct CCDictIterator {
    public:
        CCDictIterator(cocos2d::CCDictElement* p) : m_ptr(p) {}

        cocos2d::CCDictElement* m_ptr;

        std::pair<K, T> operator*() {
            if constexpr (std::is_same_v<K, std::string> || std::is_same_v<K, gd::string>) {
                return {m_ptr->getStrKey(), static_cast<T>(m_ptr->getObject())};
            }
            else {
                return {m_ptr->getIntKey(), static_cast<T>(m_ptr->getObject())};
            }
        }

        auto& operator++() {
            m_ptr = static_cast<decltype(m_ptr)>(m_ptr->hh.next);
            return *this;
        }

        friend bool operator==(CCDictIterator<K, T> const& a, CCDictIterator<K, T> const& b) {
            return a.m_ptr == b.m_ptr;
        };

        friend bool operator!=(CCDictIterator<K, T> const& a, CCDictIterator<K, T> const& b) {
            return a.m_ptr != b.m_ptr;
        };

        bool operator!=(int b) {
            return m_ptr != nullptr;
        }
    };

    template <typename K, typename T>
    struct CCDictEntry {
        K m_key;
        cocos2d::CCDictionary* m_dict;

        CCDictEntry(K key, cocos2d::CCDictionary* dict) : m_key(key), m_dict(dict) {}

        T operator->() {
            return static_cast<T>(m_dict->objectForKey(m_key));
        }

        operator T() {
            return static_cast<T>(m_dict->objectForKey(m_key));
        }

        CCDictEntry& operator=(T f) {
            m_dict->setObject(f, m_key);
            return *this;
        }
    };

    /**
     * A templated wrapper over CCDictionary, providing easy iteration and indexing.
     * This will keep ownership of the given CCDictionary*.
     * 
     * @tparam Key Type of the key. MUST only be int or gd::string or std::string.
     * @tparam ValuePtr Pointer to a type that inherits CCObject.
     *
     * @example
     * CCDictionaryExt<std::string, GJGameLevel*> levels = getSomeDict();
     * // Easy indexing, giving you the type you assigned
     * GJGameLevel* myLvl = levels["Cube Adventures"];
     * 
     * // Easy iteration using C++ range-based for loops
     * for (auto [name, level] : levels) {
     *   log::info("{}: {}", name, level->m_levelID);
     * }
     */
    template <CocosDictionaryKey Key, CocosObjectPtr ValuePtr>
    struct CCDictionaryExt {
    protected:
        Ref<cocos2d::CCDictionary> m_dict;

    public:
        CCDictionaryExt() : m_dict(cocos2d::CCDictionary::create()) {}

        CCDictionaryExt(cocos2d::CCDictionary* dict) : m_dict(dict) {}

        CCDictionaryExt(CCDictionaryExt const& d) : m_dict(d.m_dict) {}

        CCDictionaryExt(CCDictionaryExt&& d) : m_dict(d.m_dict) {
            d.m_dict = nullptr;
        }

        auto begin() {
            return CCDictIterator<Key, ValuePtr>(m_dict->m_pElements);
        }

        // do not use this
        auto end() {
            return nullptr;
        }

        size_t size() {
            return m_dict->count();
        }

        auto operator[](const Key& key) {
            auto ret = static_cast<ValuePtr>(m_dict->objectForKey(key));
            if (!ret) m_dict->setObject(cocos2d::CCNode::create(), key);

            return CCDictEntry<Key, ValuePtr>(key, m_dict);
        }

        bool contains(const Key& key) {
            return m_dict->objectForKey(key) != nullptr;
        }

        size_t count(const Key& key) {
            return this->contains(key) ? 1 : 0;
        }

        cocos2d::CCDictionary* inner() {
            return m_dict;
        }
    };

    void GEODE_DLL handleTouchPriorityWith(cocos2d::CCNode* node, int priority, bool force = false);
    void GEODE_DLL handleTouchPriority(cocos2d::CCNode* node, bool force = false);
}
