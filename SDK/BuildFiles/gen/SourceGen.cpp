#include "SharedGen.hpp"

namespace format_strings {
    char const* source_start = R"CAC(
#include <InterfaceBase.hpp>
using namespace geode;
using cocos2d::CCDestructor;
)CAC";

	char const* declare_using = "\n\nusing _{unqualified_name} = interfaces::${unqualified_name}<>;";

	char const* declare_member = R"CAC(
template<>
struct address_of_t<(_{unqualified_name}::ret{index}({class_name}::*)({raw_arg_types}){const})(&{class_name}::{function_name})> {{
	static inline auto value = _{unqualified_name}::address{index};
}};
_{unqualified_name}::ret{index} {class_name}::{function_name}({raw_args}) {const}{{
    return ((_{unqualified_name}*)this)->_{unqualified_name}::{function_name}({raw_parameters});
}})CAC";

	char const* declare_static = R"CAC(
template<>
struct address_of_t<(_{unqualified_name}::ret{index}(*)({raw_arg_types}){const})(&{class_name}::{function_name})> {{
	static inline auto value = _{unqualified_name}::address{index};
}};
_{unqualified_name}::ret{index} {class_name}::{function_name}({raw_args}) {const}{{
    return _{unqualified_name}::{function_name}({raw_parameters});
}})CAC";

	char const* declare_destructor = R"CAC(
{class_name}::{function_name}() {{
    if (CCDestructor::lock(this)) return;
	CCDestructor::lock(this) = true;
	((_{unqualified_name}*)this)->destructor({raw_parameters});
}})CAC";

	char const* declare_constructor = R"CAC(
{class_name}::{function_name}({raw_args}) : {class_name}(*this) {{
	((_{unqualified_name}*)this)->constructor({raw_parameters});
}})CAC";

	// requires: static, return_type, function_name, raw_parameters, const, class_name, definition
    char const* ool_function_definition = "{return_type} {class_name}::{function_name}({raw_params}){const} {definition}\n";
}

int main(int argc, char** argv) {
    vector<ClassDefinition*> in_order;
    string output(format_strings::source_start);
    Root root = CacShare::init(argc, argv);

    vector<Function> outofline;

    for (auto& [name, c] : root.classes) {
        string unqualifiedName = CacShare::toUnqualified(name);
        output += fmt::format(format_strings::declare_using,
            fmt::arg("unqualified_name", unqualifiedName)
        );

        for (auto& f : c.functions) {
        	if (f.is_defined) {
        		output += fmt::format(format_strings::ool_function_definition,
	                fmt::arg("return_type", CacShare::getReturn(f)),
	                fmt::arg("function_name", f.name),
	                fmt::arg("raw_params", CacShare::formatRawParams(f.args, f.argnames)),
	                fmt::arg("const", f.is_const ? " const" : ""),
	                fmt::arg("class_name", f.parent_class->name),
	                fmt::arg("definition", f.definition)
	        	);
	        	continue;
        	}
            if (f.binds[CacShare::platform].size() == 0) continue; // Function not implemented, skip

            char const* used_format;

            switch (f.function_type) {
                case kVirtualFunction:
                case kRegularFunction:
                    used_format = format_strings::declare_member;
                    break;
                case kStaticFunction:
                    used_format = format_strings::declare_static;
                    break;
                case kDestructor:
               		used_format = format_strings::declare_destructor;
                    break;
                case kConstructor:
                	used_format = format_strings::declare_constructor;
                    break;
            }
            output += fmt::format(used_format,
                fmt::arg("arg_types", CacShare::formatArgTypes(f.args)),
                fmt::arg("raw_arg_types", CacShare::formatRawArgTypes(f.args)),
                fmt::arg("class_name", name),
                fmt::arg("unqualified_name", unqualifiedName),
                fmt::arg("const", f.is_const ? "const " : ""),
                fmt::arg("convention", CacShare::getConvention(f)),
                fmt::arg("function_name",f.name),
                fmt::arg("index",f.index),
                fmt::arg("raw_args", CacShare::formatRawArgs(f.args)),
                fmt::arg("raw_parameters", CacShare::formatRawParameters(f.args.size()))
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
