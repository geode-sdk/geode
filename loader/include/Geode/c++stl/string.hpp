#pragma once

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
		string();
		string(string const&);
		// string(string&&);
		string(char const*);
		string(char const*, size_t);
		string(std::string const&);
		// tried to add a string_view ctor, but got overload errors :(
		~string();

		string& operator=(string const&);
		string& operator=(string&&);
		string& operator=(char const*);
		string& operator=(std::string const&);

		void clear();
		
		char& at(size_t pos);
		char const& at(size_t pos) const;

		char& operator[](size_t pos);
		char const& operator[](size_t pos) const;

		char* data();
		char const* data() const;
		char const* c_str() const;

		size_t size() const;
		size_t capacity() const;
		bool empty() const;

		bool operator==(string const& other) const;
		bool operator==(std::string_view const other) const;
		bool operator==(char const* other) const {
			return *this == std::string_view(other);
		}
		bool operator==(std::string const& other) const {
			return *this == std::string_view(other);
		}
		std::strong_ordering operator<=>(string const& other) const;
		std::strong_ordering operator<=>(std::string_view const other) const;
		std::strong_ordering operator<=>(char const* other) const {
			return *this <=> std::string_view(other);
		}
		std::strong_ordering operator<=>(std::string const& other) const {
			return *this <=> std::string_view(other);
		}

		operator std::string() const;
		operator std::string_view() const;
	};

#endif
}