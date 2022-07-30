#pragma once

#include <Geode/DefaultInclude.hpp>
#include <cocos2d.h>
#include <functional>
#include <type_traits>

namespace geode::cocos {
    /**
     * Get child at index. Checks bounds. A negative 
     * index will get the child starting from the end
     * @returns Child at index cast to the given type, 
     * or nullptr if index exceeds bounds
     */
    template<class T = cocos2d::CCNode*>
    static T getChild(cocos2d::CCNode* x, int i) {
        // start from end for negative index
        if (i < 0) i = x->getChildrenCount() + i;
        // check if backwards index is out of bounds
        if (i < 0) return nullptr;
        // check if forwards index is out of bounds
        if (static_cast<int>(x->getChildrenCount()) <= i) return nullptr;
        return reinterpret_cast<T>(x->getChildren()->objectAtIndex(i));
    }

    /**
     * Get nth child that is a given type. Checks bounds. 
     * @returns Child at index cast to the given type, 
     * or nullptr if index exceeds bounds
     */
    template<class Type = cocos2d::CCNode*>
    static Type getChildOfType(cocos2d::CCNode* node, size_t index) {
    	auto indexCounter = static_cast<size_t>(0);

		for (size_t i = 0; i < node->getChildrenCount(); ++i) {
			auto obj = cast::typeinfo_cast<Type>(
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
    struct GEODE_DLL CCArrayIterator {
    public:
        CCArrayIterator(T* p) : m_ptr(p) {}
        T* m_ptr;

        T& operator*() { return *m_ptr; }
        T* operator->() { return m_ptr; }

        auto& operator++() {
            ++m_ptr;
            return *this;
        }

        friend bool operator== (const CCArrayIterator<T>& a, const CCArrayIterator<T>& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const CCArrayIterator<T>& a, const CCArrayIterator<T>& b) { return a.m_ptr != b.m_ptr; };   
    };

    template <typename _Type>
    class CCArrayExt {
     protected:
        cocos2d::CCArray* m_arr;
        using T = std::remove_pointer_t<_Type>;
     public:
        CCArrayExt() : m_arr(cocos2d::CCArray::create()) {
            m_arr->retain();
        }
        CCArrayExt(cocos2d::CCArray* arr) : m_arr(arr) {
            m_arr->retain();
        }
        CCArrayExt(CCArrayExt const& a) : m_arr(a.m_arr) {
            m_arr->retain();
        }
        CCArrayExt(CCArrayExt&& a) : m_arr(a.m_arr) {
            a.m_arr = nullptr;
        }
        ~CCArrayExt() {
            if (m_arr)
                m_arr->release();
        }

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

    template <typename R, typename ...Args>
    class SelectorWrapperImpl : public cocos2d::CCObject {
     protected:
        std::function<R(Args...)> m_inner;
     public:
        static SelectorWrapperImpl<R, Args...>* create(std::function<R(Args...)> fn) {
            auto ret = new SelectorWrapperImpl<R, Args...>();
            ret->m_inner = fn;
            ret->autorelease();
            return ret;
        }
        
        R invoke(Args... args) {
            return m_inner(args...);
        }
    };

    template <typename R, typename ...Args>
    class SelectorWrapper {
     protected:
        using Target = SelectorWrapperImpl<R, Args...>;
        bool m_tied;
        Target* m_impl;
     public:
        SelectorWrapper(std::function<R(Args...)> fn) {
            m_impl = Target::create(fn);
            m_impl->retain();
        }

        ~SelectorWrapper() {
            if (!m_tied)
                m_impl->release();
        }

        Target* target() {
            return m_impl;
        }

        auto selector() {
            return reinterpret_cast<R(cocos2d::CCObject::*)(Args...)>(&Target::invoke);
        }

        SelectorWrapper<R, Args...>& leak() {
            m_impl->retain();
            return *this;
        }

        SelectorWrapper<R, Args...> tieToNode(cocos2d::CCNode* node) {
            if (!m_tied) {
                node->addChild(m_impl);
                m_impl->release();
                m_tied = true;
            }

            return *this;
        }
    };

    template <typename F>
    auto selectorFromFn(std::function<F> fn) {
        return SelectorWrapper(fn);
    }
}
