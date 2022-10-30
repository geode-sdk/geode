#pragma once

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <functional>
#include <Geode/platform/platform.hpp>
// #include <Geode/binding/GDString.hpp>

#if defined(GEODE_IS_WINDOWS) 

namespace gd {
	struct InternalString {
		union {
			char m_storage[16];
			char* m_pointer;
		};
		size_t m_length;
		size_t m_capacity;
	};

	class GEODE_DLL string : protected GDString {
	private:
		InternalString m_data;

	public:

		template <class... Params>
		string(Params&&... params) {
			m_data.m_pointer = 0;
			m_data.m_length = 0;
			m_data.m_capacity = 15;

			auto val = std::string(std::forward<Params>(params)...);
			(void)this->winAssign(val.c_str(), val.size());
		}

		~string() {
			(void)this->winDtor();
		}

		/*template <class... Params>
		decltype(auto) operator=(Params&&... params) -> decltype(this->operator=(std::forward<Params>(params)...)) {
			auto val = std::string(this->winCStr(), m_data.m_length);
			val.operator=(std::forward<Params>(params)...);
			(void)this->winAssign(val.c_str(), val.size());
		}*/

		template <class... Params>
		string& operator=(Params&&... params) {
			auto val = std::string(this->winCStr(), m_data.m_length);
			val.operator=(std::forward<Params>(params)...);
			(void)this->winAssign(val.c_str(), val.size());
			return *this;
		}

		template <class... Params>
		string& assign(Params&&... params) {
			auto val = std::string(this->winCStr(), m_data.m_length);
			val.assign(std::forward<Params>(params)...);
			(void)this->winAssign(val.c_str(), val.size());
			return *this;
		}

		char& at(size_t pos) {
			if (pos >= m_data.m_length) throw std::out_of_range("gd::string::at");
		}

		char const& at(size_t pos) const {
			if (pos >= m_data.m_length) throw std::out_of_range("gd::string::at");
		}

		char& operator[](size_t pos) {
			return this->winCStr()[pos];
		}

		char const& operator[](size_t pos) const {
			return this->winCStr()[pos];
		}

		char* data() {
			return this->winCStr();
		}

		char const* data() const {
			return this->winCStr();
		}

		char const* c_str() const {
			return this->winCStr();
		}
	};
}

#endif