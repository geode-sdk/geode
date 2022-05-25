#include "Shared.hpp"
#include <set>

namespace { namespace format_strings {

	char const* declare_member_type = R"GEN(
using ret{index} = {return};
using func{index} = ret{index}(*)({const}{const_whitespace}{class_name}*{parameter_type_comma}{parameter_types});
using pure{index} = ret{index}({parameter_types});
using meta{index} = ret{index}({const}{const_whitespace}{class_name}*{parameter_type_comma}{parameter_types});
using member{index} = ret{index}({class_name}::*)({parameter_types}){const_whitespace}{const};
)GEN";

	char const* declare_static_type = R"GEN(
using ret{index} = {return};
using func{index} = ret{index}(*)({parameter_types});
using pure{index} = ret{index}({parameter_types});
using meta{index} = ret{index}({parameter_types});
using member{index} = func{index};
)GEN";

	char const* declare_structor_type = R"GEN(
using ret{index} = void;
using func{index} = ret{index}(*)({class_name}*{parameter_type_comma}{parameter_types});
using pure{index} = ret{index}({parameter_types});
using meta{index} = ret{index}({const}{const_whitespace}{class_name}*{parameter_type_comma}{parameter_types});
using member{index} = func{index};
)GEN";

}}

using std::set;

static string getReturn(Function const& f) {
    switch (f.function_type) {
        case kConstructor: [[fallthrough]];
        case kDestructor:
            return "void";
        default: break;
    }
    if (f.return_type == "auto") {
        vector<string> declval_strings;
        for (string i : f.args) {
            declval_strings.push_back(fmt::format("std::declval<{}>()", i));
        }
        string format;
        if (f.function_type == kStaticFunction) {
            format = "decltype({}::{}({}))";
        }
        else {
            format = "decltype(std::declval<{}>().{}({}))";
        }
        return fmt::format(format, f.parent_class->name, f.name, fmt::join(declval_strings, ", "));
    }
    else return f.return_type;
}

std::string generateTypeHeader(Root const& root) {
	string output("");

	for (auto& [name, c] : root.classes) {
		for (auto& f : c.functions) {
			if (!codegen::isFunctionDefinable(f) && !codegen::isFunctionDefined(f))
                continue; // Function not supported, skip
			char const* used_format;
			switch (f.function_type) {
				case kVirtualFunction:
				case kRegularFunction:
					used_format = ::format_strings::declare_member_type;
					break;
				case kStaticFunction:
					used_format = ::format_strings::declare_static_type;
					break;
				case kDestructor:
				case kConstructor:
					used_format = ::format_strings::declare_structor_type;
					break;
			}

			output += fmt::format(used_format,
				fmt::arg("parameter_types", codegen::getParameterTypes(f)),
				fmt::arg("parameter_type_comma", codegen::getParameterTypeComma(f)),
				fmt::arg("class_name", codegen::getClassName(f)),
				fmt::arg("const", codegen::getConst(f)),
				fmt::arg("const_whitespace", codegen::getConstWhitespace(f)),
				fmt::arg("index", codegen::getIndex(f)),
				fmt::arg("return", getReturn(f))
			);
		}
	}
	return output;
}
