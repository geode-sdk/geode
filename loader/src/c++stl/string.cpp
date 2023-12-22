#include <Geode/c++stl/gdstdlib.hpp>
#include "string-adapter.hpp"

template <class Type>
Type& intoMutRef(const Type& x) {
    return const_cast<Type&>(x);
}

using geode::stl::StringImplAdapter;

#define getAdap(x) StringImplAdapter(intoMutRef(x))
#define adap getAdap(m_impl)

namespace gd {
    string::string() {
        adap.setEmpty();
    }

    string::string(string const& str) {
        adap.setStorage(str);
    }

    string::string(string&& other) {
        // TODO: do this better :-)
        adap.setStorage(other);
        getAdap(other.m_impl).free();
        getAdap(other.m_impl).setEmpty();
    }

    string::string(char const* str) {
        adap.setStorage(str);
    }

    string::string(std::string const& str) {
        adap.setStorage(str);
    }

    string::~string() {
        adap.free();
        adap.setEmpty();
    }

    string& string::operator=(string const& other) {
        if (this != &other) {
            adap.free();
            adap.setStorage(other);
        }
        return *this;
    }
    string& string::operator=(string&& other) {
        // TODO: do this better :-)
        adap.free();
        adap.setStorage(other);
        getAdap(other.m_impl).free();
        getAdap(other.m_impl).setEmpty();
        return *this;
    }
    string& string::operator=(char const* other) {
        adap.free();
        adap.setStorage(other);
        return *this;
    }
    string& string::operator=(std::string const& other) {
        adap.free();
        adap.setStorage(other);
        return *this;
    }

    void string::clear() {
        adap.free();
        adap.setEmpty();
    }
    
    char& string::at(size_t pos) {
        if (pos >= this->size())
            throw std::out_of_range("gd::string::at");
        return adap.getStorage()[pos];
    }
    char const& string::at(size_t pos) const {
        return const_cast<string*>(this)->at(pos);
    }

    char& string::operator[](size_t pos) { return adap.getStorage()[pos]; }
    char const& string::operator[](size_t pos) const { return adap.getStorage()[pos]; }

    char* string::data() { return adap.getStorage(); }
    char const* string::data() const { return adap.getStorage(); }
    char const* string::c_str() const { return this->data(); }

    size_t string::size() const { return adap.getSize(); }
    size_t string::capacity() const { return adap.getCapacity(); }
    bool string::empty() const { return this->size() == 0; }

    std::strong_ordering string::operator<=>(const std::string_view other) const {
        return std::string_view(*this) <=> other;
    }

    string::operator std::string() const {
        return std::string(this->data(), this->size());
    }
    string::operator std::string_view() const {
        return std::string_view(this->data(), this->size());
    }
}