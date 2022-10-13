#include "Shared.hpp"
#include <iostream>

namespace { namespace format_strings {
	// requires: class_name
	char const* modify_start = R"GEN(#pragma once
#include <Geode/modify/Modify.hpp>
#include <Geode/modify/Field.hpp>
#include <Geode/modify/InternalMacros.hpp>
using namespace geode::modifier;

namespace geode::modifier {{
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
}
)GEN";

	char const* modify_include = R"GEN(#include "modify/{file_name}"
)GEN";
}}


std::string generateModifyHeader(Root& root, ghc::filesystem::path const& singleFolder) {
	std::string output;

	for (auto c : root.classes) {
		if (c.name == "cocos2d")
			continue;

		std::string filename = (codegen::getUnqualifiedClassName(c.name) + ".hpp");
		output += fmt::format(format_strings::modify_include, 
			fmt::arg("file_name", filename)
		);

		std::string single_output;

		single_output += fmt::format(format_strings::modify_start, 
			fmt::arg("class_name", c.name)
		);

		for (auto& f : c.fields) {
			if (codegen::getStatus(f) != BindStatus::Unbindable) {
				auto begin = f.get_fn();

				std::string function_name;

				switch (begin->type) {
					case FunctionType::Normal:
						function_name = begin->name;
						break;
					case FunctionType::Ctor:
						function_name = "constructor";
						break;
					case FunctionType::Dtor:
						function_name = "destructor";
						break;
				}

				single_output += fmt::format(format_strings::apply_function,
					fmt::arg("index", f.field_id),
					fmt::arg("class_name", c.name),
					fmt::arg("function_name", function_name),
					fmt::arg("function_convention", codegen::getConvention(f))
				);
			}
		}

		single_output += format_strings::modify_end;

		writeFile(singleFolder / filename, single_output);
	}

	return output;
}
