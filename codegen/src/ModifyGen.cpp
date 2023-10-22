#include "Shared.hpp"

#include <iostream>
#include <set>

namespace {
    namespace format_strings {
        // requires: class_name, class_include
        char const* modify_start = R"GEN(#pragma once
#include <Geode/modify/Modify.hpp>
#include <Geode/modify/Field.hpp>
#include <Geode/modify/Addresses.hpp>
{class_include}
using namespace geode::modifier;
namespace geode::modifier {{
    {statics}

	template<class Der>
	struct ModifyDerive<Der, {class_name}> : ModifyBase<ModifyDerive<Der, {class_name}>> {{
        using BaseModify = ModifyBase<ModifyDerive<Der, {class_name}>>;
		using ModifyBase<ModifyDerive<Der, {class_name}>>::ModifyBase;
		using Base = {class_name};
        using Derived = Der;
		void apply() override {{
			using namespace geode::core::meta;

)GEN";

        char const* statics_declare_identifier = R"GEN(
	#ifndef GEODE_STATICS_{function_name}
		#define GEODE_STATICS_{function_name}
		GEODE_AS_STATIC_FUNCTION({function_name}) 
	#endif
)GEN";

        // requires: index, class_name, arg_types, function_name, raw_arg_types, non_virtual
        char const* apply_function = R"GEN(
			GEODE_APPLY_MODIFY_FOR_FUNCTION({addr_index}, {function_convention}, {class_name}, {function_name}, {parameter_types}))GEN";

        char const* apply_constructor = R"GEN(
			GEODE_APPLY_MODIFY_FOR_CONSTRUCTOR({addr_index}, {function_convention}, {class_name}, {parameter_types}))GEN";

        char const* apply_destructor = R"GEN(
			GEODE_APPLY_MODIFY_FOR_DESTRUCTOR({addr_index}, {function_convention}, {class_name}))GEN";

        char const* modify_end = R"GEN(
		}
	};
}
)GEN";

        char const* modify_include = R"GEN(#include "modify/{file_name}"
)GEN";
    }
}

std::string generateModifyHeader(Root const& root, ghc::filesystem::path const& singleFolder) {
    std::string output;

    for (auto& c : root.classes) {        
        if (c.name == "cocos2d") continue;

        std::string filename = (codegen::getUnqualifiedClassName(c.name) + ".hpp");
        output += fmt::format(format_strings::modify_include, fmt::arg("file_name", filename));

        std::string single_output;

        std::string class_include;

        if (c.name.find("cocos2d::extension") != std::string::npos) {
            class_include = "#include <cocos-ext.h>";
        }
        else if (is_cocos_class(c.name)) {
            class_include = "#include <cocos2d.h>";
        }
        else {
            class_include = fmt::format(
                "#include <Geode/binding/{class_name}.hpp>",
                fmt::arg("class_name", codegen::getUnqualifiedClassName(c.name))
            );
        }

        std::string statics;
        std::set<std::string> used;
        for (auto& f : c.fields) {
            if (codegen::getStatus(f) == BindStatus::Missing) continue;

            if (auto fn = f.get_as<FunctionBindField>()) {
                if (fn->prototype.type == FunctionType::Normal && !used.count(fn->prototype.name)) {
                    used.insert(fn->prototype.name);
                    statics += fmt::format(
                        format_strings::statics_declare_identifier, fmt::arg("function_name", fn->prototype.name)
                    );
                }
            }
        }

        single_output += fmt::format(
            format_strings::modify_start,
            fmt::arg("statics", statics),
            fmt::arg("class_name", c.name),
            fmt::arg("class_include", class_include)
        );

        // modify
        for (auto& f : c.fields) {
            if (codegen::getStatus(f) == BindStatus::Missing) continue;

            auto fn = f.get_as<FunctionBindField>();

            if (!fn) {
                continue;
            }

            if (codegen::getStatus(f) == BindStatus::NeedsBinding || codegen::platformNumber(f) != -1) {
                
            }
            else if (codegen::getStatus(f) == BindStatus::Binded && fn->prototype.type == FunctionType::Normal) {
                
            }
            else {
                continue;
            }

            std::string format_string;

            switch (fn->prototype.type) {
                case FunctionType::Normal:
                    format_string = format_strings::apply_function;
                    break;
                case FunctionType::Ctor:
                    format_string = format_strings::apply_constructor;
                    break;
                case FunctionType::Dtor:
                    format_string = format_strings::apply_destructor;
                    break;
            }

            single_output += fmt::format(
                format_string,
                fmt::arg("addr_index", codegen::getId(&f)),
                fmt::arg("class_name", c.name),
                fmt::arg("function_name", fn->prototype.name),
                fmt::arg("function_convention", codegen::getModifyConventionName(f)),
                fmt::arg("parameter_types", codegen::getParameterTypes(fn->prototype))
            );
        }

        single_output += format_strings::modify_end;

        writeFile(singleFolder / filename, single_output);
    }

    return output;
}
