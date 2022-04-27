#include "SharedGen.hpp"

namespace format_strings {

	char const* declare_address = R"GEN(
GEODE_NOINLINE GEODE_HIDDEN inline static uintptr_t address{index}() {{
	static uintptr_t ret = {address};
	return ret;
}}
)GEN";

}

static string getAddress(Function const& f) {
    if (CacShare::isFunctionDefined(f)) {
        string format;
        if (f.function_type == kVirtualFunction)
            format = "addresser::getVirtual((types::member{})(&{}::{}))";
        else
            format = "addresser::getNonVirtual((types::member{})(&{}::{}))";
        return fmt::format(format, f.global_index, f.parent_class->name, f.name);
    }
    else return "base::get()+" + f.binds[CacShare::platform];
}

int main(int argc, char** argv) {
	string output("");
	Root root = CacShare::init(argc, argv);

	for (auto& [name, c] : root.classes) {
		for (auto& f : c.functions) {
			if (!CacShare::isFunctionDefinable(f) && !CacShare::isFunctionDefined(f))
                continue; // Function not supported, skip

			output += fmt::format(format_strings::declare_address,
				fmt::arg("address", getAddress(f)),
				fmt::arg("index", CacShare::getIndex(f))
			);
		}
	}
	CacShare::writeFile(output);
}
