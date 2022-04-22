#include "SharedGen.hpp"

namespace format_strings {

	char const* declare_address = R"GEN(
GEODE_NOINLINE GEODE_HIDDEN inline static uintptr_t address{global_index}() {{
	static uintptr_t ret = {address};
	return ret;
}}
)GEN";

}

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
