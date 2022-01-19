#include "SharedGen.hpp"
#include <iostream>

namespace format_strings {
    char const* source_start = R"CAC(
// 
// Copyright camila314 & alk1m123 2022. 
// Automatically generated file
//
#include <InterfaceBase.hpp>
using namespace Cacao;
)CAC";

	char const* declare_using = "\n\nusing _{unqualified_name} = interfaces::${unqualified_name}<>;";

	char const* declare_member = R"CAC(
_{unqualified_name}::ret{index}
{class_name}::{function_name}({raw_args}) {const}{{
    return ((_{unqualified_name}*)this)->_{unqualified_name}::{function_name}({raw_parameters});
}})CAC";

	char const* declare_static = R"CAC(
_{unqualified_name}::ret{index}
{class_name}::{function_name}({raw_args}) {const}{{
    return _{unqualified_name}::{function_name}({raw_parameters});
}})CAC";

	char const* declare_destructor = R"CAC(
{class_name}::{function_name}() {{
    if (destructorLock[this]) return;
	destructorLock[this] = true;
	((_{unqualified_name}*)this)->destructor({raw_parameters});
}})CAC";

	char const* declare_constructor = R"CAC(
{class_name}::{function_name}({raw_args}) : {class_name}(*this) {{
	((_{unqualified_name}*)this)->constructor({raw_parameters});
}})CAC";
}

int main(int argc, char** argv) {
    vector<ClassDefinition*> in_order;
    string output(format_strings::source_start);
    Root root = CacShare::init(argc, argv);

    for (auto& [name, c] : root.classes) {
        string unqualifiedName = CacShare::toUnqualified(name);
        output += fmt::format(format_strings::declare_using,
            fmt::arg("unqualified_name", unqualifiedName)
        );


        for (auto& f : c.functions) {
            if (f.binds[CacShare::platform].size() == 0) continue; // Function not supported for this platform, skip it

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
