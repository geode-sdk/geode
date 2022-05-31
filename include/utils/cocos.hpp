#pragma once

#include <Geode.hpp>

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

    template <typename T>
    class CCArrayExt {
     protected:
        cocos2d::CCArray* m_arr;
     public:
        CCArrayExt() : m_arr(CCArray::create()) {
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
            return CCArrayIterator<T*>(reinterpret_cast<T*>(m_arr->data->arr));
        }
        auto end() {
            return CCArrayIterator<T*>(reinterpret_cast<T*>(m_arr->data->arr) + m_arr->count());
        }
        auto size() const {
            return m_arr->count();
        }
        T operator[](size_t index) {
            return reinterpret_cast<T>(m_arr->objectAtIndex(index));
        }
        void push_back(T item) {
            m_arr->addObject(item);
        }
        T pop_back() { 
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

        CCDictEntry(K key, CCDictionary* dict) : m_key(key), m_dict(dict) {}

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
        CCDictionaryExt() : m_dict(CCDictionary::create()) {
            m_dict.retain();
        }   
        CCDictionaryExt(cocos2d::CCDictionary* dict) : m_dict(dict) {
            m_dict.retain();
        }
        CCDictionaryExt(CCDictionaryExt const& d) : m_dict(d.m_dict) {
            m_dict.retain();
        }
        CCDictionaryExt(CCDictionaryExt&& d) : m_dict(d.m_dict) {
            d.m_dict = nullptr;
        }
        ~CCDictionaryExt() {
            if (m_dict)
                m_dict->release();
        }

        auto begin() {
            return CCDictIterator<K, T*>(m_dict->m_pElements);
        }
        // do not use this
        auto end() {
            return nullptr;
        }
        auto size() { return m_dict->count(); }
        auto operator[](K key) {
            auto ret = reinterpret_cast<T*>(m_dict->objectForKey(key));
            if (ret)
                return ret;

            m_dict->setObject(CCObject::create(), key);

            return CCDictEntry<K, T*>(key, m_dict);
        }
        size_t count(K key) {
            return m_dict->addKeys(key)->count();
        }
    };
}
