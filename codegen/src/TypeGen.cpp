#include "Shared.hpp"
#include <set>

namespace { namespace format_strings {

	char const* declare_member_type = R"GEN(
using ret{index} = {return};
using func{index} = ret{index}(*)({const}{class_name}*{parameter_type_comma}{parameter_types});
using pure{index} = ret{index}({parameter_types});
using meta{index} = ret{index}({const}{class_name}*{parameter_type_comma}{parameter_types});
using member{index} = ret{index}({class_name}::*)({parameter_types}){const};
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
using meta{index} = ret{index}({const}{class_name}*{parameter_type_comma}{parameter_types});
using member{index} = func{index};
)GEN";

}}

static std::string getReturn(FunctionBegin const& fn, std::string const& parent) {
	if (fn.type != FunctionType::Normal)
		return "void";

	if (fn.ret.name == "auto") {
		std::vector<std::string> declvals;

		for (auto& [t, n] : fn.args) {
			declvals.push_back(fmt::format("std::declval<{}>()", t.name));
		}

		return fmt::format(
			fn.is_static ? "decltype({}::{}({}))" : "decltype(std::declval<{}>().{}({}))",
			parent,
			fn.name,
			fmt::join(declvals, ", ")
		);
	}

	return fn.ret.name;
}

std::string generateTypeHeader(Root& root) {
	std::string output;

	for (auto& c : root.classes) {
		for (auto& f : c.fields) {
			if (codegen::getStatus(f) == BindStatus::Unbindable)
				continue;
			auto fn = f.get_fn();

			char const* used_format = format_strings::declare_member_type;

			if (fn->type != FunctionType::Normal) {
				used_format = format_strings::declare_structor_type;
			}

			if (fn->is_static) {
				used_format = format_strings::declare_static_type;
			}

			output += fmt::format(used_format,
				fmt::arg("parameter_types", codegen::getParameterTypes(*fn)),
				fmt::arg("parameter_type_comma", str_if(", ", !fn->args.empty())),
				fmt::arg("class_name", c.name),
				fmt::arg("const", str_if(" const ", fn->is_const)),
				fmt::arg("index", f.field_id),
				fmt::arg("return", getReturn(*fn, c.name))
			);
		}
	}

	return output;
}
