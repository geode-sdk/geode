#include "SharedGen.hpp"
#include <set>

namespace format_strings {

	char const* declare_member_type = R"GEN(
GEODE_WRAPPER_FOR_IDENTIFIER({function_name}) 
)GEN";
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
				fmt::arg("function_name", CacShare::getFunctionName(f))
			);
			used.insert(f.name);
		}
	}

	CacShare::writeFile(output);
}
