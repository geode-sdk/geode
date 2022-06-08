#pragma once

#include <vector>

namespace geode {
    using byte_array = std::vector<uint8_t>;

    template <typename T>
    byte_array to_byte_array(T& a) {
    	uint8_t* c_array = reinterpret_cast<uint8_t*>(&a);

    	return byte_array(c_array, c_array + sizeof(T));
    }

    template<class T>
	struct TypeIdentity {
	    using type = T;
	};

	template<class T>
	using TypeIdentityType = typename TypeIdentity<T>::type;

    #ifndef GEODE_NO_CRINGE

    using unknown_t = uintptr_t;
    using edx_t = uintptr_t;

    #endif
}
