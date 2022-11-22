#pragma once

#include <Geode/binding/GDString.hpp>
#include <string>
#include <stdexcept>
#include <utility>
#include <map>
#include <vector>

namespace gd {
    struct InternalString {
        union {
            char m_storage[16];
            char* m_pointer;
        };

        size_t m_length;
        size_t m_capacity;
    };

    class string : GDString {
    private:
        InternalString m_data;

        char* get_data() {
            if (m_data.m_capacity < 16) return m_data.m_storage;
            return m_data.m_pointer;
        }

        const char* get_data() const {
            if (m_data.m_capacity < 16) return m_data.m_storage;
            return m_data.m_pointer;
        }

    public:
        template <class... Params>
        string(Params&&... params) {
            m_data.m_pointer = 0;
            m_data.m_length = 0;
            m_data.m_capacity = 15;

            auto val = std::string(std::forward<Params>(params)...);
            (void)this->winAssign(val.c_str(), val.size());
        }

        template <class Param>
        string& operator=(Params&& param) {
            std::string val;
            val = std::forward<Param>(param);
            (void)this->winAssign(val.c_str(), val.size());
            return *this;
        }

        ~string() {
            (void)this->winDtor();
        }

        char& at(size_t pos) {
            if (pos >= m_data.m_length) throw std::out_of_range("gd::string::at");
            return (*this)[pos];
        }

        char const& at(size_t pos) const {
            if (pos >= m_data.m_length) throw std::out_of_range("gd::string::at");
            return (*this)[pos];
        }

        char& operator[](size_t pos) {
            return this->get_data()[pos];
        }

        char const& operator[](size_t pos) const {
            return this->get_data()[pos];
        }

        char* data() {
            return this->get_data();
        }

        char const* data() const {
            return this->get_data();
        }

        char const* c_str() const {
            return this->get_data();
        }

        size_t size() const {
            return m_data.m_length;
        }

        operator std::string() const {
            return std::string(this->c_str(), this->size());
        }
    };

    template <class T>
    using vector = std::vector<T>;

    template <class K, class V>
    using map = std::map<K, V>;
}
