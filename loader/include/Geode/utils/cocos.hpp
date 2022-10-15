#pragma once

#include <Geode/DefaultInclude.hpp>
#include <cocos2d.h>
#include <functional>
#include <type_traits>
#include "Ref.hpp"

namespace geode::cocos {
    /**
     * Get child at index. Checks bounds. A negative 
     * index will get the child starting from the end
     * @returns Child at index cast to the given type, 
     * or nullptr if index exceeds bounds
     */
    template<class T = cocos2d::CCNode>
    static T* getChild(cocos2d::CCNode* x, int i) {
        // start from end for negative index
        if (i < 0) i = x->getChildrenCount() + i;
        // check if backwards index is out of bounds
        if (i < 0) return nullptr;
        // check if forwards index is out of bounds
        if (static_cast<int>(x->getChildrenCount()) <= i) return nullptr;
        return reinterpret_cast<T*>(x->getChildren()->objectAtIndex(i));
    }

    /**
     * Get nth child that is a given type. Checks bounds. 
     * @returns Child at index cast to the given type, 
     * or nullptr if index exceeds bounds
     */
    template<class Type = cocos2d::CCNode>
    static Type* getChildOfType(cocos2d::CCNode* node, size_t index) {
    	size_t indexCounter = 0;

		for (size_t i = 0; i < node->getChildrenCount(); ++i) {
			auto obj = cast::typeinfo_cast<Type*>(
				node->getChildren()->objectAtIndex(i)
			);
			if (obj != nullptr) {
				if (indexCounter == index) {
					return obj;
				}
				++indexCounter;
			}
		}

		return nullptr;
    }

    /**
     * Return a node, or create a default one if it's 
     * nullptr. Syntactic sugar function
     */
    template<class T, class... Args>
    static T* nodeOrDefault(T* node, Args... args) {
        return node ? node : T::create(args...);
    }

    template<class T = cocos2d::CCNode>
    struct SafeCreate final {
        T* result;
        SafeCreate<T>& with(T* node) {
            result = node;
            return *this;
        }
        template<class... Args>
        SafeCreate<T>& make(Args... args) {
            result = T::create(args...);
            return *this;
        }
        // convenience for CCSprite
        template<class... Args>
        SafeCreate<T>& makeWithFrame(Args... args) {
            result = T::createWithSpriteFrameName(args...);
            return *this;
        }
        template<class... Args>
        SafeCreate<T>& makeUsing(T*(*func)(Args...), Args... args) {
            result = func(args...);
            return *this;
        }
        template<class O = T, class... Args>
        T* orMakeUsing(O*(*func)(Args...), Args... args) {
            if (result) return result;
            return func(args...);
        }
        template<class O = T, class... Args>
        T* orMake(Args... args) {
            if (result) return result;
            return O::create(args...);
        }
        template<class O = T, class... Args>
        T* orMakeWithFrame(Args... args) {
            if (result) return result;
            return O::createWithSpriteFrameName(args...);
        }
    };

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
     * Rescale node to fit inside given size
     * @param node Node to rescale
     * @param size Size to fit inside
     * @param def Default size
     * @param min Minimum size
     */
    GEODE_DLL void limitNodeSize(
        cocos2d::CCNode* node,
        cocos2d::CCSize const& size,
        float def,
        float min
    );

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
    GEODE_DLL bool fileExistsInSearchPaths(const char* filename);


    template <typename T>
    struct CCArrayIterator {
    public:
        CCArrayIterator(T* p) : m_ptr(p) {}
        T* m_ptr;

        auto& operator*() {
            return *m_ptr;
        }
        auto& operator*() const {
            return *m_ptr;
        }
        auto operator->() {
            return m_ptr;
        }

        auto operator->() const {
            return m_ptr;
        }

        auto& operator++() {
            ++m_ptr;
            return *this;
        }

        auto& operator--() {
            --m_ptr;
            return *this;
        }

        auto& operator+=(size_t val) {
            m_ptr += val;
            return *this;
        }

        auto& operator-=(size_t val) {
            m_ptr -= val;
            return *this;
        }

        auto operator+(size_t val) const {
            return CCArrayIterator<T>(m_ptr + val);
        }

        auto operator-(size_t val) const {
            return CCArrayIterator<T>(m_ptr - val);
        }

        auto operator-(CCArrayIterator<T> const& other) const {
            return m_ptr - other.m_ptr;
        }

        bool operator<(CCArrayIterator<T> const& other) const {
            return m_ptr < other.m_ptr;
        }

        bool operator>(CCArrayIterator<T> const& other) const {
            return m_ptr > other.m_ptr;
        }

        bool operator<=(CCArrayIterator<T> const& other) const {
            return m_ptr <= other.m_ptr;
        }

        bool operator>=(CCArrayIterator<T> const& other) const {
            return m_ptr >= other.m_ptr;
        }

        bool operator==(CCArrayIterator<T> const& other) const {
            return m_ptr == other.m_ptr;
        }
        bool operator!=(CCArrayIterator<T> const& other) const {
            return m_ptr != other.m_ptr;
        }
    };

}

namespace std {
    template <typename T>
    struct iterator_traits<geode::cocos::CCArrayIterator<T>> {
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag; // its random access but im too lazy to implement it
    };
}

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

    template <typename _Type>
    class CCArrayExt {
    protected:
        Ref<cocos2d::CCArray> m_arr;
        using T = std::remove_pointer_t<_Type>;
    public:
        CCArrayExt() : m_arr(cocos2d::CCArray::create()) {}
        CCArrayExt(cocos2d::CCArray* arr) : m_arr(arr) {}
        CCArrayExt(CCArrayExt const& a) : m_arr(a.m_arr) {}
        CCArrayExt(CCArrayExt&& a) : m_arr(a.m_arr) {
            a.m_arr = nullptr;
        }
        ~CCArrayExt() {}

        auto begin() {
            return CCArrayIterator<T*>(reinterpret_cast<T**>(m_arr->data->arr));
        }
        auto end() {
            return CCArrayIterator<T*>(reinterpret_cast<T**>(m_arr->data->arr) + m_arr->count());
        }
        auto size() const {
            return m_arr->count();
        }
        T operator[](size_t index) {
            return reinterpret_cast<T*>(m_arr->objectAtIndex(index));
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
            if constexpr (std::is_same<K, std::string>::value) {
                return { m_ptr->getStrKey(), reinterpret_cast<T>(m_ptr->getObject()) };
            } else {
                return { m_ptr->getIntKey(), reinterpret_cast<T>(m_ptr->getObject()) };
            }
        }

        auto& operator++() {
            m_ptr = reinterpret_cast<decltype(m_ptr)>(m_ptr->hh.next);
            return *this;
        }

        friend bool operator== (const CCDictIterator<K, T>& a, const CCDictIterator<K, T>& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const CCDictIterator<K, T>& a, const CCDictIterator<K, T>& b) { return a.m_ptr != b.m_ptr; };
        bool operator!= (int b) { return m_ptr != nullptr; }
    };

    template <typename K, typename T>
    struct CCDictEntry {
        K m_key;
        cocos2d::CCDictionary* m_dict;

        CCDictEntry(K key, cocos2d::CCDictionary* dict) : m_key(key), m_dict(dict) {}

        T operator->() {
            return reinterpret_cast<T>(m_dict->objectForKey(m_key));
        }

        operator T() {
            return reinterpret_cast<T>(m_dict->objectForKey(m_key));
        }

        CCDictEntry& operator=(T f) {
            m_dict->setObject(f, m_key);
            return *this;
        }
    };


    template <typename K, typename T>
    struct CCDictionaryExt {
     protected:
        cocos2d::CCDictionary* m_dict;
    public:
        CCDictionaryExt() : m_dict(cocos2d::CCDictionary::create()) {
            m_dict->retain();
        }   
        CCDictionaryExt(cocos2d::CCDictionary* dict) : m_dict(dict) {
            m_dict->retain();
        }
        CCDictionaryExt(CCDictionaryExt const& d) : m_dict(d.m_dict) {
            m_dict->retain();
        }
        CCDictionaryExt(CCDictionaryExt&& d) : m_dict(d.m_dict) {
            d.m_dict = nullptr;
        }
        ~CCDictionaryExt() {
            if (m_dict)
                m_dict->release();
        }

        CCDictionaryExt const& operator=(cocos2d::CCDictionary* d) {
            m_dict->release();
            m_dict = d;
            m_dict->retain();
        }

        auto begin() {
            return CCDictIterator<K, T*>(m_dict->m_pElements);
        }
        // do not use this
        auto end() {
            return nullptr;
        }
        size_t size() { return m_dict->count(); }
        auto operator[](K key) {
            auto ret = reinterpret_cast<T*>(m_dict->objectForKey(key));
            if (!ret)
                m_dict->setObject(cocos2d::CCNode::create(), key);

            return CCDictEntry<K, T*>(key, m_dict);
        }
        size_t count(K key) {
            return m_dict->allKeys(key)->count();
        }
    };
    
    // namespace for storing implementation stuff for 
    // inline member functions
    namespace {
        // class that holds the lambda (probably should've just used 
        // std::function but hey, this one's heap-free!)
        template<class F, class Ret, class... Args>
        struct LambdaHolder {
            bool m_assigned = false;
            // lambdas don't implement operator= so we 
            // gotta do this wacky union stuff
            union {
                F m_lambda;
            };
            LambdaHolder() {}
            ~LambdaHolder() {
                if (m_assigned) {
                    m_lambda.~F();
                }
            }
            LambdaHolder(F&& func) {
                this->assign(std::forward<F>(func));
            }
            Ret operator()(Args... args) {
                if (m_assigned) {
                    return m_lambda(std::forward<Args>(args)...);
                } else {
                    return Ret();
                }
            }
            void assign(F&& func) {
                if (m_assigned) {
                    m_lambda.~F();
                }
                new (&m_lambda) F(func);
                m_assigned = true;
            }
        };

        // Extract parameters and return type from a lambda
        template<class Func>
        struct ExtractLambda : public ExtractLambda<decltype(&Func::operator())> {};

        template<class C, class R, class... Args>
        struct ExtractLambda<R(C::*)(Args...) const> {
            using Ret = R;
            using Params = std::tuple<Args...>;
        };

        // Class for storing the member function
        template<class Base, class Func, class Args>
        struct InlineMemberFunction;

        template<class Base, class Func, class... Args>
        struct InlineMemberFunction<Base, Func, std::tuple<Args...>> : public Base {
            using Ret = typename ExtractLambda<Func>::Ret;
            using Selector = Ret(Base::*)(Args...);
            using Holder = LambdaHolder<Func, Ret, Args...>;

            static inline Holder s_selector {};
            Ret selector(Args... args) {
                return s_selector(std::forward<Args>(args)...);
            }
            static Selector get(Func&& function) {
                s_selector.assign(std::move(function));
                return static_cast<Selector>(&InlineMemberFunction::selector);
            }
        };
    }

    /**
     * Wrap a lambda into a member function pointer. Useful for creating 
     * callbacks that have to be members of a class without having to deal
     * with all of the boilerplate associated with defining a new class 
     * member function.
     * 
     * Do note that due to implementation problems, captures may have 
     * unexpected side-effects. In practice, lambda member functions with 
     * captures do not work properly in loops. If you assign the same 
     * member lambda to multiple different targets, they will share the 
     * same captured values.
     */
    template<class Base, class Func>
    [[deprecated(
        "Due to too many implementation problems, "
        "makeMemberFunction will be removed in the future."
    )]]
    static auto makeMemberFunction(Func&& function) {
        return InlineMemberFunction<
            Base, Func, typename ExtractLambda<Func>::Params
        >::get(std::move(function));
    }

    /**
     * Create a SEL_MenuHandler out of a lambda with optional captures. Useful 
     * for adding callbacks to CCMenuItemSpriteExtras without needing to add 
     * the callback as a member to a class. Use the GEODE_MENU_SELECTOR class 
     * for even more concise code.
     * 
     * Do note that due to implementation problems, captures may have 
     * unexpected side-effects. In practice, **you should not expect to be able 
     * to pass any more information than you can pass to a normal menu selector 
     * through captures**. If you assign the same member lambda to multiple 
     * different targets, they will share the same captured values.
     */
    template<class Func>
    [[deprecated(
        "Due to too many implementation problems, "
        "makeMenuSelector will be removed in the future."
    )]]
    static cocos2d::SEL_MenuHandler makeMenuSelector(Func&& selector) {
        return reinterpret_cast<cocos2d::SEL_MenuHandler>(
            makeMemberFunction<cocos2d::CCObject, Func>(std::move(selector))
        );
    }

    #define GEODE_MENU_SELECTOR(senderArg, ...) \
        makeMenuSelector([this](senderArg) { __VA_ARGS__; })
}
