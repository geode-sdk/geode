#pragma once

#include <Geode/platform/platform.hpp>
#include <cstdint>
#include <array>
#include <string_view>
#include <string>
#include <compare>

namespace geode::stl {
	class StringImpl;

	struct StringData;

#if defined(GEODE_IS_WINDOWS)
	struct StringData {
		union {
			std::array<char, 16> m_smallStorage;
			char* m_bigStorage;
		};

		size_t m_size;
		size_t m_capacity;
	};
#elif defined(GEODE_IS_MACOS) || defined(GEODE_IS_ANDROID)
	struct StringData {
		struct Internal {
			size_t m_size;
			size_t m_capacity;
			int m_refcount;
		};
		Internal* m_data = nullptr;
	};
#elif defined(GEODE_IS_IOS)
	struct StringData {
		struct Short {
			uint8_t sizex2;
			std::array<char, 23> shortStorage;
		};

		struct Long {
			size_t capacitym1;
			size_t size;
			char* longStorage;
		};

		union {
			Short m_short;
			Long m_long;
		};
	};
#endif
}

namespace gd {
#if defined(GEODE_IS_MACOS) || defined(GEODE_IS_WINDOWS) || defined(GEODE_IS_IOS)
	// rob uses libc++ now! this will prob work fine
	using string = std::string;

#else

	class GEODE_DLL string {
		geode::stl::StringData m_data;
		friend geode::stl::StringImpl;
	public:
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using value_type = char;
		using reference = char&;
		using const_reference = char const&;
		using pointer = char*;
		using const_pointer = char const*;
		using iterator = std::string::iterator;
		using const_iterator = std::string::const_iterator;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		static const size_t npos = -1;

		string();
		string(string const&);
		string(string&&);
		string(char const*);
		string(char const*, size_t);
		string(size_t, char);
		string(std::string const&);
		string(std::initializer_list<char>);
		string(string const&, size_t);
		string(string const&, size_t, size_t);
		string(string&&, size_t);
		string(string&&, size_t, size_t);

		template <class T>
		requires std::convertible_to<T, std::string_view>
		string(T const& other) {
			this->assign(std::string_view(other));
		}

		template <class T>
		requires std::convertible_to<T, std::string_view>
		string(T const& other, size_t pos, size_t count) {
			this->assign(std::string_view(other), pos, count);
		}

		template <class It>
		string(It first, It last) : string(std::to_address(first), std::distance(first, last)) {}

		template <class Range>
		string(std::from_range_t, Range&& range) : string(std::ranges::begin(range), std::ranges::end(range)) {}

		~string();

		string& assign(string const& other);
		string& assign(string&& other);
		string& assign(char const* other);
		string& assign(char const* other, size_t count);
		string& assign(string const& other, size_t pos, size_t count = npos);
		string& assign(std::initializer_list<char> ilist);
		string& assign(size_t count, char c);

		template <class T>
		requires std::convertible_to<T, std::string_view>
		string& assign(T const& other) {
			std::string_view view(other);
			return this->assign(view.data(), view.size());
		}

		template <class T>
		requires std::convertible_to<T, std::string_view>
		string& assign(T const& other, size_t pos, size_t count = npos) {
			std::string_view view(other);
			if (pos > view.size()) {
				throw std::out_of_range("gd::string::assign");
			}
			return this->assign(view.data() + pos, std::min(count, view.size() - pos));
		}

		template <class It>
		string& assign(It first, It last) {
			return this->assign(std::to_address(first), std::distance(first, last));
		}

		template <class Range>
		string& assign_range(Range&& range) {
			return this->assign(std::ranges::begin(range), std::ranges::end(range));
		}

		string& operator=(string const&);
		string& operator=(string&&);
		string& operator=(char const*);
		string& operator=(char);
		string& operator=(std::initializer_list<char>);
		string& operator=(std::string const&);

		template <class T>
		requires std::convertible_to<T, std::string_view>
		string& operator=(T const& other) {
			return this->assign(std::string_view(other));
		}

		void clear();
		void swap(string& other);

		void reserve(size_t capacity);
		void resize(size_t size, char fill = '\0');
		void push_back(char c);
		void pop_back();
		size_t copy(char* buffer, size_t count, size_t pos = 0) const;
		void shrink_to_fit();

		template <class Operation>
		void resize_and_overwrite(size_t newSize, Operation op) {
			this->resize(newSize);
			this->erase(op(this->data(), newSize));
		}

		string& append(char const* other);
		string& append(char const* other, size_t count);
		string& append(string const& other);
		string& append(string const& other, size_t pos, size_t count = npos);
		string& append(std::initializer_list<char> ilist);
		string& append(size_t count, char c);

		template <class T>
		requires std::convertible_to<T, std::string_view>
		string& append(T const& other) {
			std::string_view view(other);
			return this->append(view.data(), view.size());
		}

		template <class T>
		requires std::convertible_to<T, std::string_view>
		string& append(T const& other, size_t pos, size_t count = npos) {
			std::string_view view(other);
			if (pos > view.size()) {
				throw std::out_of_range("gd::string::append");
			}
			return this->append(view.data() + pos, std::min(count, view.size() - pos));
		}

		template <class It>
		string& append(It first, It last) {
			return this->append(std::to_address(first), std::distance(first, last));
		}

		template <class Range>
		string& append_range(Range&& range) {
			return this->append(std::ranges::begin(range), std::ranges::end(range));
		}

		string& operator+=(string const&);
		string& operator+=(char const*);
		string& operator+=(char);
		string& operator+=(std::initializer_list<char>);

		template <class T>
		requires std::convertible_to<T, std::string_view>
		string& operator+=(T const& other) {
			return this->append(std::string_view(other));
		}

		string& insert(size_t pos, char const* other);
		string& insert(size_t pos, char const* other, size_t count);
		string& insert(size_t pos, string const& other);
		string& insert(size_t pos, string const& other, size_t pos2, size_t count = npos);
		iterator insert(const_iterator pos, char c);
		iterator insert(const_iterator pos, size_t count, char c);
		iterator insert(const_iterator pos, std::initializer_list<char> ilist);
		string& insert(size_t pos, size_t count, char c);

		template <class It>
		iterator insert(const_iterator pos, It first, It last) {
			size_t index = pos - this->begin();
			this->insert(index, std::to_address(first), std::distance(first, last));
			return this->begin() + index;
		}

		template <class T>
		requires std::convertible_to<T, std::string_view>
		string& insert(size_t pos, T const& other) {
			std::string_view view(other);
			return this->insert(pos, view.data(), view.size());
		}

		template <class T>
		requires std::convertible_to<T, std::string_view>
		string& insert(size_t pos, T const& other, size_t pos2, size_t count = npos) {
			std::string_view view(other);
			if (pos2 > view.size()) {
				throw std::out_of_range("gd::string::insert");
			}
			return this->insert(pos, view.data() + pos2, std::min(count, view.size() - pos2));
		}

		string& erase(size_t pos = 0, size_t count = npos);
		iterator erase(const_iterator pos);
		iterator erase(const_iterator first, const_iterator last);

		string& replace(size_t pos, size_t count, string const& other);
		string& replace(const_iterator first, const_iterator last, string const& other);
		string& replace(size_t pos, size_t count, string const& other, size_t pos2, size_t count2 = npos);
		string& replace(size_t pos, size_t count, char const* other);
		string& replace(size_t pos, size_t count, char const* other, size_t count2);
		string& replace(const_iterator first, const_iterator last, char const* other);
		string& replace(const_iterator first, const_iterator last, char const* other, size_t count2);
		string& replace(const_iterator first, const_iterator last, std::initializer_list<char> ilist);
		string& replace(size_t pos, size_t count, size_t count2, char c);
		string& replace(const_iterator first, const_iterator last, size_t count2, char c);

		template <class It>
		string& replace(const_iterator first, const_iterator last, It first2, It last2) {
			this->replace(first, last, std::to_address(first2), std::distance(first2, last2));
			return *this;
		}

		template <class T>
		requires std::convertible_to<T, std::string_view>
		string& replace(size_t pos, size_t count, T const& other) {
			if (pos > this->size()) {
				throw std::out_of_range("gd::string::replace");
			}
			std::string_view view(other);
			return this->replace(pos, count, view.data(), view.size());
		}

		template <class T>
		requires std::convertible_to<T, std::string_view>
		string& replace(const_iterator first, const_iterator last, T const& other) {
			std::string_view view(other);
			return this->replace(first, last, view.data(), view.size());
		}

		template <class T>
		requires std::convertible_to<T, std::string_view>
		string& replace(size_t pos, size_t count, T const& other, size_t pos2, size_t count2 = npos) {
			if (pos > this->size()) {
				throw std::out_of_range("gd::string::replace");
			}
			std::string_view view(other);
			if (pos2 > view.size()) {
				throw std::out_of_range("gd::string::replace");
			}
			return this->replace(pos, count, view.data() + pos2, std::min(count2, view.size() - pos2));
		}

		template <class Range>
		string& replace_with_range(const_iterator first, const_iterator last, Range&& range) {
			return this->replace(first, last, std::ranges::begin(range), std::ranges::end(range));
		}

		string substr(size_t pos = 0, size_t count = npos) const& {
			return string(*this, pos, count);
		}

		string substr(size_t pos = 0, size_t count = npos) && {
			return string(std::move(*this), pos, count);
		}

		char& at(size_t pos);
		char const& at(size_t pos) const;

		char& operator[](size_t pos);
		char const& operator[](size_t pos) const;

		char& front();
		char const& front() const;
		char& back();
		char const& back() const;

		char* data();
		char const* data() const;
		char const* c_str() const;

		size_t size() const;
		size_t length() const;
		size_t capacity() const;
		bool empty() const;
		size_t max_size() const;

		iterator begin();
		const_iterator begin() const;
		const_iterator cbegin() const;
		iterator end();
		const_iterator end() const;
		const_iterator cend() const;

		reverse_iterator rbegin();
		const_reverse_iterator rbegin() const;
		const_reverse_iterator crbegin() const;
		reverse_iterator rend();
		const_reverse_iterator rend() const;
		const_reverse_iterator crend() const;

		int compare(string const& other) const {
			return std::string_view(*this).compare(other);
		}
		int compare(size_t pos, size_t count, string const& other) const {
			return std::string_view(*this).compare(pos, count, other);
		}
		int compare(size_t pos, size_t count, string const& other, size_t pos2, size_t count2 = npos) const {
			return std::string_view(*this).compare(pos, count, other, pos2, count2);
		}
		int compare(char const* other) const {
			return std::string_view(*this).compare(other);
		}
		int compare(size_t pos, size_t count, char const* other) const {
			return std::string_view(*this).compare(pos, count, other);
		}
		int compare(size_t pos, size_t count, char const* other, size_t count2) const {
			return std::string_view(*this).compare(pos, count, other, count2);
		}
		template <class T>
		requires std::convertible_to<T, std::string_view>
		int compare(T const& other) const {
			return std::string_view(*this).compare(std::string_view(other));
		}
		template <class T>
		requires std::convertible_to<T, std::string_view>
		int compare(size_t pos, size_t count, T const& other) const {
			return std::string_view(*this).compare(pos, count, std::string_view(other));
		}
		template <class T>
		requires std::convertible_to<T, std::string_view>
		int compare(size_t pos, size_t count, T const& other, size_t pos2, size_t count2 = npos) const {
			return std::string_view(*this).compare(pos, count, std::string_view(other), pos2, count2);
		}

		bool operator==(string const& other) const;
		bool operator==(std::string_view other) const;
		bool operator==(char const* other) const {
			return *this == std::string_view(other);
		}
		bool operator==(std::string const& other) const {
			return *this == std::string_view(other);
		}
		std::strong_ordering operator<=>(string const& other) const;
		std::strong_ordering operator<=>(std::string_view other) const;
		std::strong_ordering operator<=>(char const* other) const {
			return *this <=> std::string_view(other);
		}
		std::strong_ordering operator<=>(std::string const& other) const {
			return *this <=> std::string_view(other);
		}

		bool starts_with(std::string_view prefix) const {
			return std::string_view(*this).starts_with(prefix);
		}
		bool starts_with(char const* prefix) const {
			return std::string_view(*this).starts_with(prefix);
		}
		bool starts_with(char prefix) const {
			return std::string_view(*this).starts_with(prefix);
		}

		bool ends_with(std::string_view suffix) const {
			return std::string_view(*this).ends_with(suffix);
		}
		bool ends_with(char const* suffix) const {
			return std::string_view(*this).ends_with(suffix);
		}
		bool ends_with(char suffix) const {
			return std::string_view(*this).ends_with(suffix);
		}

		bool contains(std::string_view str) const {
			return std::string_view(*this).find(str) != npos;
		}
		bool contains(char const* str) const {
			return std::string_view(*this).find(str) != npos;
		}
		bool contains(char c) const {
			return std::string_view(*this).find(c) != npos;
		}

		size_t find(string const& str, size_t pos = 0) const {
			return std::string_view(*this).find(str, pos);
		}
		size_t find(char const* str, size_t pos = 0) const {
			return std::string_view(*this).find(str, pos);
		}
		size_t find(char const* str, size_t pos, size_t count) const {
			return std::string_view(*this).find(str, pos, count);
		}
		size_t find(char c, size_t pos = 0) const {
			return std::string_view(*this).find(c, pos);
		}
		template <class T>
		requires std::convertible_to<T, std::string_view>
		size_t find(T const& other, size_t pos = 0) const {
			return std::string_view(*this).find(std::string_view(other), pos);
		}

		size_t rfind(string const& str, size_t pos = npos) const {
			return std::string_view(*this).rfind(str, pos);
		}
		size_t rfind(char const* str, size_t pos = npos) const {
			return std::string_view(*this).rfind(str, pos);
		}
		size_t rfind(char const* str, size_t pos, size_t count) const {
			return std::string_view(*this).rfind(str, pos, count);
		}
		size_t rfind(char c, size_t pos = npos) const {
			return std::string_view(*this).rfind(c, pos);
		}
		template <class T>
		requires std::convertible_to<T, std::string_view>
		size_t rfind(T const& other, size_t pos = npos) const {
			return std::string_view(*this).rfind(std::string_view(other), pos);
		}

		size_t find_first_of(string const& str, size_t pos = 0) const {
			return std::string_view(*this).find_first_of(str, pos);
		}
		size_t find_first_of(char const* str, size_t pos = 0) const {
			return std::string_view(*this).find_first_of(str, pos);
		}
		size_t find_first_of(char const* str, size_t pos, size_t count) const {
			return std::string_view(*this).find_first_of(str, pos, count);
		}
		size_t find_first_of(char c, size_t pos = 0) const {
			return std::string_view(*this).find_first_of(c, pos);
		}
		template <class T>
		requires std::convertible_to<T, std::string_view>
		size_t find_first_of(T const& other, size_t pos = 0) const {
			return std::string_view(*this).find_first_of(std::string_view(other), pos);
		}

		size_t find_last_of(string const& str, size_t pos = npos) const {
			return std::string_view(*this).find_last_of(str, pos);
		}
		size_t find_last_of(char const* str, size_t pos = npos) const {
			return std::string_view(*this).find_last_of(str, pos);
		}
		size_t find_last_of(char const* str, size_t pos, size_t count) const {
			return std::string_view(*this).find_last_of(str, pos, count);
		}
		size_t find_last_of(char c, size_t pos = npos) const {
			return std::string_view(*this).find_last_of(c, pos);
		}
		template <class T>
		requires std::convertible_to<T, std::string_view>
		size_t find_last_of(T const& other, size_t pos = npos) const {
			return std::string_view(*this).find_last_of(std::string_view(other), pos);
		}

		size_t find_first_not_of(string const& str, size_t pos = 0) const {
			return std::string_view(*this).find_first_not_of(str, pos);
		}
		size_t find_first_not_of(char const* str, size_t pos = 0) const {
			return std::string_view(*this).find_first_not_of(str, pos);
		}
		size_t find_first_not_of(char const* str, size_t pos, size_t count) const {
			return std::string_view(*this).find_first_not_of(str, pos, count);
		}
		size_t find_first_not_of(char c, size_t pos = 0) const {
			return std::string_view(*this).find_first_not_of(c, pos);
		}
		template <class T>
		requires std::convertible_to<T, std::string_view>
		size_t find_first_not_of(T const& other, size_t pos = 0) const {
			return std::string_view(*this).find_first_not_of(std::string_view(other), pos);
		}

		size_t find_last_not_of(string const& str, size_t pos = npos) const {
			return std::string_view(*this).find_last_not_of(str, pos);
		}
		size_t find_last_not_of(char const* str, size_t pos = npos) const {
			return std::string_view(*this).find_last_not_of(str, pos);
		}
		size_t find_last_not_of(char const* str, size_t pos, size_t count) const {
			return std::string_view(*this).find_last_not_of(str, pos, count);
		}
		size_t find_last_not_of(char c, size_t pos = npos) const {
			return std::string_view(*this).find_last_not_of(c, pos);
		}
		template <class T>
		requires std::convertible_to<T, std::string_view>
		size_t find_last_not_of(T const& other, size_t pos = npos) const {
			return std::string_view(*this).find_last_not_of(std::string_view(other), pos);
		}

		operator std::string() const;
		operator std::string_view() const;
	};

    inline string operator+(string const& lhs, string const& rhs) {
        string out = lhs;
        out += rhs;
        return out;
    }

    inline string operator+(string const& lhs, char const* rhs) {
        string out = lhs;
        out += rhs;
        return out;
    }

    inline string operator+(char const* lhs, string const& rhs) {
        string out = lhs;
        out += rhs;
        return out;
    }

    inline string operator+(string const& lhs, char rhs) {
        string out = lhs;
        out += rhs;
        return out;
    }

    inline string operator+(char lhs, string const& rhs) {
        string out = string(1, lhs);
        out += rhs;
        return out;
    }

    inline string operator+(string const& lhs, std::type_identity_t<std::string_view> rhs) {
        string out = lhs;
        out += rhs;
        return out;
    }

    inline string operator+(std::type_identity_t<std::string_view> lhs, string const& rhs) {
        string out = string(lhs);
        out += rhs;
        return out;
    }

    inline string operator+(string&& lhs, string const& rhs) {
        lhs += rhs;
        return std::move(lhs);
    }

    inline string operator+(string const& lhs, string&& rhs) {
        rhs.insert(0, lhs);
        return std::move(rhs);
    }

    inline string operator+(string&& lhs, string&& rhs) {
        lhs += rhs;
        return std::move(lhs);
    }

    inline string operator+(string&& lhs, char const* rhs) {
        lhs += rhs;
        return std::move(lhs);
    }

    inline string operator+(char const* lhs, string&& rhs) {
        rhs.insert(0, lhs);
        return std::move(rhs);
    }

    inline string operator+(string&& lhs, char rhs) {
        lhs += rhs;
        return std::move(lhs);
    }

    inline string operator+(char lhs, string&& rhs) {
        rhs.insert(0, 1, lhs);
        return std::move(rhs);
    }

    inline string operator+(string&& lhs, std::type_identity_t<std::string_view> rhs) {
        lhs += rhs;
        return std::move(lhs);
    }

    inline string operator+(std::type_identity_t<std::string_view> lhs, string&& rhs) {
        rhs.insert(0, lhs);
        return std::move(rhs);
    }

	inline std::string_view format_as(gd::string const& str) {
		return std::string_view(str);
	}
#endif
}