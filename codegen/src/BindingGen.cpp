#include "Shared.hpp"
#include <iostream>
#include <set>

namespace { namespace format_strings {
    // requires: base_classes, class_name
    char const* binding_include = R"GEN(#include "binding/{file_name}"
)GEN";

    char const* class_includes = R"GEN(#pragma once
#include <stdexcept>
#include <Geode/platform/platform.hpp>
#include <Geode/c++stl/gdstdlib.hpp>
#include <cocos2d.h>
#include <cocos-ext.h>
#include <Geode/GeneratedPredeclare.hpp>
#include <Geode/Enums.hpp>
#include <Geode/utils/SeedValue.hpp>

)GEN";

    char const* class_no_includes = R"GEN(#pragma once
#include <Geode/platform/platform.hpp>
#include <stdexcept>

)GEN";
    
    char const* class_include_prereq = R"GEN(#include "{file_name}"
)GEN";

    char const* class_start = R"GEN(
class {class_name}{base_classes} {{
public:
    static constexpr auto CLASS_NAME = "{class_name}";
)GEN";

	char const* custom_constructor = R"GEN(    GEODE_CUSTOM_CONSTRUCTOR_GD({class_name}, {first_base})
)GEN";

	char const* custom_constructor_cutoff = R"GEN(    GEODE_CUSTOM_CONSTRUCTOR_CUTOFF({class_name}, {first_base})
)GEN";

    char const* function_definition = R"GEN(
    /**
{docs}{docs_addresses}     */
    {static}{virtual}{return_type} {function_name}({parameters}){const};
)GEN";

    char const* error_definition = R"GEN(    
    #ifdef GEODE_WARN_INCORRECT_MEMBERS
    [[deprecated("Function is not implemented - will throw at runtime!!!")]]
    #endif
    /**
{docs}{docs_addresses}     */
    {static}{return_type} {function_name}({parameters}){const}{{
        throw std::runtime_error("Use of undefined function " + GEODE_PRETTY_FUNCTION);
    }}
)GEN";

    char const* error_definition_virtual = R"GEN(    
    #ifdef GEODE_WARN_INCORRECT_MEMBERS
    [[deprecated("Use of undefined virtual function - will crash at runtime!!!")]]
    #endif
    /**
{docs}{docs_addresses}     */
    {virtual}{return_type} {function_name}({parameters}){const}{{
        #ifdef GEODE_NO_UNDEFINED_VIRTUALS
        static_assert(false, "Undefined virtual function - implement in GeometryDash.bro");
        #endif
        throw std::runtime_error("Use of undefined virtual function " + GEODE_PRETTY_FUNCTION);
    }}
)GEN";

    char const* warn_offset_member = R"GEN(
    #ifdef GEODE_WARN_INCORRECT_MEMBERS
    [[deprecated("Member placed incorrectly - will crash at runtime!!!")]]
    #endif
)GEN";

    char const* structor_definition = R"GEN(
    /**
{docs}{docs_addresses}     */
    {function_name}({parameters});
)GEN";
    
    // requires: type, member_name, array
    char const* member_definition = R"GEN(    {type} {member_name};
)GEN";

    char const* pad_definition = R"GEN(    GEODE_PAD({hardcode});
)GEN";

    char const* class_end = R"GEN(};
)GEN";
}}

inline std::string nameForPlatform(Platform platform) {
    switch (platform) {
        case Platform::Mac: return "MacOS";
        case Platform::Windows: return "Windows";
        case Platform::iOS: return "iOS";
        case Platform::Android: return "Android";
        default: // unreachable
            return "Windows";
    }
}


std::string generateAddressDocs(Field const& field, FunctionBindField* fn) {
    std::string ret;

    for (auto platform : {Platform::Mac, Platform::Windows, Platform::iOS, Platform::Android}) {
        auto status = codegen::getStatusWithPlatform(platform, field);

        if (status == BindStatus::NeedsBinding) {
            ret += fmt::format("     * @note[short] {}: 0x{:x}\n", 
                nameForPlatform(platform),
                codegen::platformNumberWithPlatform(platform, fn->binds)
            );
        }
        else if (status == BindStatus::Binded) {
            ret += fmt::format("     * @note[short] {}\n", 
                nameForPlatform(platform)
            );
        }
        
    }

    return ret;
}

std::string generateDocs(std::string const& docs) {
    if (docs.size() < 7) return "";
    auto ret = docs.substr(1, docs.size() - 6); // i hate this but idk how to generalize

    for (auto next = ret.find("        "); next != std::string::npos; next = ret.find("        ")) {
        ret.replace(next, 8, "     * ");
    }
        
    return ret;
}

std::string generateBindingHeader(Root& root, ghc::filesystem::path const& singleFolder) {
    std::string output;

   	for (auto& cls : root.classes) {
        if (can_find(cls.name, "cocos2d"))
            continue;

        std::string filename = (codegen::getUnqualifiedClassName(cls.name) + ".hpp");
        output += fmt::format(format_strings::binding_include, 
            fmt::arg("file_name", filename)
        );

        std::string single_output;
        if (cls.name != "GDString") {
            single_output += format_strings::class_includes;
        } else {
            single_output += format_strings::class_no_includes;
        }

        for (auto dep : cls.depends) {
            if (can_find(dep, "cocos2d::")) continue;

            std::string depfilename = (codegen::getUnqualifiedClassName(dep) + ".hpp");

            single_output += fmt::format(format_strings::class_include_prereq, fmt::arg("file_name", depfilename));
        }

        std::string supers = str_if(
            fmt::format(" : public {}", fmt::join(cls.superclasses, ", ")),
            !cls.superclasses.empty()
        );

        single_output += fmt::format(::format_strings::class_start,
            fmt::arg("class_name", cls.name),
            fmt::arg("base_classes", supers)
        );

        // what.
        if (!cls.superclasses.empty()) {
            single_output += fmt::format(
                can_find(cls.superclasses[0], "cocos2d") 
                    ? format_strings::custom_constructor_cutoff
                    : format_strings::custom_constructor,
                fmt::arg("class_name", cls.name),
                fmt::arg("first_base", cls.superclasses[0])
            );
        }

        bool unimplementedField = false;
        for (auto field : cls.fields) {
            FunctionBegin* fb;
            char const* used_format = format_strings::function_definition;

            std::string addressDocs;

            if (auto i = field.get_as<InlineField>()) {
                single_output += "\t" + i->inner + "\n";
                continue;
            } else if (auto m = field.get_as<MemberField>()) {
                if (unimplementedField) single_output += format_strings::warn_offset_member;
                single_output += fmt::format(format_strings::member_definition,
                    fmt::arg("type", m->type.name),
                    fmt::arg("member_name", m->name + str_if(fmt::format("[{}]", m->count), m->count))
                );
                continue;
            } else if (auto p = field.get_as<PadField>()) {
                auto hardcode = codegen::platformNumber(p->amount);

                if (hardcode) {
                    single_output += fmt::format(format_strings::pad_definition, fmt::arg("hardcode", hardcode));
                } else {
                    unimplementedField = true;
                }
                continue;
            } else if (auto fn = field.get_as<OutOfLineField>()) {
                fb = &fn->beginning;
                addressDocs = "     * @note[short] Out of line\n";

            } else if (auto fn = field.get_as<FunctionBindField>()) {
                fb = &fn->beginning;

                if (!codegen::platformNumber(fn->binds)) {
                    used_format = format_strings::error_definition;

                    if (fb->is_virtual)
                        used_format = format_strings::error_definition_virtual;

                    if (fb->type != FunctionType::Normal)
                        continue;
                }

                addressDocs = generateAddressDocs(field, fn);
            }

            std::string docs = generateDocs(fb->docs);

            single_output += fmt::format(used_format,
                fmt::arg("virtual", str_if("virtual ", fb->is_virtual)),
                fmt::arg("static", str_if("static ", fb->is_static)),
                fmt::arg("class_name", cls.name),
                fmt::arg("const", str_if(" const ", fb->is_const)),
                fmt::arg("function_name", fb->name),
                fmt::arg("index", field.field_id),
                fmt::arg("parameters", codegen::getParameters(*fb)),
                fmt::arg("return_type", fb->ret.name),
                fmt::arg("docs_addresses", addressDocs),
                fmt::arg("docs", docs)
            );
        }

        // if (hasClass)
        single_output += ::format_strings::class_end;

        writeFile(singleFolder / filename, single_output);
    }

    return output;
}
