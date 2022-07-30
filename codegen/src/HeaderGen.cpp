#include "Shared.hpp"
#include <iostream>
#include <set>

namespace { namespace format_strings {
	char const* class_predeclare = "class {class_name};\n";
    // requires: base_classes, class_name
    char const* class_start = R"GEN(
class {class_name}{base_classes} {{
public:
)GEN";

	char const* monostate_constructor = R"GEN(
	GEODE_MONOSTATE_CONSTRUCTOR_GD({class_name}, {first_base})
)GEN";

	char const* monostate_constructor_cutoff = R"GEN(
	GEODE_MONOSTATE_CONSTRUCTOR_CUTOFF({class_name}, {first_base})
)GEN";

    char const* function_definition = R"GEN(
    {docs}{static}{virtual}{return_type} {function_name}({parameters}){const};
)GEN";

    char const* error_definition = R"GEN(
    template <bool T=false>
    {static}{return_type} {function_name}({parameters}){const}{{
        static_assert(T, "Implement {class_name}::{function_name}");
    }}
)GEN";

    char const* structor_definition = R"GEN(
    {function_name}({parameters});)GEN";
    
    // requires: type, member_name, array
    char const* member_definition = R"GEN(
    {type} {member_name};)GEN";

    char const* pad_definition = R"GEN(
    GEODE_PAD({hardcode});)GEN";
    char const* unimplemented_definition = R"GEN(
    GEODE_UNIMPLEMENTED_PAD)GEN";

    // requires: hardcode_macro, type, member_name, hardcode
    char const* hardcode_definition = R"GEN(
    CLASSPARAM({type}, {member_name}, {hardcode});)GEN";

    char const* class_end = R"GEN(
};
)GEN";
}}

std::string generateGDHeader(Root& root) {
    std::string output("#pragma once\n#include <Geode/c++stl/gdstdlib.hpp>\n#include <cocos2d.h>\n");

    for (auto& cls : root.classes) {
        if (can_find(cls.name, "cocos2d"))
            continue;

        output += fmt::format(::format_strings::class_predeclare,
            fmt::arg("class_name", cls.name)
        );
    }

   	for (auto& cls : root.classes) {
        if (can_find(cls.name, "cocos2d"))
            continue;

        std::string supers = str_if(
            fmt::format(" : public {}", fmt::join(cls.superclasses, ", ")),
            !cls.superclasses.empty()
        );

        output += fmt::format(::format_strings::class_start,
            fmt::arg("class_name", cls.name),
            fmt::arg("base_classes", supers)
        );

        // what.
        if (!cls.superclasses.empty()) {
            output += fmt::format(
                can_find(cls.superclasses[0], "cocos2d") 
                    ? format_strings::monostate_constructor_cutoff
                    : format_strings::monostate_constructor,
                fmt::arg("class_name", cls.name),
                fmt::arg("first_base", cls.superclasses[0])
            );
        }

        for (auto field : cls.fields) {
            FunctionBegin* fb;
            char const* used_format = format_strings::function_definition;

            if (auto i = field.get_as<InlineField>()) {
                output += "\t" + i->inner + "\n";
                continue;
            } else if (auto m = field.get_as<MemberField>()) {
                output += fmt::format(format_strings::member_definition,
                    fmt::arg("type", m->type.name),
                    fmt::arg("member_name", m->name + str_if(fmt::format("[{}]", m->count), m->count))
                );
                continue;
            } else if (auto p = field.get_as<PadField>()) {
                auto hardcode = codegen::platformNumber(p->amount);

                if (hardcode) {
                    output += fmt::format(format_strings::pad_definition, fmt::arg("hardcode", hardcode));
                } else {
                    output += "\n        GEODE_UNIMPLEMENTED_PAD";
                }
                continue;
            } else if (auto fn = field.get_as<OutOfLineField>()) {
                fb = &fn->beginning;
            } else if (auto fn = field.get_as<FunctionBindField>()) {
                fb = &fn->beginning;

                if (!codegen::platformNumber(fn->binds)) {
                    used_format = format_strings::error_definition;

                    if (fb->type != FunctionType::Normal)
                        continue;
                }
            }

            output += fmt::format(used_format,
                fmt::arg("virtual", str_if("virtual ", fb->is_virtual)),
                fmt::arg("static", str_if("static ", fb->is_static)),
                fmt::arg("class_name", cls.name),
                fmt::arg("const", str_if(" const ", fb->is_const)),
                fmt::arg("function_name", fb->name),
                fmt::arg("index", field.field_id),
                fmt::arg("parameters", codegen::getParameters(*fb)),
                fmt::arg("return_type", fb->ret.name),
                fmt::arg("docs", fb->docs)
            );
        }

        // if (hasClass)
        output += ::format_strings::class_end;

        // queued.pop_front();
    }

    return output;
}
