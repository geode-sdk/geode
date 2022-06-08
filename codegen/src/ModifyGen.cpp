#include "Shared.hpp"
#include <iostream>

namespace { namespace format_strings {
	// requires: class_name
	char const* modify_start = R"GEN(
template<class Derived>
struct Modify<Derived, {class_name}> : ModifyBase<Modify<Derived, {class_name}>> {{
	using ModifyBase<Modify<Derived, {class_name}>>::ModifyBase;
	using Base = {class_name};
	static void apply() {{
		using namespace geode::core::meta;
)GEN";

	// requires: index, class_name, arg_types, function_name, raw_arg_types, non_virtual
	char const* apply_function = R"GEN(
		GEODE_APPLY_MODIFY_FOR_FUNCTION({index}, {function_convention}, {class_name}, {function_name}))GEN";

	char const* modify_end = R"GEN(
	}
};
)GEN";
}}


std::string generateModifyHeader(Root const& root) {
	string output;

	for (auto& [name, c] : root.classes) {
		if (name == "" || name == "cocos2d")
			continue;

		output += fmt::format(::format_strings::modify_start, 
			fmt::arg("class_name", name)
		);

		for (auto& f : c.functions) {
			if (!codegen::isFunctionDefinable(f) && !codegen::isFunctionDefined(f))
				continue; // Function not supported for this platform, skip it
            string function_name = codegen::getFunctionName(f);
            switch (f.function_type) {
                case kConstructor: function_name = "constructor";
                case kDestructor: function_name = "destructor";
                default: break;
            }

			output += fmt::format(::format_strings::apply_function,
				fmt::arg("index", codegen::getIndex(f)),
				fmt::arg("class_name", codegen::getClassName(f)),
				fmt::arg("function_name", function_name),
				fmt::arg("function_convention", codegen::getConvention(f))
			);
		}

		output += ::format_strings::modify_end;
	}

	// fmt::print("{}", output);
	return output;
}
