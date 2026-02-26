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

    string::string(string&& other) {
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
        impl.setStorage(other);
        return *this;
    }

    string& string::assign(string&& other) {
        impl.swapStorage(other.m_data);
        implFor(other).setEmpty();
        return *this;
    }

    string& string::assign(char const* other) {
        impl.setStorage(other);
        return *this;
    }

    string& string::assign(char const* other, size_t count) {
        impl.setStorage(std::string_view(other, count));
        return *this;
    }

    string& string::assign(string const& other, size_t pos, size_t count) {
        if (pos > other.size()) {
            throw std::out_of_range("gd::string::assign");
        }

        impl.setStorage(std::string_view(other.data() + pos, std::min(count, other.size() - pos)));
        return *this;
    }

    string& string::assign(std::initializer_list<char> ilist) {
        impl.setStorage(std::string_view(ilist.begin(), ilist.size()));
        return *this;
    }

    string& string::assign(size_t count, char c) {
        impl.setSize(count, c);
        return *this;
    }

    string& string::operator=(string const& other) {
        // check if the strings share the same storage
        if (this->m_data.m_data != other.m_data.m_data) {
            impl.setStorage(other);
        }
        return *this;
    }
    string& string::operator=(string&& other) {
        impl.swapStorage(other.m_data);
        implFor(other).setEmpty();
        return *this;
    }
    string& string::operator=(char const* other) {
        impl.setStorage(other);
        return *this;
    }
    string& string::operator=(char other) {
        impl.setSize(1, other);
        return *this;
    }
    string& string::operator=(std::initializer_list<char> ilist) {
        impl.setStorage(std::string_view(ilist.begin(), ilist.size()));
        return *this;
    }
    string& string::operator=(std::string const& other) {
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

    void string::push_back(char c) {
        impl.append(1, c);
    }

    void string::pop_back() {
        impl.setSize(this->size() - 1, 0);
    }

    size_t string::copy(char* buffer, size_t count, size_t pos) const {
        if (pos > this->size()) {
            throw std::out_of_range("gd::string::copy");
        }
        size_t toCopy = std::min(count, this->size() - pos);
        std::memcpy(buffer, this->data() + pos, toCopy);
        return toCopy;
    }

    void string::shrink_to_fit() {
        impl.setCapacity(this->size());
    }

    string& string::append(char const* other) {
        impl.append(other);
        return *this;
    }

    string& string::append(char const* other, size_t count) {
        impl.append(std::string_view(other, count));
        return *this;
    }

    string& string::append(string const& other) {
        impl.append(other);
        return *this;
    }

    string& string::append(string const& other, size_t pos, size_t count) {
        if (pos > other.size()) {
            throw std::out_of_range("gd::string::append");
        }
        impl.append(std::string_view(other.data() + pos, std::min(count, other.size() - pos)));
        return *this;
    }

    string& string::append(std::initializer_list<char> ilist) {
        impl.append(std::string_view(ilist.begin(), ilist.size()));
        return *this;
    }

    string& string::append(size_t count, char c) {
        impl.append(count, c);
        return *this;
    }

    string& string::insert(size_t pos, char const* other) {
        if (pos > this->size()) {
            throw std::out_of_range("gd::string::insert");
        }
        impl.insert(pos, other);
        return *this;
    }

    string& string::insert(size_t pos, char const* other, size_t count) {
        if (pos > this->size()) {
            throw std::out_of_range("gd::string::insert");
        }
        impl.insert(pos, std::string_view(other, count));
        return *this;
    }

    string& string::insert(size_t pos, string const& other) {
        if (pos > this->size()) {
            throw std::out_of_range("gd::string::insert");
        }
        impl.insert(pos, other);
        return *this;
    }

    string& string::insert(size_t pos, string const& other, size_t pos2, size_t count) {
        if (pos > this->size() || pos2 > other.size()) {
            throw std::out_of_range("gd::string::insert");
        }
        impl.insert(pos, std::string_view(other.data() + pos2, std::min(count, other.size() - pos2)));
        return *this;
    }

    string::iterator string::insert(const_iterator pos, char c) {
        size_t index = pos - this->begin();
        impl.insert(index, 1, c);
        return this->begin() + index;
    }

    string::iterator string::insert(const_iterator pos, size_t count, char c) {
        size_t index = pos - this->begin();
        impl.insert(index, count, c);
        return this->begin() + index;
    }

    string::iterator string::insert(const_iterator pos, std::initializer_list<char> ilist) {
        size_t index = pos - this->begin();
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

    string::iterator string::erase(const_iterator pos) {
        size_t index = pos - this->begin();
        impl.erase(index, 1);
        return this->begin() + index;
    }

    string::iterator string::erase(const_iterator first, const_iterator last) {
        size_t index = first - this->begin();
        impl.erase(index, last - first);
        return this->begin() + index;
    }

    string& string::replace(size_t pos, size_t count, string const& other) {
        if (pos > this->size()) {
            throw std::out_of_range("gd::string::replace");
        }
        impl.replace(pos, count, other);
        return *this;
    }

    string& string::replace(const_iterator first, const_iterator last, string const& other) {
        size_t index = first - this->begin();
        impl.replace(index, last - first, other);
        return *this;
    }

    string& string::replace(size_t pos, size_t count, string const& other, size_t pos2, size_t count2) {
        if (pos > this->size() || pos2 > other.size()) {
            throw std::out_of_range("gd::string::replace");
        }
        impl.replace(pos, count, std::string_view(other.data() + pos2, std::min(count2, other.size() - pos2)));
        return *this;
    }

    string& string::replace(size_t pos, size_t count, char const* other) {
        if (pos > this->size()) {
            throw std::out_of_range("gd::string::replace");
        }
        impl.replace(pos, count, other);
        return *this;
    }

    string& string::replace(size_t pos, size_t count, char const* other, size_t count2) {
        if (pos > this->size()) {
            throw std::out_of_range("gd::string::replace");
        }
        impl.replace(pos, count, std::string_view(other, count2));
        return *this;
    }

    string& string::replace(const_iterator first, const_iterator last, char const* other) {
        size_t index = first - this->begin();
        impl.replace(index, last - first, other);
        return *this;
    }

    string& string::replace(const_iterator first, const_iterator last, char const* other, size_t count2) {
        size_t index = first - this->begin();
        impl.replace(index, last - first, std::string_view(other, count2));
        return *this;
    }

    string& string::replace(const_iterator first, const_iterator last, std::initializer_list<char> ilist) {
        size_t index = first - this->begin();
        impl.replace(index, last - first, std::string_view(ilist.begin(), ilist.size()));
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
        size_t index = first - this->begin();
        impl.replace(index, last - first, count2, c);
        return *this;
    }

    string& string::operator+=(string const& other) {
        impl.append(other);
        return *this;
    }

    string& string::operator+=(char const* other) {
        impl.append(other);
        return *this;
    }

    string& string::operator+=(char c) {
        impl.append(1, c);
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

    char& string::front() { return impl.getStorage()[0]; }
    char const& string::front() const { return impl.getStorage()[0]; }
    char& string::back() { return impl.getStorage()[this->size() - 1]; }
    char const& string::back() const { return impl.getStorage()[this->size() - 1]; }

    char* string::data() { return impl.getStorage(); }
    char const* string::data() const { return impl.getStorage(); }
    char const* string::c_str() const { return this->data(); }

    size_t string::size() const { return impl.getSize(); }
    size_t string::length() const { return this->size(); }
    size_t string::capacity() const { return impl.getCapacity(); }
    bool string::empty() const { return this->size() == 0; }
    size_t string::max_size() const {
        return (npos - sizeof(geode::stl::StringData::Internal) - 1) / 4;
    }

    string::iterator makeIter(char* ptr) {
        return *reinterpret_cast<string::iterator*>(&ptr);
    }

    string::const_iterator makeIter(char const* ptr) {
        return *reinterpret_cast<string::const_iterator*>(&ptr);
    }

    string::iterator string::begin() { return makeIter(this->data()); }
    string::const_iterator string::begin() const { return makeIter(this->data()); }
    string::const_iterator string::cbegin() const { return this->begin(); }
    string::iterator string::end() { return makeIter(this->data() + this->size()); }
    string::const_iterator string::end() const { return makeIter(this->data() + this->size()); }
    string::const_iterator string::cend() const { return this->end(); }

    string::reverse_iterator string::rbegin() { return string::reverse_iterator(this->end()); }
    string::const_reverse_iterator string::rbegin() const { return string::const_reverse_iterator(this->end()); }
    string::const_reverse_iterator string::crbegin() const { return this->rbegin(); }
    string::reverse_iterator string::rend() { return string::reverse_iterator(this->begin()); }
    string::const_reverse_iterator string::rend() const { return string::const_reverse_iterator(this->begin()); }
    string::const_reverse_iterator string::crend() const { return this->rend(); }

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