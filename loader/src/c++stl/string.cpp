#include <Geode/c++stl/gdstdlib.hpp>
#include "string-impl.hpp"
#include <compare>
#include <stdexcept>

template <class Type>
Type& intoMutRef(const Type& x) {
    return const_cast<Type&>(x);
}

using geode::stl::StringImpl;

#define implFor(x) StringImpl{intoMutRef(x.m_data)}
#define impl implFor((*this))

namespace gd {
#if !defined(GEODE_IS_MACOS) && !defined(GEODE_IS_IOS) && !defined(GEODE_IS_WINDOWS)
    string::string() {
        impl.setEmpty();
    }

    string::string(string const& str) {
        impl.setStorage(intoMutRef(str.m_data));
    }

    string::string(string&& other) noexcept {
        impl.swapStorage(other.m_data);
        implFor(other).setEmpty();
    }

    string::string(char const* str) {
        impl.setStorage(str);
    }

    string::string(char const* str, size_t size) {
        impl.setStorage(std::string_view(str, size));
    }

    string::string(size_t count, char c) {
        impl.setSize(count, c);
    }

    string::string(std::string const& str) {
        impl.setStorage(str);
    }

    string::string(std::initializer_list<char> ilist) {
        impl.setStorage(std::string_view(ilist.begin(), ilist.size()));
    }

    string::string(string const& other, size_t pos) {
        if (pos > other.size()) {
            throw std::out_of_range("gd::string::string");
        }
        impl.setStorage(std::string_view(other.data() + pos, other.size() - pos));
    }

    string::string(string const& other, size_t pos, size_t count) {
        if (pos > other.size()) {
            throw std::out_of_range("gd::string::string");
        }
        impl.setStorage(std::string_view(other.data() + pos, std::min(count, other.size() - pos)));
    }

    string::string(string&& other, size_t pos) {
        if (pos > other.size()) {
            throw std::out_of_range("gd::string::string");
        }
        other.erase(0, pos);
        impl.swapStorage(other.m_data);
        implFor(other).setEmpty();
    }

    string::string(string&& other, size_t pos, size_t count) {
        if (pos > other.size()) {
            throw std::out_of_range("gd::string::string");
        }
        other.erase(0, pos);
        other.erase(std::min(count, other.size() - pos));
        impl.swapStorage(other.m_data);
        implFor(other).setEmpty();
    }

    string::~string() {
        this->clear();
    }

    string& string::assign(string const& other) {
        impl.setStorage(intoMutRef(other.m_data));
        return *this;
    }

    string& string::assign(char const* other, size_t count) {
        impl.setStorage(std::string_view(other, count));
        return *this;
    }

    string& string::operator=(string const& other) {
        // check if the strings share the same storage
        if (this->m_data.m_data != other.m_data.m_data) {
            impl.setStorage(intoMutRef(other.m_data));
        }
        return *this;
    }
    string& string::operator=(string&& other) noexcept {
        impl.swapStorage(other.m_data);
        implFor(other).setEmpty();
        return *this;
    }
    string& string::operator=(char const* other) {
        impl.setStorage(other);
        return *this;
    }

    void string::clear() {
        impl.setEmpty();
    }

    void string::swap(string& other) {
        impl.swapStorage(other.m_data);
    }

    void string::reserve(size_t capacity) {
        impl.setCapacity(impl.getSize() + capacity);
    }

    void string::resize(size_t size, char fill) {
        impl.setSize(size, fill);
    }

    void string::shrink_to_fit() {
        impl.setCapacity(this->size());
    }

    string& string::append(char const* other, size_t count) {
        impl.append(std::string_view(other, count));
        return *this;
    }

    string& string::append(size_t count, char c) {
        impl.append(count, c);
        return *this;
    }

    string& string::insert(size_t pos, char const* other, size_t count) {
        if (pos > this->size()) {
            throw std::out_of_range("gd::string::insert");
        }
        impl.insert(pos, std::string_view(other, count));
        return *this;
    }

    string::iterator string::insert(const_iterator pos, size_t count, char c) {
        size_t index = pos - this->cbegin();
        impl.insert(index, count, c);
        return this->begin() + index;
    }

    string::iterator string::insert(const_iterator pos, std::initializer_list<char> ilist) {
        size_t index = pos - this->cbegin();
        impl.insert(index, std::string_view(ilist.begin(), ilist.size()));
        return this->begin() + index;
    }

    string& string::insert(size_t pos, size_t count, char c) {
        if (pos > this->size()) {
            throw std::out_of_range("gd::string::insert");
        }
        impl.insert(pos, count, c);
        return *this;
    }

    string& string::erase(size_t pos, size_t count) {
        if (pos > this->size()) {
            throw std::out_of_range("gd::string::erase");
        }
        impl.erase(pos, count);
        return *this;
    }

    string::iterator string::erase(const_iterator first, const_iterator last) {
        size_t index = first - this->cbegin();
        impl.erase(index, last - first);
        return this->begin() + index;
    }

    string& string::replace(size_t pos, size_t count, char const* other, size_t count2) {
        if (pos > this->size()) {
            throw std::out_of_range("gd::string::replace");
        }
        impl.replace(pos, count, std::string_view(other, count2));
        return *this;
    }

    string& string::replace(const_iterator first, const_iterator last, char const* other, size_t count2) {
        size_t index = first - this->cbegin();
        impl.replace(index, last - first, std::string_view(other, count2));
        return *this;
    }

    string& string::replace(size_t pos, size_t count, size_t count2, char c) {
        if (pos > this->size()) {
            throw std::out_of_range("gd::string::replace");
        }
        impl.replace(pos, count, count2, c);
        return *this;
    }

    string& string::replace(const_iterator first, const_iterator last, size_t count2, char c) {
        size_t index = first - this->cbegin();
        impl.replace(index, last - first, count2, c);
        return *this;
    }

    char& string::at(size_t pos) {
        if (pos >= this->size())
            throw std::out_of_range("gd::string::at");
        return impl.getStorage()[pos];
    }
    char const& string::at(size_t pos) const {
        return const_cast<string*>(this)->at(pos);
    }

    char& string::operator[](size_t pos) { return impl.getStorage()[pos]; }
    char const& string::operator[](size_t pos) const { return impl.getStorage()[pos]; }

    char* string::data() { return impl.getStorage(); }
    char const* string::data() const { return impl.getStorage(); }
    char const* string::c_str() const { return this->data(); }

    size_t string::size() const { return impl.getSize(); }
    size_t string::capacity() const { return impl.getCapacity(); }
    bool string::empty() const { return this->size() == 0; }

    string::iterator makeIter(char* ptr) {
        return *reinterpret_cast<string::iterator*>(&ptr);
    }

    string::const_iterator makeIter(char const* ptr) {
        return *reinterpret_cast<string::const_iterator*>(&ptr);
    }

    string::iterator string::begin() { return makeIter(this->data()); }
    string::const_iterator string::begin() const { return makeIter(this->data()); }
    string::iterator string::end() { return makeIter(this->data() + this->size()); }
    string::const_iterator string::end() const { return makeIter(this->data() + this->size()); }

    bool string::operator==(string const& other) const {
        return std::string_view(*this) == std::string_view(other);
    }
	bool string::operator==(std::string_view other) const {
        return std::string_view(*this) == other;
    }

    std::strong_ordering string::operator<=>(std::string_view other) const {
        return static_cast<std::strong_ordering>(std::string_view(*this).compare(other) <=> 0);
    }

    std::strong_ordering string::operator<=>(string const& other) const {
        return static_cast<std::strong_ordering>(std::string_view(*this).compare(std::string_view(other)) <=> 0);
    }

    string::operator std::string() const {
        return std::string(this->data(), this->size());
    }
    string::operator std::string_view() const {
        return std::string_view(this->data(), this->size());
    }
#endif
}

#undef impl
#undef implFor