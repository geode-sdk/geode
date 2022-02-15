#include "SharedGen.hpp"
#include <set>

namespace format_strings {

	char const* declare_member_type = R"RAW(
template <class, class, class, class = void>
struct {function_name} {{
private:
	static void wrapper(...) {{}}
public:
	constexpr static inline auto value = &wrapper;
}};

template <class Derived, class Base, class Ret, class ...Parameters>
struct {function_name}<Derived, Base, Ret(Parameters...), std::enable_if_t<
	std::is_member_function_pointer_v<decltype(substitute<Ret, Base, Derived, Parameters...>(&Derived::{function_name}))>
>> {{
private:
	static Ret wrapper(Derived* self, Parameters... ps) {{
		return self->Derived::{function_name}(ps...);
	}}
public:
	constexpr static inline auto value = &wrapper;
}};

template <class Derived, class Base, class Ret, class ...Parameters>
struct {function_name}<Derived, Base, Ret(Parameters...), std::enable_if_t<
	is_function_pointer_v<decltype(substitute<Ret, Base, Derived, Parameters...>(&Derived::{function_name}))>
>> {{
private:
	static Ret wrapper(Parameters... ps) {{
		return Derived::{function_name}(ps...);
	}}
public:
	constexpr static inline auto value = &wrapper;
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
			switch (f.function_type) {
				case kConstructor: [[fallthrough]];
				case kDestructor: continue;
				default: break;
			}
			if (used.find(f.name) != used.end()) continue;
			output += fmt::format(format_strings::declare_member_type,
				fmt::arg("function_name", f.name)
			);
			used.insert(f.name);
		}
	}

	CacShare::writeFile(output);
}
