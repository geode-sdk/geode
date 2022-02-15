#include "SharedGen.hpp"

namespace format_strings {

	char const* declare_address = R"CAC(
GEODE_NOINLINE GEODE_HIDDEN inline static uintptr_t address{global_index}() {{
	static uintptr_t ret = {address};
	return ret;
}}
)CAC";

}

int main(int argc, char** argv) {
	string output("");
	Root root = CacShare::init(argc, argv);

	for (auto& [name, c] : root.classes) {
		string unqualifiedName = CacShare::toUnqualified(name);

		for (auto& f : c.functions) {
			if (!CacShare::functionDefined(f))
                continue; // Function not supported, skip

			output += fmt::format(format_strings::declare_address,
				fmt::arg("address", CacShare::getAddress(f)),
				fmt::arg("global_index", f.global_index)
			);
		}
	}

	CacShare::writeFile(output);
}
