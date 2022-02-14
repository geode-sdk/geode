#include "SharedGen.hpp"
#include <iostream>

namespace format_strings {
    // requires: class_name
    char const* modify_start = R"RAW(
template<class Derived>
struct Modify<Derived, {class_name}> : ModifyBase<Derived, {class_name}> {{
    using ModifyBase::ModifyBase;
    using Base = {class_name};
    static void apply() {{
)RAW";

    // requires: index, class_name, arg_types, function_name, raw_arg_types, non_virtual
    char const* apply_function = R"RAW(
    	if (compare::init<Derived, Base, types::pure{global_index}>::value) {{
    		Interface::get()->logInfo(
    			"Adding hook at function {class_name}::{function_name}", 
    			Severity::Debug
    		);
            Interface::get()->addHook(
            	"{class_name}::{function_name}", 
            	types::address{global_index}(), 
            	(void*)wrap::init<Derived, Base, types::pure{global_index}>::value
            );
        }}
)RAW";

    char const* apply_end = R"RAW(
    }
};
)RAW";
}


int main(int argc, char** argv) {
    auto root = CacShare::init(argc, argv);
    string output;

    for (auto& [name, c] : root.classes) {
        string unqualifiedName = CacShare::toUnqualified(name);

        output += fmt::format(format_strings::interface_start, fmt::arg("class_name", unqualifiedName), fmt::arg("raw_class_name", name));

        for (auto& f : c.functions) {
            if (!CacShare::functionDefined(f))
                continue; // Function not supported for this platform, skip it
            string name;
            switch (f.function_type) {
            	case kDestructor:
            		name = "destructor";
            	case kConstructor:
            		if (name != "destructor") name = "constructor";
            		output += fmt::format(format_strings::declare_structor,
				        fmt::arg("arg_types", CacShare::formatArgTypes(f.args)),
				        fmt::arg("raw_arg_types", CacShare::formatRawArgTypes(f.args)),
				        fmt::arg("class_name", unqualifiedName),
				        fmt::arg("const", f.is_const ? "const " : ""),
				        fmt::arg("convention", CacShare::getConvention(f)),
				        fmt::arg("function_name",name),
				        fmt::arg("global_index",f.global_index),
				        fmt::arg("parameters", CacShare::formatParameters(f.args.size())),
				        fmt::arg("raw_args", CacShare::formatRawArgs(f.args)),
				        fmt::arg("raw_parameters", CacShare::formatRawParameters(f.args.size()))
				    );
            	default:
            		break;
            }
        }

        output += format_strings::apply_start;

        for (auto& f : c.functions) {
            if (!CacShare::functionDefined(f))
                continue; // Function not supported for this platform, skip it

            char const* used_format;
            string name = f.name;
            switch (f.function_type) {
                case kStaticFunction:
                    used_format = format_strings::apply_function_static;
                    break;
                case kDestructor:
            		name = "destructor";
            	case kConstructor:
            		if (name != "destructor") name = "constructor";
                    used_format = format_strings::apply_function_structor;
                    break;
                default:
               		used_format = format_strings::apply_function_member;
                    break;
            }

            output += fmt::format(used_format,
                fmt::arg("global_index",f.hash()),
                fmt::arg("class_name", unqualifiedName),
                fmt::arg("arg_types", CacShare::formatArgTypes(f.args)),
                fmt::arg("function_name", name),
                fmt::arg("raw_arg_types", CacShare::formatRawArgTypes(f.args)),
                fmt::arg("non_virtual", f.function_type == kVirtualFunction ? "" : "Non"),
                fmt::arg("const", f.is_const ? "const " : "")
            );
        }

        output += format_strings::apply_end;
        output += "};\n";
    }

    // fmt::print("{}", output);
    CacShare::writeFile(output);
}
