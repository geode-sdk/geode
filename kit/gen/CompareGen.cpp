#include "SharedGen.hpp"
#include <set>

namespace format_strings {

	char const* declare_member_type = R"RAW(
template <class, class, class, class = void>
struct {function_name} {{
	constexpr static inline bool value = false;
}};

template <class Derived, class Base, class Ret, class ...Parameters>
struct {function_name}<Derived, Base, Ret(Parameters...), std::void_t<
	decltype(substitute<Ret, Base, Derived, Parameters...>(&Base::{function_name})),
	decltype(substitute<Ret, Base, Derived, Parameters...>(&Derived::{function_name}))
>> {{
private:
	using base_type = decltype(substitute<Ret, Base, Derived, Parameters...>(&Base::{function_name}));         
	using derived_type = decltype(substitute<Ret, Base, Derived, Parameters...>(&Derived::{function_name}));   
public:
	constexpr static inline bool value =
		function_uuid<(derived_type)&Derived::{function_name}>::value !=
		function_uuid<(base_type)&Base::{function_name}>::value;
}};

)RAW";
}

using std::set;

int main(int argc, char** argv) {
	string output("");
	Root root = CacShare::init(argc, argv);
	set<string> used;

	for (auto& [name, c] : root.classes) {
		for (auto& f : c.functions) {
			if (used.find(f.name) != used.end()) continue;
			output += fmt::format(format_strings::declare_member_type,
				fmt::arg("function_name", f.name)
			);
			used.insert(f.name);
		}
	}

	CacShare::writeFile(output);
}
