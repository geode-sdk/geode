#include "SharedGen.hpp"

namespace format_strings {
	char const* source_start = R"CAC(
#include <InterfaceBase.hpp>
#include <utils/casts.hpp>
#include <modify/Addresses.hpp>
#include <modify/Types.hpp>
using namespace geode;
using namespace geode::cast;
using cocos2d::CCDestructor;
using namespace geode::core::meta;
using namespace geode::modifier;
)CAC";

	char const* declare_member_type = R"CAC(
using ret{global_index} = types::ret{global_index};
using func{global_index} = ret{global_index}(*)({const}{constw}{class_name}*{arg_types});
using pure{global_index} = ret{global_index}({class_name}*{arg_types});
using member{global_index} = ret{global_index}({class_name}::*)({raw_arg_types}){const};)CAC";

	char const* declare_member_type_fixed_orig = R"CAC(
using fixori_func{global_index} = ret{global_index}(*)({const}{constw}{class_name}*{arg_types});
using fixori_member{global_index} = ret{global_index}({class_name}::*)({raw_arg_types}){const};
)CAC";

	char const* declare_static_type = R"CAC(
using ret{global_index} = types::ret{global_index};
using func{global_index} = ret{global_index}(*)({raw_arg_types});
using pure{global_index} = ret{global_index}({raw_arg_types});
using member{global_index} = func{global_index};)CAC";

	char const* declare_static_type_fixed_orig = R"CAC(
using fixori_func{global_index} = ret{global_index}(*)({raw_arg_types});
using fixori_member{global_index} = fixori_func{global_index};
)CAC";

	char const* declare_structor_type = R"CAC(
using ret{global_index} = void;
using func{global_index} = ret{global_index}(*)({class_name}*{arg_types});
using pure{global_index} = ret{global_index}({class_name}*{arg_types});
using member{global_index} = func{global_index};)CAC";

	char const* declare_structor_type_fixed_orig = R"CAC(
using func{global_index} = ret{global_index}(*)({class_name}*{arg_types});	
using fixori_member{global_index} = fixori_func{global_index};
)CAC";

	char const* declare_address_of = R"CAC(
template<>
struct address_of_t<(member{global_index})(&{class_name}::{function_name})> {{
	static inline auto value = addresses::address{global_index}();
}};)CAC";

	char const* declare_address_of_fixed = R"CAC(
template<>
struct address_of_t<(fixori_member{global_index})(&{class_name}::{function_name})> {{
	static inline auto value = addresses::address{global_index}();
}};)CAC";

	char const* thunk_adjust = "addresser::thunkAdjust(({is_fixori}member{global_index})(&{class_name}::{function_name}), this)";

	char const* declare_member_function = "reinterpret_cast<func{global_index}>(addresses::address{global_index}())(this{parameters})";
	char const* declare_virtual_function = "reinterpret_cast<func{global_index}>(addresses::address{global_index}())(this{parameters})";
	char const* declare_static_function = "reinterpret_cast<func{global_index}>(addresses::address{global_index}())({raw_parameters})";
	char const* declare_meta_member_function = "Function<pure{global_index}, x86::{convention}>({{addresses::address{global_index}()}})({const_cast}(this){parameters})";
	char const* declare_meta_virtual_function = "Function<pure{global_index}, x86::{convention}>({{addresses::address{global_index}()}})({const_cast}({thunk_adjusted_this}){parameters})";
	char const* declare_meta_static_function = "Function<pure{global_index}, x86::{convention}>({{addresses::address{global_index}()}})({raw_parameters})";

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

	for (auto& [name, c] : root.classes) {
		string unqualifiedName = CacShare::toUnqualified(name);

		for (auto& f : c.functions) {
			auto function_name = f.name;
			auto raw_args = CacShare::formatRawArgs(f.args);
			auto raw_arg_types = CacShare::formatRawArgTypes(f.args);
			auto arg_types = CacShare::formatArgTypes(f.args);
			auto raw_parameters = CacShare::formatRawParameters(f.args.size());
			auto parameters = CacShare::formatParameters(f.args.size());

            auto [reordered_args, fix_args] = CacShare::reorderStructs(f);

            if (reordered_args.size() && CacShare::platform == kWindows) {
				function_name = "_" + f.name;
				raw_args = CacShare::formatRawArgs(fix_args);
				raw_arg_types = CacShare::formatRawArgTypes(fix_args);
				arg_types = CacShare::formatArgTypes(fix_args);
				raw_parameters = CacShare::formatRawParameters(fix_args.size());
				parameters = CacShare::formatParameters(fix_args.size());
			}

			if (f.is_defined) {
				output += fmt::format(format_strings::ool_function_definition,
					fmt::arg("return_type", CacShare::getReturn(f)),
					fmt::arg("function_name", function_name),
					fmt::arg("raw_params", CacShare::formatRawArgs(f.args, f.argnames)),
					fmt::arg("const", f.is_const ? "const" : ""),
					fmt::arg("constw", f.is_const ? " " : ""),
					fmt::arg("class_name", f.parent_class->name),
					fmt::arg("definition", f.definition)
				);
				continue;
			}
			if (f.binds[CacShare::platform].size() == 0) continue; // Function not implemented, skip

			char const* used_declare_format;
			char const* used_function_format;
			char const* used_type_format;
			char const* used_type_format_fixed_orig;

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
					used_function_format = CacShare::platform == kWindows ? 
						format_strings::declare_meta_virtual_function :
						format_strings::declare_virtual_function;
					break;
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
					used_type_format_fixed_orig = format_strings::declare_member_type_fixed_orig;
					break;
				case kStaticFunction:
					used_type_format = format_strings::declare_static_type;
					used_type_format_fixed_orig = format_strings::declare_static_type_fixed_orig;
					break;
				case kDestructor:
				case kConstructor:
					used_type_format = format_strings::declare_structor_type;
					used_type_format_fixed_orig = format_strings::declare_structor_type_fixed_orig;
					break;
			}
		
			// cout << "352947u" << endl;

			output += fmt::format(used_type_format,
				fmt::arg("arg_types", arg_types),
				fmt::arg("raw_arg_types", raw_arg_types),
				fmt::arg("class_name", name),
				fmt::arg("unqualified_name", unqualifiedName),
				fmt::arg("const", f.is_const ? "const " : ""),
				fmt::arg("constw", f.is_const ? " " : ""),
				fmt::arg("convention", CacShare::getConvention(f)),
				fmt::arg("function_name", function_name),
				fmt::arg("index",f.index),
				fmt::arg("global_index", f.hash()),
				fmt::arg("raw_args", raw_args),
				fmt::arg("raw_parameters", raw_parameters),
				fmt::arg("parameters", parameters),
				fmt::arg("return", CacShare::getReturn(f))
			);

			// address_of doesn't make sense to be defined 
			// for the platform _function version (since 
			// that's private so the user can't access that;
			// however address_of also needs some type info, 
			// so this is providing the bare minimum so you 
			// can get the address of the function using 
			// address_of through the "overload" (i.e. 
			// android sig version)

			const char* address_of = format_strings::declare_address_of;
			if (reordered_args.size()) {
				output += fmt::format(used_type_format_fixed_orig,
					fmt::arg("arg_types", CacShare::formatArgTypes(f.args)),
					fmt::arg("raw_arg_types", CacShare::formatRawArgTypes(f.args)),
					fmt::arg("class_name", name),
					fmt::arg("unqualified_name", unqualifiedName),
					fmt::arg("const", f.is_const ? "const " : ""),
					fmt::arg("constw", f.is_const ? " " : ""),
					fmt::arg("convention", CacShare::getConvention(f)),
					fmt::arg("function_name", f.name),
					fmt::arg("index",f.index),
					fmt::arg("global_index", f.hash()),
					fmt::arg("raw_args", CacShare::formatRawArgs(f.args)),
					fmt::arg("raw_parameters", CacShare::formatRawParameters(f.args.size())),
					fmt::arg("parameters", CacShare::formatParameters(f.args.size())),
					fmt::arg("return", CacShare::getReturn(f))
				);
				address_of = format_strings::declare_address_of_fixed;
			}
			// cout << "4tter" << endl;

			switch (f.function_type) {
				case kVirtualFunction:
				case kRegularFunction:
				case kStaticFunction:
					output += fmt::format(address_of, 
						fmt::arg("global_index",f.hash()),
						fmt::arg("class_name", name),
						fmt::arg("index",f.index),
						fmt::arg("unqualified_name", unqualifiedName),
						fmt::arg("function_name", f.name)
					);
				default:
					break;
			}
			
			// cout << "rwerwe" << endl
			
			std::string this_thunked = "this";

			if (f.function_type != kConstructor && f.function_type != kDestructor) {
				this_thunked = fmt::format(format_strings::thunk_adjust,
					fmt::arg("class_name", name),
					fmt::arg("function_name", function_name),
					fmt::arg("global_index", f.hash()),
					fmt::arg("is_fixori", reordered_args.size() ? "fixori_" : "")
				);
			}

			auto function_implementation = fmt::format(used_function_format,
				fmt::arg("arg_types", arg_types),
				fmt::arg("raw_arg_types", raw_arg_types),
				fmt::arg("class_name", name),
				fmt::arg("unqualified_name", unqualifiedName),
				fmt::arg("const", f.is_const ? "const " : ""),
				fmt::arg("constw", f.is_const ? " " : ""),
				fmt::arg("const_cast", f.is_const ? "const_cast<" + name + "*>" : ""),
				fmt::arg("convention", CacShare::getConvention(f)),
				fmt::arg("function_name", function_name),
				fmt::arg("index",f.index),
				fmt::arg("global_index", f.hash()),
				fmt::arg("raw_args", raw_args),
				fmt::arg("raw_parameters", raw_parameters),
				fmt::arg("parameters", parameters),
				fmt::arg("return", CacShare::getReturn(f)),
				fmt::arg("thunk_adjusted_this", this_thunked)
			);

			// cout << "dsffdssd" << endl;
			output += fmt::format(used_declare_format,
				fmt::arg("arg_types", arg_types),
				fmt::arg("raw_arg_types", raw_arg_types),
				fmt::arg("class_name", name),
				fmt::arg("unqualified_name", unqualifiedName),
				fmt::arg("const", f.is_const ? "const " : ""),
				fmt::arg("constw", f.is_const ? " " : ""),
				fmt::arg("convention", CacShare::getConvention(f)),
				fmt::arg("function_name",function_name),
				fmt::arg("index",f.index),
				fmt::arg("global_index", f.hash()),
				fmt::arg("raw_args", raw_args),
				fmt::arg("raw_parameters", raw_parameters),
				fmt::arg("function_implementation", function_implementation)
			);
		}
		if (CacShare::platform == kMac || CacShare::platform == kIos) {
			for (auto& i : c.inlines) {
				if (c.name.find("cocos2d") == string::npos) continue; // cocos inlines
				output += i.inlined + "\n";
			}
		}
	}

	CacShare::writeFile(output);
}
