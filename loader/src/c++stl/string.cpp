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
#if !defined(GEODE_IS_MACOS) && !defined(GEODE_IS_WINDOWS)
    string::string() {
        impl.setEmpty();
    }

    string::string(string const& str) {
        impl.setStorage(str);
    }

    // string::string(string&& other) {
    //     // TODO: do this better :-)
    //     impl.setStorage(other);
    //     implFor(other).free();
    //     implFor(other).setEmpty();
    // }

    string::string(char const* str) {
        impl.setStorage(str);
    }

    string::string(std::string const& str) {
        impl.setStorage(str);
    }

    string::~string() {
        this->clear();
    }

    string& string::operator=(string const& other) {
        if (this != &other) {
            impl.free();
            impl.setStorage(other);
        }
        return *this;
    }
    string& string::operator=(string&& other) {
        // TODO: do this better :-)
        impl.free();
        impl.setStorage(other);
        implFor(other).free();
        implFor(other).setEmpty();
        return *this;
    }
    string& string::operator=(char const* other) {
        impl.free();
        impl.setStorage(other);
        return *this;
    }
    string& string::operator=(std::string const& other) {
        impl.free();
        impl.setStorage(other);
        return *this;
    }

    void string::clear() {
        impl.free();
        impl.setEmpty();
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

    bool string::operator==(string const& other) const {
        return std::string_view(*this) == std::string_view(other);
    }
	bool string::operator==(std::string_view const other) const {
        return std::string_view(*this) == other;
    }

    std::strong_ordering string::operator<=>(std::string_view const other) const {
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
