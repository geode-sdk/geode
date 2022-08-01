#pragma once

#include <cocos2d.h>

namespace geode {
    /**
     * A smart pointer to a managed CCObject-deriving class. Retains shared 
     * ownership over the managed instance. Releases the object when the Ref 
     * is destroyed, or assigned another object or nullptr.
     * 
     * Use-cases include, for example, non-CCNode class members, or nodes that 
     * are not always in the scene tree.
     * 
     * @example class MyNode : public CCNode {
     * protected:
     *      // no need to manually call retain or 
     *      // release on this array; Ref manages it 
     *      // for you :3
     *      Ref<CCArray> m_list = CCArray::create();
     * };
     */
    template<class T>
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
    };
}
