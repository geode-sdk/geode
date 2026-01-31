#pragma once

#include <matjson.hpp>
#include "casts.hpp"
#include "general.hpp"
#include "../DefaultInclude.hpp"
#include <Geode/utils/ZStringView.hpp>
#include <cocos2d.h>
#include <functional>
#include <type_traits>
#include "../loader/Event.hpp"
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <unordered_map>
#include "../ui/Layout.hpp"
#include "../ui/SpacerNode.hpp"

// support converting ccColor3B / ccColor4B to / from json

template <>
struct matjson::Serialize<cocos2d::ccColor3B> {
    static geode::Result<cocos2d::ccColor3B> GEODE_DLL fromJson(Value const& value);
    static Value GEODE_DLL toJson(cocos2d::ccColor3B const& value);
};

template <>
struct matjson::Serialize<cocos2d::ccColor4B> {
    static geode::Result<cocos2d::ccColor4B> GEODE_DLL fromJson(Value const& value);
    static Value GEODE_DLL toJson(cocos2d::ccColor4B const& value);
};

namespace geode::cocos {
    template <class InpT, bool Retain>
    class CCArrayExt;
    template <class Key, class ValueInpT, bool Retain>
    class CCDictionaryExt; 
}

template <typename T>
struct ::geode::CCArrayExtCheck<T, void> {
    using type = cocos::CCArrayExt<T, true>;
};

template <typename K, typename V>
struct ::geode::CCDictionaryExtCheck<K, V, void> {
    using type = cocos::CCDictionaryExt<K, V, true>;
};

// operators for CC geometry
namespace cocos2d {
    static constexpr cocos2d::CCPoint& operator*=(cocos2d::CCPoint& pos, float mul) {
        pos.x *= mul;
        pos.y *= mul;
        return pos;
    }
    static constexpr cocos2d::CCSize& operator*=(cocos2d::CCSize& size, float mul) {
        size.width *= mul;
        size.height *= mul;
        return size;
    }
    static constexpr cocos2d::CCSize operator*(cocos2d::CCSize const& size, cocos2d::CCPoint const& point) {
        return {
            size.width * point.x,
            size.height * point.y,
        };
    }
    static constexpr cocos2d::CCRect operator*=(cocos2d::CCRect& rect, float mul) {
        rect.origin *= mul;
        rect.size *= mul;
        return rect;
    }
    static constexpr cocos2d::CCRect operator*(cocos2d::CCRect const& rect, float mul) {
        return {
            rect.origin.x * mul,
            rect.origin.y * mul,
            rect.size.width * mul,
            rect.size.height * mul,
        };
    }
    static constexpr cocos2d::CCPoint operator/=(cocos2d::CCPoint& pos, float div) {
        pos.x /= div;
        pos.y /= div;
        return pos;
    }
    static constexpr cocos2d::CCSize operator/=(cocos2d::CCSize& size, float div) {
        size.width /= div;
        size.height /= div;
        return size;
    }
    static constexpr cocos2d::CCRect operator/=(cocos2d::CCRect& rect, float div) {
        rect.origin /= div;
        rect.size /= div;
        return rect;
    }
    static constexpr cocos2d::CCPoint operator+=(cocos2d::CCPoint& pos, cocos2d::CCPoint const& add) {
        pos.x += add.x;
        pos.y += add.y;
        return pos;
    }
    static constexpr cocos2d::CCSize operator+=(cocos2d::CCSize& size, cocos2d::CCPoint const& add) {
        size.width += add.x;
        size.height += add.y;
        return size;
    }
    static constexpr cocos2d::CCSize operator+=(cocos2d::CCSize& size, cocos2d::CCSize const& add) {
        size.width += add.width;
        size.height += add.height;
        return size;
    }
    static constexpr cocos2d::CCRect operator+=(cocos2d::CCRect& rect, cocos2d::CCPoint const& add) {
        rect.origin += add;
        return rect;
    }
    static constexpr cocos2d::CCRect operator+=(cocos2d::CCRect& rect, cocos2d::CCSize const& add) {
        rect.size += add;
        return rect;
    }
    static constexpr cocos2d::CCRect operator+=(cocos2d::CCRect& rect, cocos2d::CCRect const& add) {
        rect.origin += add.origin;
        rect.size += add.size;
        return rect;
    }
    static constexpr cocos2d::CCPoint operator-=(cocos2d::CCPoint& pos, cocos2d::CCPoint const& add) {
        pos.x -= add.x;
        pos.y -= add.y;
        return pos;
    }
    static constexpr cocos2d::CCSize operator-=(cocos2d::CCSize& size, cocos2d::CCPoint const& add) {
        size.width -= add.x;
        size.height -= add.y;
        return size;
    }
    static constexpr cocos2d::CCSize operator-=(cocos2d::CCSize& size, cocos2d::CCSize const& add) {
        size.width -= add.width;
        size.height -= add.height;
        return size;
    }
    static constexpr cocos2d::CCRect operator-=(cocos2d::CCRect& rect, cocos2d::CCPoint const& add) {
        rect.origin -= add;
        return rect;
    }
    static constexpr cocos2d::CCRect operator-=(cocos2d::CCRect& rect, cocos2d::CCSize const& add) {
        rect.size -= add;
        return rect;
    }
    static constexpr cocos2d::CCRect operator-=(cocos2d::CCRect& rect, cocos2d::CCRect const& add) {
        rect.origin -= add.origin;
        rect.size -= add.size;
        return rect;
    }
    static constexpr cocos2d::CCSize operator-(cocos2d::CCSize const& size, float f) {
        return {size.width - f, size.height - f};
    }
    static constexpr cocos2d::CCSize operator-(cocos2d::CCSize const& size) {
        return {-size.width, -size.height};
    }
    static constexpr bool operator==(cocos2d::CCPoint const& p1, cocos2d::CCPoint const& p2) {
        return p1.x == p2.x && p1.y == p2.y;
    }
    static constexpr bool operator!=(cocos2d::CCPoint const& p1, cocos2d::CCPoint const& p2) {
        return p1.x != p2.x || p1.y != p2.y;
    }
    static constexpr bool operator==(cocos2d::CCSize const& s1, cocos2d::CCSize const& s2) {
        return s1.width == s2.width && s1.height == s2.height;
    }
    static constexpr bool operator!=(cocos2d::CCSize const& s1, cocos2d::CCSize const& s2) {
        return s1.width != s2.width || s1.height != s2.height;
    }
    static constexpr bool operator<(cocos2d::CCSize const& s1, cocos2d::CCSize const& s2) {
        return s1.width < s2.width && s1.height < s2.height;
    }
    static constexpr bool operator<=(cocos2d::CCSize const& s1, cocos2d::CCSize const& s2) {
        return s1.width <= s2.width && s1.height <= s2.height;
    }
    static constexpr bool operator>(cocos2d::CCSize const& s1, cocos2d::CCSize const& s2) {
        return s1.width > s2.width && s1.height > s2.height;
    }
    static constexpr bool operator>=(cocos2d::CCSize const& s1, cocos2d::CCSize const& s2) {
        return s1.width >= s2.width && s1.height >= s2.height;
    }
    static constexpr bool operator==(cocos2d::CCRect const& r1, cocos2d::CCRect const& r2) {
        return r1.origin == r2.origin && r1.size == r2.size;
    }
    static constexpr bool operator!=(cocos2d::CCRect const& r1, cocos2d::CCRect const& r2) {
        return r1.origin != r2.origin || r1.size != r2.size;
    }
    static constexpr bool operator==(cocos2d::ccColor4B const& c1, cocos2d::ccColor4B const& c2) {
        return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
    }
    static constexpr bool operator!=(cocos2d::ccColor4B const& c1, cocos2d::ccColor4B const& c2) {
        return c1.r != c2.r || c1.g != c2.g || c1.b != c2.b || c1.a != c2.a;
    }
    static constexpr bool operator==(cocos2d::ccColor3B const& c1, cocos2d::ccColor3B const& c2) {
        return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
    }
    static constexpr bool operator!=(cocos2d::ccColor3B const& c1, cocos2d::ccColor3B const& c2) {
        return c1.r != c2.r || c1.g != c2.g || c1.b != c2.b;
    }
    static constexpr bool operator==(cocos2d::ccHSVValue const& c1, cocos2d::ccHSVValue const& c2) {
        return c1.h == c2.h && c1.s == c2.s && c1.v == c2.v &&
            c1.absoluteSaturation == c2.absoluteSaturation &&
            c1.absoluteBrightness == c2.absoluteBrightness;
    }
    static constexpr bool operator!=(cocos2d::ccHSVValue const& c1, cocos2d::ccHSVValue const& c2) {
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

        Ref(Ref<T>&& other) noexcept : m_obj(other.m_obj) {
            other.m_obj = nullptr;
        }

        /**
         * Construct an empty Ref (the managed object will be null)
         */
        Ref() = default;

        /**
         * Construct a Ref of an object, without retaining it.
         * The object will still be released when Ref goes out of scope.
         * @param obj Object to construct the Ref from
         */
        static Ref<T> adopt(T* obj) {
            Ref<T> ref;
            ref.m_obj = obj;
            return ref;
        }

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
         * Takes out the object from the Ref, without calling `release` on it.
         * This is like a symmetric counterpart to `Ref::adopt`, it essentially "leaks" the object,
         * making the Ref empty and making you responsible for releasing it manually.
         * @returns The managed object
         */
        T* take() {
            auto obj = m_obj;
            m_obj = nullptr;
            return obj;
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
            m_obj = other.data();
            other.m_obj = nullptr;
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

        // Releases the object from the pool, removing the strong reference to it
        void forget(cocos2d::CCObject* obj);

        friend class WeakRefController;

        template <class T>
        friend class WeakRef;

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

        friend class std::hash<WeakRef<T>>;


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
        WeakRef(T* obj) : m_controller(obj ? WeakRefPool::get()->manage(obj) : nullptr) {}

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

                if (m_controller.use_count() == 2) {
                    // if refcount is 2 (this WeakRef + pool), free the object to avoid leaks
                    WeakRefPool::get()->forget(m_controller->get());
                }
            }
        }

        /**
         * Lock the WeakRef, returning a Ref if the pointed object is valid or
         * a null Ref if the object has been freed
         */
        Ref<T> lock() const {
            if (m_controller && m_controller->isManaged()) {
                return Ref(static_cast<T*>(m_controller->get()));
            }
            return Ref<T>(nullptr);
        }

        /**
         * Check if the WeakRef points to a valid object
         */
        bool valid() const {
            return m_controller && m_controller->isManaged();
        }

        /**
         * Swap the managed object with another object. The managed object
         * will be released, and the new object retained
         * @param other The new object to swap to
         */
        void swap(T* other) {
            if (m_controller) {
                m_controller->swap(other);
            } else if (other) {
                m_controller = WeakRefPool::get()->manage(other);
            } else {
                m_controller = nullptr;
            }
        }

        Ref<T> operator=(T* obj) {
            this->swap(obj);
            return this->lock();
        }

        WeakRef<T>& operator=(WeakRef<T> const& other) {
            this->swap(static_cast<T*>(other.m_controller ? other.m_controller->get() : nullptr));
            return *this;
        }

        WeakRef<T>& operator=(WeakRef<T>&& other) {
            m_controller = std::move(other.m_controller);
            return *this;
        }

        explicit operator bool() const noexcept {
            return this->valid();
        }

        bool operator==(T* other) const {
            return (m_controller && m_controller->get() == other) || (!m_controller && !other);
        }

        bool operator==(WeakRef<T> const& other) const {
            if (!m_controller && !other.m_controller) return true;
            if (!m_controller || !other.m_controller) return false;

            return m_controller->get() == other.m_controller->get();
        }

        bool operator!=(T* other) const {
            return !(*this == other);
        }

        bool operator!=(WeakRef<T> const& other) const {
            return !(*this == other);
        }

        // for containers
        bool operator<(WeakRef<T> const& other) const {
            if (!m_controller && !other.m_controller) return false;
            if (!m_controller) return true;
            if (!other.m_controller) return false;

            return m_controller->get() < other.m_controller->get();
        }
        bool operator<=(WeakRef<T> const& other) const {
            return !(*this > other);
        }
        bool operator>(WeakRef<T> const& other) const {
            return other < *this;
        }
        bool operator>=(WeakRef<T> const& other) const {
            return !(*this < other);
        }
    };

    /**
     * A utility node that allows for EventListener's to be added
     * as a node to a class, delegating memory handling onto cocos.
     * This is helpful when there is a need to tie a listener onto
     * a node.
     *
     * @example
     * this->addChild(EventListenerNode<MyFilter>::create([&](){
     *     // handling code here
     * }));
     * @example
     *
     * @tparam Filter The event filter this listener uses. See
     * `EventListener` for more information.
     */
    class EventListenerNode : public cocos2d::CCNode {
    protected:
        ListenerHandle m_handle;

        EventListenerNode(ListenerHandle&& handle)
          : m_handle(std::move(handle)) {}

    public:
        static EventListenerNode* create(ListenerHandle&& handle) {
            auto ret = new EventListenerNode(std::move(handle));
            if (ret->init()) {
                ret->autorelease();
                return ret;
            }
            delete ret;
            return nullptr;
        }

        template <class Event, class Callback>
        static EventListenerNode* create(Event event, Callback&& callback, int priority) {
            auto ret = new EventListenerNode(event.listen(std::forward<Callback>(callback), priority));
            if (ret->init()) {
                ret->autorelease();
                return ret;
            }
            delete ret;
            return nullptr;
        }
    };

    /**
     * A simple `CCObject` wrapper for a non-`CCObject` type. This is
     * useful for storing custom types in a `CCNode::setUserObject` without
     * making a new class for it.
     *
     * @tparam T The type to wrap into a CCObject
     */
    template <class T>
        requires (!std::is_base_of_v<T, cocos2d::CCObject>)
    class ObjWrapper : public cocos2d::CCObject {
    protected:
        T m_value;

        ObjWrapper(T&& value) : m_value(std::forward<T>(value)) {
            this->autorelease();
        }
        ObjWrapper(T const& value) : m_value(value) {
            this->autorelease();
        }

    public:
        /**
         * Construct an object wrapper
         */
        static ObjWrapper* create(T&& value) {
            return new ObjWrapper(std::forward<T>(value));
        }
        /**
         * Construct an object wrapper
         */
        static ObjWrapper* create(T const& value) {
            return new ObjWrapper(value);
        }

        T& getValue() {
            return m_value;
        }
        void setValue(T&& value) {
            m_value = std::forward<T>(value);
        }
    };
}

// Cocos2d utils
namespace geode::cocos {
    /**
     * Get child at index. Checks bounds. A negative
     * index will get the child starting from the end
     * @deprecated Use CCNode::getChildByIndex instead
     * @returns Child at index cast to the given type,
     * or nullptr if index exceeds bounds
     */
    template <class T = cocos2d::CCNode>
    static T* getChild(cocos2d::CCNode* x, int i) {
        return x->getChildByIndex<T>(i);
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
    GEODE_DLL cocos2d::CCRect calculateNodeCoverage(std::span<cocos2d::CCNode*> nodes);
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

    using CreateLayerFunc = geode::Function<cocos2d::CCLayer*()>;

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
     * Rescale node to fit inside given width
     * @param node Node to rescale
     * @param width Width to fit inside
     * @param def Default scale
     * @param min Minimum scale
     */
    GEODE_DLL void limitNodeWidth(cocos2d::CCNode* node, float width, float def, float min);

    /**
     * Rescale node to fit inside given height
     * @param node Node to rescale
     * @param height Height to fit inside
     * @param def Default scale
     * @param min Minimum scale
     */
    GEODE_DLL void limitNodeHeight(cocos2d::CCNode* node, float height, float def, float min);

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
     * Checks if a node has the given sprite name either
     * in the sprite or in the sprite inside the button.
     *
     * @param node Node to check
     * @param name Name of the sprite to search for
     * @returns True if the node has the given sprite name
     */
    GEODE_DLL bool isSpriteName(cocos2d::CCNode* node, const char* name);

    /**
     * Get the first child that has the given sprite name
     * either in the sprite or in the sprite inside the
     * button.
     *
     * @param parent Parent node to search in
     * @param name Name of the sprite to search for
     * @returns Child with the given sprite name, or
     * nullptr if there is none
     */
    GEODE_DLL cocos2d::CCNode* getChildBySpriteName(cocos2d::CCNode* parent, const char* name);

    /**
     * Gets the demangled name of an object using RTTI. The returned name does not include 'struct' or 'class'
     * @param obj Object to get the name of
     * @returns Demangled name of the object
     */
    GEODE_DLL std::string_view getObjectName(cocos2d::CCObject const* obj);

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

    /**
     * Calls `cocos2d::ccDrawColor4B` directly with a `cocos2d::ccColor4B` color
     *
     * @param color The color to draw
     */
    inline void ccDrawColor4B(cocos2d::ccColor4B const& color) {
        cocos2d::ccDrawColor4B(color.r, color.g, color.b, color.a);
    }

    /**
     * Inverts the color's rgb values.
     *
     * @param color The color to invert
     * @returns Converted color
     */
    inline cocos2d::ccColor4B invert4B(cocos2d::ccColor4B const& color) {
        return {
            static_cast<GLubyte>(255 - color.r),
            static_cast<GLubyte>(255 - color.g),
            static_cast<GLubyte>(255 - color.b),
            color.a};
    }

    /**
     * Inverts the color's rgb values.
     *
     * @param color The color to invert
     * @returns Converted color
     */
    inline cocos2d::ccColor3B invert3B(cocos2d::ccColor3B const& color) {
        return {
            static_cast<GLubyte>(255 - color.r),
            static_cast<GLubyte>(255 - color.g),
            static_cast<GLubyte>(255 - color.b)};
    }

    /**
     * Lightens the color's rgb values by a given amount.
     *
     * @param color The color to lighten
     * @param amount The amount to lighten
     * @returns Converted color
     */
    inline cocos2d::ccColor3B lighten3B(cocos2d::ccColor3B const& color, int amount) {
        return {
            static_cast<GLubyte>(std::clamp(color.r + amount, 0, 255)),
            static_cast<GLubyte>(std::clamp(color.g + amount, 0, 255)),
            static_cast<GLubyte>(std::clamp(color.b + amount, 0, 255)),
        };
    }

    /**
     * Darkens the color's rgb values by a given amount.
     *
     * @param color The color to darken
     * @param amount The amount to darken
     * @returns Converted color
     */
    inline cocos2d::ccColor3B darken3B(cocos2d::ccColor3B const& color, int amount) {
        return lighten3B(color, -amount);
    }

    /**
     * Converts a `cocos2d::ccColor4B` into `cocos2d::ccColor3B`
     *
     * @param color The color to convert
     * @returns Converted color
     */
    inline cocos2d::ccColor3B to3B(cocos2d::ccColor4B const& color) {
        return {color.r, color.g, color.b};
    }

    /**
     * Converts a `cocos2d::ccColor3B` into `cocos2d::ccColor4B`
     *
     * @param color The color to convert
     * @param alpha The additional alpha value
     * @returns Converted color
     */
    inline cocos2d::ccColor4B to4B(cocos2d::ccColor3B const& color, GLubyte alpha = 255) {
        return {color.r, color.g, color.b, alpha};
    }

    /**
     * Converts a `cocos2d::ccColor4B` into `cocos2d::ccColor`4F
     *
     * @param color The color to convert
     * @returns Converted color
     */
    inline cocos2d::ccColor4F to4F(cocos2d::ccColor4B const& color) {
        return {color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f};
    }

    /**
     * Parse a ccColor3B from a hexadecimal string. The string may contain
     * a leading '#'
     * @param hexValue The string to parse into a color
     * @param permissive If true, strings like "f" are considered valid
     * representations of the color white. Useful for UIs that allow entering
     * a hex color. Empty strings evaluate to pure white
     * @returns A ccColor3B if it could be successfully parsed, or an error
     * indicating the failure reason
     */
    GEODE_DLL Result<cocos2d::ccColor3B> cc3bFromHexString(std::string_view hexValue, bool permissive = false);
    /**
     * Parse a ccColor4B from a hexadecimal string. The string may contain
     * a leading '#'
     * @param hexValue The string to parse into a color
     * @param requireAlpha Require the alpha component to be passed. If false,
     * alpha defaults to 255
     * @param permissive If true, strings like "f" are considered valid
     * representations of the color white. Useful for UIs that allow entering
     * a hex color. Empty strings evaluate to pure white
     * @returns A ccColor4B if it could be successfully parsed, or an error
     * indicating the failure reason
     */
    GEODE_DLL Result<cocos2d::ccColor4B> cc4bFromHexString(std::string_view hexValue, bool requireAlpha = false, bool permissive = false);

    /**
     * Converts a `cocos2d::ccColor3B` into a string based on the `RRGGBB` format.
     *
     * @param color The color to convert
     * @returns Hex string
     */
    GEODE_DLL std::string cc3bToHexString(cocos2d::ccColor3B const& color);

    /**
     * Converts a `cocos2d::ccColor4B` into a string based on the `RRGGBBAA` format.
     *
     * @param color The color to convert
     * @returns Hex string
     */
    GEODE_DLL std::string cc4bToHexString(cocos2d::ccColor4B const& color);

    /**
     * Gets the mouse position in cocos2d coordinates.
     * On mobile platforms this will probably return (0, 0)
     * @returns The mouse position
     */
    GEODE_DLL cocos2d::CCPoint getMousePos();


    /**
     * Create an ObjWrapper without having to specify the template argument
     * @param value The value to pass into ObjWrapper::create
     * @returns The created ObjWrapper
     */
    template <typename T>
    ObjWrapper<T>* makeObjWrapper(T&& value) {
        return ObjWrapper<T>::create(std::forward<T>(value));
    }

    /**
     * Get the size of a label with given text and font
     * @param text The text of the label
     * @param font The font name of the label
     * @param kerning Extra kerning to apply to the label
     * @returns The size of the label
     */
    GEODE_DLL cocos2d::CCSize getLabelSize(std::u16string_view text, const char* font, int kerning = 0);

    /**
     * Get the size of a label with given text and font
     * @param text The text of the label
     * @param font The font name of the label
     * @param kerning Extra kerning to apply to the label
     * @returns The size of the label
     */
    GEODE_DLL cocos2d::CCSize getLabelSize(std::string_view text, const char* font, int kerning = 0);
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

    template <typename T>
    struct hash<geode::WeakRef<T>> {
        size_t operator()(geode::WeakRef<T> const& ref) const {
            // the explicit template argument is needed here because it would otherwise cast to WeakRef and recurse
            return std::hash<std::shared_ptr<geode::WeakRefController>>{}(ref.m_controller);
        }
    };
}

// more utils
namespace geode::cocos {
    /**
     * A `std::back_inserter` like utility class that acts as an inserter
     * for `CCArray`.
     */
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
    concept CocosObject = std::derived_from<T, cocos2d::CCObject>;

    template <class T>
    concept CocosObjectPtr = std::is_pointer_v<T> && std::is_convertible_v<T, cocos2d::CCObject const*>;

    template <class K>
    concept CocosDictionaryKey = std::same_as<K, int> || std::same_as<K, intptr_t> || std::same_as<K, gd::string> || std::same_as<K, std::string> || std::same_as<K, std::string_view> || std::same_as<K, const char*>;

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
    template <class InpT = cocos2d::CCObject, bool Retain = true>
    class CCArrayExt {
    protected:
        using T = std::remove_pointer_t<InpT>;
        using Container = std::conditional_t<Retain, Ref<cocos2d::CCArray>, cocos2d::CCArray*>;
        static_assert(CocosObject<T>);

        Container m_arr;

    public:
        using value_type = T*;
        using iterator = T**;
        using const_iterator = const T**;

        CCArrayExt() : m_arr(cocos2d::CCArray::create()) {}

        CCArrayExt(cocos2d::CCArray* arr)
          : m_arr(arr) {}

        CCArrayExt(std::vector<T> const& vec) : m_arr(cocos2d::CCArray::createWithCapacity(vec.size())) {
            for (auto obj : vec) {
                m_arr->addObject(obj);
            }
        }

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
            T* ret = static_cast<T*>(m_arr->lastObject());
            m_arr->removeLastObject();
            return ret;
        }

        cocos2d::CCArray* inner() {
            return m_arr;
        }

        std::vector<T> toVector() const {
            std::vector<T> vec;
            vec.reserve(this->size());
            for (auto item : *this) {
                vec.push_back(item);
            }
            return vec;
        }

        bool empty() const {
            return this->size() == 0;
        }

        T* front() const {
            return static_cast<T*>(m_arr->firstObject());
        }

        T* back() const {
            return static_cast<T*>(m_arr->lastObject());
        }
    };

    /**
     * A templated wrapper over `CCDictElement`, acting as a simple iterator over
     * `CCDictionary`.
     *
     * @tparam Type Pointer to a type that inherits CCObject.
     */
    template <class K, class T>
    struct CCDictIterator {
    public:
        CCDictIterator(cocos2d::CCDictElement* p) : m_ptr(p) {}

        cocos2d::CCDictElement* m_ptr;

        std::pair<K, T*> operator*() {
            if constexpr (
                std::is_same_v<K, std::string>
                || std::is_same_v<K, gd::string>
                || std::is_same_v<K, std::string_view>
                || std::is_same_v<K, const char*>)
            {
                return {m_ptr->getStrKey(), static_cast<T*>(m_ptr->getObject())};
            }
            else {
                return {m_ptr->getIntKey(), static_cast<T*>(m_ptr->getObject())};
            }
        }

        auto& operator++() {
            m_ptr = static_cast<decltype(m_ptr)>(m_ptr->hh.next);
            return *this;
        }

        friend bool operator==(CCDictIterator const& a, CCDictIterator const& b) {
            return a.m_ptr == b.m_ptr;
        }

        friend bool operator!=(CCDictIterator const& a, CCDictIterator const& b) {
            return a.m_ptr != b.m_ptr;
        }
    };

    /**
     * A simple struct that as an entry to a `CCDictionary`.
     *
     * @tparam Type Pointer to a type that inherits CCObject.
     */
    template <class K, class T>
    struct CCDictEntry {
        K m_key;
        cocos2d::CCDictionary* m_dict;

        CCDictEntry(K key, cocos2d::CCDictionary* dict) : m_key(key), m_dict(dict) {}

        T* operator->() {
            return static_cast<T*>(m_dict->objectForKey(m_key));
        }

        operator T*() {
            return static_cast<T*>(m_dict->objectForKey(m_key));
        }

        CCDictEntry& operator=(T* f) {
            m_dict->setObject(f, m_key);
            return *this;
        }
    };

    /**
     * A templated wrapper over CCDictionary, providing easy iteration and indexing.
     * This will keep ownership of the given CCDictionary*.
     *
     * @tparam Key Type of the key. MUST be one of: int, std::string_view (recommended), gd::string, std::string.
     * @tparam ValuePtr Pointer to a type that inherits CCObject.
     *
     * @example
     * CCDictionaryExt<std::string_view, GJGameLevel*> levels = getSomeDict();
     * // Easy indexing, giving you the type you assigned
     * GJGameLevel* myLvl = levels["Cube Adventures"];
     *
     * // Easy iteration using C++ range-based for loops
     * for (auto [name, level] : levels) {
     *   log::info("{}: {}", name, level->m_levelID);
     * }
     */
    template <class Key = std::string_view, class ValueInpT = cocos2d::CCObject, bool Retain = true>
    struct CCDictionaryExt {
    protected:
        using Value = std::remove_pointer_t<ValueInpT>;
        using ValuePtr = Value*;
        using Container = std::conditional_t<Retain, Ref<cocos2d::CCDictionary>, cocos2d::CCDictionary*>;
        using Entry = CCDictEntry<Key, Value>;
        using Iterator = CCDictIterator<Key, Value>;
        static_assert(CocosDictionaryKey<Key>);
        static_assert(CocosObject<Value>);

        Container m_dict;

    public:
        CCDictionaryExt() : m_dict(cocos2d::CCDictionary::create()) {}

        CCDictionaryExt(cocos2d::CCDictionary* dict) : m_dict(dict) {}

        template<CocosDictionaryKey MapKey>
        CCDictionaryExt(std::unordered_map<MapKey, ValuePtr> const& map) : m_dict(cocos2d::CCDictionary::create()) {
            for (auto& [k, v] : map) {
                m_dict->setObject(v, k);
            }
        }

        CCDictionaryExt(CCDictionaryExt const& d) : m_dict(d.m_dict) {}

        CCDictionaryExt(CCDictionaryExt&& d) : m_dict(std::exchange(d.m_dict, nullptr)) {}

        auto begin() {
            return Iterator(m_dict->m_pElements);
        }

        auto end() {
            return Iterator(nullptr);
        }

        size_t size() {
            return m_dict->count();
        }

        Entry operator[](const Key& key) {
            auto ret = static_cast<ValuePtr>(m_dict->objectForKey(key));
            if (!ret) m_dict->setObject(cocos2d::CCNode::create(), key);

            return Entry(key, m_dict);
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

        bool empty() const {
            return this->size() == 0;
        }
    };

    /**
     * Contains various utility functions related to `CCMenuItem` set of classes.
     */
    struct CCMenuItemExt {
    private:
        template <class Node>
        class LambdaCallback : public cocos2d::CCObject {
        public:
            geode::Function<void(Node*)> m_callback;

            static LambdaCallback* create(geode::Function<void(Node*)> callback) {
                auto ret = new (std::nothrow) LambdaCallback();
                if (ret->init(std::move(callback))) {
                    ret->autorelease();
                    return ret;
                }
                delete ret;
                return nullptr;
            }

            bool init(geode::Function<void(Node*)> callback) {
                m_callback = std::move(callback);
                return true;
            }

            void execute(cocos2d::CCNode* node) {
                m_callback(static_cast<Node*>(node));
            }
        };

    public:
        /**
         * Creates a `CCMenuItem` with a callback.
         *
         * @param callback The callback for the button
         * @returns The created button
         */
        static cocos2d::CCMenuItem* create(
            geode::Function<void(cocos2d::CCMenuItem*)> callback
        ) {
            auto item = cocos2d::CCMenuItem::create();
            assignCallback(item, std::move(callback));
            return item;
        }

        /**
         * Creates a `CCMenuItemSprite` with a normal and a selected sprite.
         *
         * @param normalSprite The sprite used on idle
         * @param selectedSprite The sprite used when selected
         * @param callback The callback for the button
         * @returns The created button
         */
        static cocos2d::CCMenuItemSprite* createSprite(
            cocos2d::CCNode* normalSprite,
            cocos2d::CCNode* selectedSprite,
            geode::Function<void(cocos2d::CCMenuItemSprite*)> callback
        ) {
            auto item = cocos2d::CCMenuItemSprite::create(normalSprite, selectedSprite);
            assignCallback(item, std::move(callback));
            return item;
        }

        /**
         * Creates a `CCMenuItemSprite` with a disabled, normal and a selected sprite.
         *
         * @param normalSprite The sprite used on idle
         * @param selectedSprite The sprite used when selected
         * @param disabledSprite The sprite used when disabled
         * @param callback The callback for the button
         * @returns The created button
         */
        static cocos2d::CCMenuItemSprite* createSprite(
            cocos2d::CCNode* normalSprite,
            cocos2d::CCNode* selectedSprite,
            cocos2d::CCNode* disabledSprite,
            geode::Function<void(cocos2d::CCMenuItemSprite*)> callback
        ) {
            auto item = cocos2d::CCMenuItemSprite::create(normalSprite, selectedSprite, disabledSprite);
            assignCallback(item, std::move(callback));
            return item;
        }

        /**
         * Creates a `CCMenuItemSpriteExtra` with a sprite and a callback.
         *
         * @param normalSprite The sprite for the button
         * @param callback The callback for the button
         * @returns The created button
         */
        static CCMenuItemSpriteExtra* createSpriteExtra(
            cocos2d::CCNode* normalSprite,
            geode::Function<void(CCMenuItemSpriteExtra*)> callback
        ) {
            auto item = CCMenuItemSpriteExtra::create(normalSprite, nullptr, nullptr);
            assignCallback(item, std::move(callback));
            return item;
        }

        /**
         * Creates a `CCMenuItemSpriteExtra` with a file name and a sprite scale.
         *
         * @param normalSpriteName The file name used for the normal sprite
         * @param scale The scale used for the sprite
         * @param callback The callback for the button
         * @returns The created button
         */
        static CCMenuItemSpriteExtra* createSpriteExtraWithFilename(
            ZStringView normalSpriteName,
            float scale,
            geode::Function<void(CCMenuItemSpriteExtra*)> callback
        ) {
            auto sprite = cocos2d::CCSprite::create(normalSpriteName.c_str());
            sprite->setScale(scale);

            return createSpriteExtra(sprite, std::move(callback));
        }

        /**
         * Creates a `CCMenuItemSpriteExtra` with a frame name and a sprite scale.
         *
         * @param normalSpriteName The frame name used for the normal sprite
         * @param scale The scale used for the sprite
         * @param callback The callback for the button
         * @returns The created button
         */
        static CCMenuItemSpriteExtra* createSpriteExtraWithFrameName(
            ZStringView normalSpriteName,
            float scale,
            geode::Function<void(CCMenuItemSpriteExtra*)> callback
        ) {
            auto sprite = cocos2d::CCSprite::createWithSpriteFrameName(normalSpriteName.c_str());
            sprite->setScale(scale);

            return createSpriteExtra(sprite, std::move(callback));
        }

        /**
         * Creates a `CCMenuItemToggler` with an on and off sprite.
         *
         * @param onSprite The sprite used when toggled on
         * @param offSprite The sprite used when toggled off
         * @param callback The callback for the toggle
         * @returns The created toggle
         */
        static CCMenuItemToggler* createToggler(
            cocos2d::CCNode* onSprite,
            cocos2d::CCNode* offSprite,
            geode::Function<void(CCMenuItemToggler*)> callback
        ) {
            auto item = CCMenuItemToggler::create(offSprite, onSprite, nullptr, nullptr);
            assignCallback(item, std::move(callback));
            return item;
        }

        /**
         * Creates a `CCMenuItemToggler` with standard toggle sprites GD uses.
         *
         * @param scale The scale of the sprites
         * @param callback The callback for the toggle
         * @returns The created toggle
         */
        static CCMenuItemToggler* createTogglerWithStandardSprites(
            float scale,
            geode::Function<void(CCMenuItemToggler*)> callback
        ) {
            auto offSprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
            auto onSprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");

            offSprite->setScale(scale);
            onSprite->setScale(scale);

            return createToggler(onSprite, offSprite, std::move(callback));
        }

        /**
         * Creates a `CCMenuItemToggler` with an on and off sprite file name
         *
         * @param onSpriteName The file name thats used to create the toggled on sprite
         * @param offSpriteName The file name thats used to create the toggled off sprite
         * @param callback The callback for the toggle
         * @returns The created toggle
         */
        static CCMenuItemToggler* createTogglerWithFilename(
            ZStringView onSpriteName,
            ZStringView offSpriteName,
            float scale,
            geode::Function<void(CCMenuItemToggler*)> callback
        ) {
            auto offSprite = cocos2d::CCSprite::create(offSpriteName.c_str());
            auto onSprite = cocos2d::CCSprite::create(onSpriteName.c_str());

            offSprite->setScale(scale);
            onSprite->setScale(scale);

            return createToggler(onSprite, offSprite, std::move(callback));
        }

        /**
         * Creates a `CCMenuItemToggler` with an on and off sprite frame name
         *
         * @param onSpriteName The frame name thats used to create the toggled on sprite
         * @param offSpriteName The frame name thats used to create the toggled off sprite
         * @param callback The callback for the toggle
         * @returns The created toggle
         */
        static CCMenuItemToggler* createTogglerWithFrameName(
            ZStringView onSpriteName,
            ZStringView offSpriteName,
            float scale,
            geode::Function<void(CCMenuItemToggler*)> callback
        ) {
            auto offSprite = cocos2d::CCSprite::createWithSpriteFrameName(offSpriteName.c_str());
            auto onSprite = cocos2d::CCSprite::createWithSpriteFrameName(onSpriteName.c_str());

            offSprite->setScale(scale);
            onSprite->setScale(scale);

            return createToggler(onSprite, offSprite, std::move(callback));
        }

        /**
         * Assigns a lambda callback to a `CCMenuItem`
         *
         * @param item The item to assign callback to
         * @param callback The callback to assign
         */
        template <class Node>
        static void assignCallback(
            cocos2d::CCMenuItem* item,
            geode::Function<void(Node*)> callback
        ) {
            auto lambda = LambdaCallback<Node>::create(std::move(callback));
            item->setTarget(lambda, menu_selector(LambdaCallback<Node>::execute));
            item->setUserObject("lambda-callback", lambda);
        }
    };

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
    Type* findFirstChildRecursive(cocos2d::CCNode* gnode, geode::FunctionRef<bool(Type*)> predicate) {
        auto node = cast::typeinfo_cast<Type*>(gnode);
        if (node && predicate(node))
            return node;

        for (auto child : gnode->getChildrenExt()) {
            auto result = findFirstChildRecursive(child, predicate);
            if (result) return result;
        }

        return nullptr;
    }

    /**
     * CCCallFunc alternative that accepts a lambda (or any function object)
     *
     * @tparam The type of the function object
     */
    template <std::invocable F>
    class CallFuncExtImpl : public cocos2d::CCActionInstant {
    public:
        static CallFuncExtImpl* create(const F& func) {
            auto ret = new CallFuncExtImpl(func);
            ret->autorelease();
            return ret;
        }

        static CallFuncExtImpl* create(F&& func) {
            auto ret = new CallFuncExtImpl(std::move(func));
            ret->autorelease();
            return ret;
        }

    private:
        F m_func;

        // F may not be default-constructible
        CallFuncExtImpl(F&& func) : m_func(std::move(func)) {}
        CallFuncExtImpl(F const& func) : m_func(func) {}

        void update(float) override {
            // Make sure any `geode::Function`s are valid
            if constexpr (requires { static_cast<bool>(m_func); }) {
                if (m_func) m_func();
            } else {
                m_func();
            }
        }
    };

    // small hack to allow template deduction
    struct CallFuncExt {
        template <std::invocable F>
        static auto create(F&& func) {
            using Fd = std::decay_t<F>;

            return CallFuncExtImpl<Fd>::create(std::forward<F>(func));
        }
    };

    /**
     * A utility function that recursively sets the touch priorities
     * of a node and its children.
     *
     * Not very recommended for use but there are cases it can quickly
     * fix some touch handler related problems.
     *
     * @param node The parent node to set touch priority to
     * @param priority The priority value to set to
     * @param force Whether it should force set without smaller-than checks
     */
    void GEODE_DLL handleTouchPriorityWith(cocos2d::CCNode* node, int priority, bool force = false);

    /**
     * A utility function that recursively sets the touch priorities
     * of a node and its children.
     *
     * Not very recommended for use but there are cases it can quickly
     * fix some touch handler related problems.
     *
     * @param node The parent node to set touch priority to
     * @param force Whether it should force set without smaller-than checks
     */
    void GEODE_DLL handleTouchPriority(cocos2d::CCNode* node, bool force = false);
}