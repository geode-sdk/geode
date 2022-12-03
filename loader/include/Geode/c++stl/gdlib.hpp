#pragma once

#include <cstdint>
#include <array>

namespace geode::stl {

	template <class Type>
	class AllocatorTemplate {
	protected:
		inline Type* allocate(size_t count);
		inline void deallocate(Type* pointer);
	};

#if defined(GEODE_IS_WINDOWS)
	template <class Type>
	class AllocatorBase {
	protected:
		inline Type* allocate(size_t count) {

		}
		inline void deallocate(Type* pointer) {

		}
	};
#elif defined(GEODE_IS_MACOS) || defined(GEODE_IS_ANDROID)

#elif defined(GEODE_IS_IOS)

#endif


	class StringTemplate : public AllocatorBase {
	protected:
		// depends on already set size and capacity
		inline void setStorage(char* pointer) = delete;
		inline char* getStorage() = delete;

		inline void setSize(size_t size) = delete;
		inline size_t getSize() = delete;

		inline void setCapacity(size_t capacity) = delete;
		inline size_t getCapacity() = delete;
	};

#if defined(GEODE_IS_WINDOWS)
	class StringBase : public StringTemplate {
	protected:
		union {
			std::array<char, 16> m_smallStorage;
			char* m_bigStorage;
		};

		size_t m_size;
		size_t m_capacity;

		inline void setStorage(char* storage) {
			if (m_capacity > 15) {
				return m_bigStorage = storage;
			}
		}
		inline char* getStorage() {
			if (m_capacity > 15) {
				return m_bigStorage;
			}
			return m_smallStorage.data();
		}

		inline void setSize(size_t size) {
			m_size = size;
		}
		inline size_t getSize() {
			return m_size;
		}

		inline void setCapacity(size_t capacity) {
			m_capacity = capacity;
		}
		inline size_t getCapacity() {
			return m_capacity;
		}
	};
#elif defined(GEODE_IS_MACOS) || defined(GEODE_IS_ANDROID)
	class StringBase : public StringTemplate {
	protected:
		char* m_storage;
		size_t m_size;
		size_t m_capacity;

		inline void setStorage(char* storage) {
			m_storage = storage;
		}
		inline char* getStorage() {
			return m_storage;
		}

		inline void setSize(size_t size) {
			m_size = size;
		}
		inline size_t getSize() {
			return m_size;
		}

		inline void setCapacity(size_t capacity) {
			m_capacity = capacity;
		}
		inline size_t getCapacity() {
			return m_capacity;
		}
	};
#elif defined(GEODE_IS_IOS)
	class StringBase : public StringTemplate {
	protected:
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

		inline void setStorage(char* storage) {
			m_storage = storage;
		}
		inline char* getStorage() {
			return m_storage;
		}

		inline void setSize(size_t size) {
			m_size = size;
		}
		inline size_t getSize() {
			return m_size;
		}

		inline void setCapacity(size_t capacity) {
			m_capacity = capacity;
		}
		inline size_t getCapacity() {
			return m_capacity;
		}
	};
#endif

}