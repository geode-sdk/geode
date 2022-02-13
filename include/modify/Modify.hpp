
namespace geode::modifier {


	template <class Derived, class Base>
	class ModifyBase {
		// explicit Modify(Property property) idea
	};

	template <class Derived, class Base>
	class Modify {
		void apply() {}
	};

	#include <gen/Modify.hpp>
}
