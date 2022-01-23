#include "SharedGen.hpp"

namespace format_strings {
	char const* source_start = R"CAC(
#include <InterfaceBase.hpp>
using namespace geode;
using cocos2d::CCDestructor;
)CAC";

	char const* declare_address = R"CAC(
static uintptr_t _address{global_index};
GEODE_NOINLINE inline auto address{global_index}() {{
	if (!_address{global_index})
		_address{global_index} = interfaces::${unqualified_name}<>::address{index}();
	return _address{global_index};
}})CAC";

	char const* declare_member_type = R"CAC(
using ret{global_index} = interfaces::${unqualified_name}<>::ret{index};
using func{global_index} = ret{global_index}(*)({const}{constw}{class_name}*{arg_types});
using pure{global_index} = ret{global_index}({class_name}*{arg_types}){constw}{const};
using member{global_index} = ret{global_index}({class_name}::*)({raw_arg_types}){const};)CAC";

	char const* declare_static_type = R"CAC(
using ret{global_index} = interfaces::${unqualified_name}<>::ret{index};
using func{global_index} = ret{global_index}(*)({raw_arg_types});
using pure{global_index} = ret{global_index}({raw_arg_types}){constw}{const};
using member{global_index} = func{global_index};)CAC";

	char const* declare_structor_type = R"CAC(
using ret{global_index} = void;
using func{global_index} = ret{global_index}(*)({class_name}*{arg_types});
using pure{global_index} = ret{global_index}({class_name}*{arg_types});
using member{global_index} = func{global_index};)CAC";

	char const* declare_address_of = R"CAC(
template<>
struct address_of_t<(member{global_index})(&{class_name}::{function_name})> {{
	static inline auto value = address{global_index}();
}};)CAC";

	char const* declare_member_function = "reinterpret_cast<func{global_index}>(address{global_index}())(this{parameters})";
	char const* declare_static_function = "reinterpret_cast<func{global_index}>(address{global_index}())({raw_parameters})";
	char const* declare_meta_member_function = "geode::core::meta::Function<pure{global_index}, geode::core::meta::x86::{convention}>({{address{global_index}()}})(this{parameters})";
	char const* declare_meta_static_function = "geode::core::meta::Function<pure{global_index}, geode::core::meta::x86::{convention}>({{address{global_index}()}})({raw_parameters})";

	char const* declare_member = R"CAC(
ret{global_index} {class_name}::{function_name}({raw_args}){constw}{const} {{
	return {function_implementation};
}}
)CAC";

	char const* declare_static = R"CAC(
ret{global_index} {class_name}::{function_name}({raw_args}){constw}{const} {{
	return {function_implementation};
}}
)CAC";

	char const* declare_destructor = R"CAC(
{class_name}::{function_name}() {{
	if (CCDestructor::lock(this)) return;
	CCDestructor::lock(this) = true;
	{function_implementation};
}}
)CAC";

	char const* declare_constructor = R"CAC(
{class_name}::{function_name}({raw_args}) : {class_name}(*this) {{
	{function_implementation};
}}
)CAC";

	// requires: static, return_type, function_name, raw_parameters, const, class_name, definition
	char const* ool_function_definition = "{return_type} {class_name}::{function_name}({raw_params}){constw}{const} {definition}\n";
}

int main(int argc, char** argv) {
	vector<ClassDefinition*> in_order;
	string output(format_strings::source_start);
	Root root = CacShare::init(argc, argv);

	int global_index = 0;

	for (auto& [name, c] : root.classes) {
		string unqualifiedName = CacShare::toUnqualified(name);

		for (auto& f : c.functions) {
			if (f.is_defined) {
				output += fmt::format(format_strings::ool_function_definition,
					fmt::arg("return_type", CacShare::getReturn(f)),
					fmt::arg("function_name", f.name),
					fmt::arg("raw_params", CacShare::formatRawParams(f.args, f.argnames)),
					fmt::arg("const", f.is_const ? "const" : ""),
					fmt::arg("constw", f.is_const ? " " : ""),
					fmt::arg("class_name", f.parent_class->name),
					fmt::arg("definition", f.definition)
				);
				continue;
			}
			if (f.binds[CacShare::platform].size() == 0) continue; // Function not implemented, skip
			++global_index;

			char const* used_declare_format;
			char const* used_function_format;
			char const* used_type_format;

			switch (f.function_type) {
				case kVirtualFunction:
				case kRegularFunction:
					used_declare_format = format_strings::declare_member;
					break;
				case kStaticFunction:
					used_declare_format = format_strings::declare_static;
					break;
				case kDestructor:
					used_declare_format = format_strings::declare_destructor;
					break;
				case kConstructor:
					used_declare_format = format_strings::declare_constructor;
					break;
			}
			switch (f.function_type) {
				case kVirtualFunction:
				case kRegularFunction:
				case kDestructor:
				case kConstructor:
					used_function_format = CacShare::platform == kWindows ? 
						format_strings::declare_meta_member_function :
						format_strings::declare_member_function;
					break;
				case kStaticFunction:
					used_function_format = CacShare::platform == kWindows ? 
						format_strings::declare_meta_static_function :
						format_strings::declare_static_function;
					break;
			}
			switch (f.function_type) {
				case kVirtualFunction:
				case kRegularFunction:
					used_type_format = format_strings::declare_member_type;
					break;
				case kStaticFunction:
					used_type_format = format_strings::declare_static_type;
					break;
				case kDestructor:
				case kConstructor:
					used_type_format = format_strings::declare_structor_type;
					break;
			}

			// cout << "dsfdf" << endl;
			output += fmt::format(format_strings::declare_address, 
				fmt::arg("global_index",global_index),
				fmt::arg("index",f.index),
				fmt::arg("unqualified_name", unqualifiedName),
				fmt::arg("address", CacShare::getAddress(f))
			);
		
			// cout << "352947u" << endl;

			output += fmt::format(used_type_format,
				fmt::arg("arg_types", CacShare::formatArgTypes(f.args)),
				fmt::arg("raw_arg_types", CacShare::formatRawArgTypes(f.args)),
				fmt::arg("class_name", name),
				fmt::arg("unqualified_name", unqualifiedName),
				fmt::arg("const", f.is_const ? "const " : ""),
				fmt::arg("constw", f.is_const ? " " : ""),
				fmt::arg("convention", CacShare::getConvention(f)),
				fmt::arg("function_name", f.name),
				fmt::arg("index",f.index),
				fmt::arg("global_index", global_index),
				fmt::arg("raw_args", CacShare::formatRawArgs(f.args)),
				fmt::arg("raw_parameters", CacShare::formatRawParameters(f.args.size())),
				fmt::arg("parameters", CacShare::formatParameters(f.args.size())),
				fmt::arg("return", CacShare::getReturn(f))
			);
			// cout << "4tter" << endl;

			switch (f.function_type) {
				case kVirtualFunction:
				case kRegularFunction:
				case kStaticFunction:
					output += fmt::format(format_strings::declare_address_of, 
						fmt::arg("global_index",global_index),
						fmt::arg("class_name", name),
						fmt::arg("index",f.index),
						fmt::arg("unqualified_name", unqualifiedName),
						fmt::arg("function_name", f.name)
					);
				default:
					break;
			}
			
			// cout << "rwerwe" << endl;

			auto function_implementation = fmt::format(used_function_format,
				fmt::arg("arg_types", CacShare::formatArgTypes(f.args)),
				fmt::arg("raw_arg_types", CacShare::formatRawArgTypes(f.args)),
				fmt::arg("class_name", name),
				fmt::arg("unqualified_name", unqualifiedName),
				fmt::arg("const", f.is_const ? "const " : ""),
				fmt::arg("constw", f.is_const ? " " : ""),
				fmt::arg("convention", CacShare::getConvention(f)),
				fmt::arg("function_name", f.name),
				fmt::arg("index",f.index),
				fmt::arg("global_index", global_index),
				fmt::arg("raw_args", CacShare::formatRawArgs(f.args)),
				fmt::arg("raw_parameters", CacShare::formatRawParameters(f.args.size())),
				fmt::arg("parameters", CacShare::formatParameters(f.args.size())),
				fmt::arg("return", CacShare::getReturn(f))
			);

			// cout << "dsffdssd" << endl;
			output += fmt::format(used_declare_format,
				fmt::arg("arg_types", CacShare::formatArgTypes(f.args)),
				fmt::arg("raw_arg_types", CacShare::formatRawArgTypes(f.args)),
				fmt::arg("class_name", name),
				fmt::arg("unqualified_name", unqualifiedName),
				fmt::arg("const", f.is_const ? "const " : ""),
				fmt::arg("constw", f.is_const ? " " : ""),
				fmt::arg("convention", CacShare::getConvention(f)),
				fmt::arg("function_name",f.name),
				fmt::arg("index",f.index),
				fmt::arg("global_index", global_index),
				fmt::arg("raw_args", CacShare::formatRawArgs(f.args)),
				fmt::arg("raw_parameters", CacShare::formatRawParameters(f.args.size())),
				fmt::arg("function_implementation", function_implementation)
			);
		}
		if (CacShare::platform == kMac || CacShare::platform == kIos) {
			for (auto& i : c.inlines) {
				if (c.name.find("cocos2d::") == string::npos) continue; // cocos inlines
				output += i.inlined + "\n";
			}
		}
	}

	CacShare::writeFile(output);
}
