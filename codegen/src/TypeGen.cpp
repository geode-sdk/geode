#include "Shared.hpp"
#include "TypeOpt.hpp"
#include <set>

std::string generateTypeHeader(Root& root) {
	std::string output;

	TypeBank bank;
	bank.loadFrom(root);

	std::map<std::string, int> used_returns;
	std::map<std::string, int> used_funcs;
	std::map<std::string, int> used_pures;

	int i = 0;
	for (auto& f : bank.typeList()) {

		char const* return_fmt = "using ret{index} = {return};";
		char const* func_fmt;
		char const* pure_fmt = "ret{ret_index}({parameter_types});";
		char const* meta_fmt;
		char const* member_fmt;

		switch (f.type) {
			case FuncType::Member:
				func_fmt = "ret{ret_index}(*)( {const}{class_name}*{parameter_type_comma}{parameter_types});";
				meta_fmt = "ret{ret_index}({const}{class_name}*{parameter_type_comma}{parameter_types});";
				member_fmt = "ret{ret_index}({class_name}::*)({parameter_types}){const};";
				break;
			case FuncType::Static:
				func_fmt = "ret{ret_index}(*)({parameter_types});";
				meta_fmt = "ret{ret_index}({parameter_types});";
				member_fmt = "func{index};";
				break;
			case FuncType::Structor:
				func_fmt = "ret{ret_index}(*)({class_name}*{parameter_type_comma}{parameter_types});";
				meta_fmt = "ret{ret_index}({const}{class_name}*{parameter_type_comma}{parameter_types});";
				member_fmt = "func{index};";
				break;
		}

		if (used_returns.count(f.return_type) == 0) {
			output += fmt::format(return_fmt,
				fmt::arg("index", i),
				fmt::arg("return", f.return_type)
			) + "\n";
			used_returns[f.return_type] = i;
		}
		int ret_index = used_returns[f.return_type];

		std::string pure_val = fmt::format(pure_fmt,
			fmt::arg("ret_index", ret_index),
			fmt::arg("parameter_types", fmt::join(f.parameter_types, ", "))
		);
		if (used_pures.count(pure_val) == 0) {
			output += fmt::format("using pure{} = {}\n", i, pure_val);
			used_pures[pure_val] = i;
		}

		std::string func_val = fmt::format(func_fmt,
			fmt::arg("ret_index", ret_index),
			fmt::arg("parameter_types", fmt::join(f.parameter_types, ", ")),
			fmt::arg("parameter_type_comma", str_if(", ", !f.parameter_types.empty())),
			fmt::arg("class_name", f.class_name),
			fmt::arg("const", str_if(" const ", f.is_const))
		);
		std::string meta_val = fmt::format(meta_fmt,
			fmt::arg("ret_index", ret_index),
			fmt::arg("parameter_types", fmt::join(f.parameter_types, ", ")),
			fmt::arg("parameter_type_comma", str_if(", ", !f.parameter_types.empty())),
			fmt::arg("class_name", f.class_name),
			fmt::arg("const", str_if(" const ", f.is_const))
		);
		std::string member_val = fmt::format(member_fmt,
			fmt::arg("ret_index", ret_index),
			fmt::arg("parameter_types", fmt::join(f.parameter_types, ", ")),
			fmt::arg("class_name", f.class_name),
			fmt::arg("const", str_if(" const ", f.is_const)),
			fmt::arg("index", i)
		);

		if (used_funcs.count(func_val) == 0) {
			output += fmt::format("using func{index} = {func}\nusing meta{index} = {meta}\nusing member{index} = {member}\n",
				fmt::arg("index", i), 
				fmt::arg("func", func_val),
				fmt::arg("meta", meta_val),
				fmt::arg("member", member_val)
			);
			used_funcs[func_val] = i;
		}

		++i;
	}

	return output;
}
