#include "SharedGen.hpp"
#include <iostream>

namespace format_strings {
	// requires: class_name
	char const* modify_start = R"RAW(
template<class Derived>
struct Modify<Derived, {class_name}> : ModifyBase<Modify<Derived, {class_name}>> {{
	using ModifyBase<Modify<Derived, {class_name}>>::ModifyBase;
	using Base = {class_name};
	static void apply() {{
		using namespace geode::core::meta;
		using namespace geode::core::meta::x86;
)RAW";

	// requires: index, class_name, arg_types, function_name, raw_arg_types, non_virtual
	char const* apply_function = R"RAW(
		if constexpr (compare::{function_name}<Derived, Base, types::pure{global_index}>::value) {{
			Interface::get()->logInfo(
				"Adding hook at function {class_name}::{function_name}", 
				Severity::Debug
			);
			Interface::get()->addHook<wrap::{function_name}<DefaultConv, Derived, Base, types::pure{global_index}>::value, {function_convention}>(
				"{class_name}::{function_name}", 
				(void*)addresses::address{global_index}()
			);
		}}
)RAW";

	char const* modify_end = R"RAW(
	}
};
)RAW";
}


int main(int argc, char** argv) {
	auto root = CacShare::init(argc, argv);
	string output;

	for (auto& [name, c] : root.classes) {
		if (name == "" || name == "cocos2d::")
			continue;

		output += fmt::format(format_strings::modify_start, 
			fmt::arg("class_name", name)
		);

		for (auto& f : c.functions) {
			if (!CacShare::functionDefined(f))
				continue; // Function not supported for this platform, skip it
			string function_name = f.name;
			switch (f.function_type) {
				case kConstructor: 
					function_name = "constructor";
					break;
				case kDestructor: 
					function_name = "destructor";
					break;
				default: break;
			}

			output += fmt::format(format_strings::apply_function,
				fmt::arg("global_index",f.hash()),
				fmt::arg("class_name", name),
				fmt::arg("function_name", function_name),
				fmt::arg("function_convention", CacShare::getConvention(f))
			);
		}

		output += format_strings::modify_end;
	}

	// fmt::print("{}", output);
	CacShare::writeFile(output);
}
