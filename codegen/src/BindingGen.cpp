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
private:
    [[deprecated("{class_name}::{function_name} not implemented")]]
    /**
{docs}{docs_addresses}     */
    {static}{virtual}{return_type} {function_name}({parameters}){const};
public:
)GEN";

    char const* structor_definition = R"GEN(
    /**
{docs}{docs_addresses}     */
    {function_name}({parameters});
)GEN";
    
    // requires: type, member_name, array
    char const* member_definition = R"GEN({private}    {type} {member_name};{public}
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

template <class T>
std::string generateAddressDocs(T const& f, PlatformNumber pn) {
    std::string ret;

    for (auto platform : {Platform::Mac, Platform::Windows, Platform::iOS, Platform::Android}) {
        auto status = codegen::getStatusWithPlatform(platform, f);

        if (status == BindStatus::NeedsBinding) {
            ret += fmt::format("     * @note[short] {}: 0x{:x}\n", 
                nameForPlatform(platform),
                codegen::platformNumberWithPlatform(platform, pn)
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

std::string generateBindingHeader(Root const& root, ghc::filesystem::path const& singleFolder) {
    std::string output;

    {
        std::string filename = "Standalones.hpp";
        output += fmt::format(format_strings::binding_include, 
            fmt::arg("file_name", filename)
        );

        std::string single_output;
        single_output += format_strings::class_includes;

        for (auto& f : root.functions) {
            if (codegen::getStatus(f) == BindStatus::Missing) continue;

            FunctionProto const* fb = &f.prototype;
            char const* used_format = format_strings::function_definition;

            std::string addressDocs = generateAddressDocs(f, f.binds);
            std::string docs = generateDocs(fb->docs);

            single_output += fmt::format(used_format,
                fmt::arg("virtual", ""),
                fmt::arg("static", ""),
                fmt::arg("class_name", ""),
                fmt::arg("const", ""),
                fmt::arg("function_name", fb->name),
                fmt::arg("parameters", codegen::getParameters(*fb)),
                fmt::arg("return_type", fb->ret.name),
                fmt::arg("docs_addresses", addressDocs),
                fmt::arg("docs", docs)
            );

        }

        writeFile(singleFolder / filename, single_output);
    }

   	for (auto& cls : root.classes) {
        if (is_cocos_class(cls.name))
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
            fmt::format(" : public {}", fmt::join(cls.superclasses, ", public ")),
            !cls.superclasses.empty()
        );

        single_output += fmt::format(::format_strings::class_start,
            fmt::arg("class_name", cls.name),
            fmt::arg("base_classes", supers)//,
            // fmt::arg("hidden", str_if("GEODE_HIDDEN ", (codegen::platform & (Platform::Mac | Platform::iOS)) != Platform::None))
        );

        // what.
        if (!cls.superclasses.empty()) {
            single_output += fmt::format(
                is_cocos_class(cls.superclasses[0]) 
                    ? format_strings::custom_constructor_cutoff
                    : format_strings::custom_constructor,
                fmt::arg("class_name", cls.name),
                fmt::arg("first_base", cls.superclasses[0])
            );
        }

        bool unimplementedField = false;
        for (auto field : cls.fields) {
            if (codegen::getStatus(field) == BindStatus::Missing) continue;

            MemberFunctionProto* fb;
            char const* used_format = format_strings::function_definition;

            std::string addressDocs;

            if (auto i = field.get_as<InlineField>()) {
                single_output += "\t" + i->inner + "\n";
                continue;
            } else if (auto m = field.get_as<MemberField>()) {
                single_output += fmt::format(format_strings::member_definition,
                    fmt::arg("private", unimplementedField ? "private:\n" : ""),
                    fmt::arg("public", unimplementedField ? "\npublic:" : ""),
                    fmt::arg("type", m->type.name),
                    fmt::arg("member_name", m->name + str_if(fmt::format("[{}]", m->count), m->count))
                );
                continue;
            } else if (auto p = field.get_as<PadField>()) {
                auto hardcode = codegen::platformNumber(p->amount);

                if (hardcode > 0) {
                    single_output += fmt::format(format_strings::pad_definition, fmt::arg("hardcode", hardcode));
                } 
                else if (hardcode == 0) {
                    single_output += "    // no padding\n";
                }
                else {
                    unimplementedField = true;
                }
                continue;
            } else if (auto fn = field.get_as<OutOfLineField>()) {
                fb = &fn->prototype;
                addressDocs = "     * @note[short] Out of line\n";

            } else if (auto fn = field.get_as<FunctionBindField>()) {
                fb = &fn->prototype;

                if (codegen::platformNumber(fn->binds) == -1 && codegen::getStatus(field) != BindStatus::Binded) {
                    used_format = format_strings::error_definition;

                    if (fb->type != FunctionType::Normal)
                        continue;
                }

                addressDocs = generateAddressDocs(field, fn->binds);
            }

            std::string docs = generateDocs(fb->docs);

            single_output += fmt::format(used_format,
                fmt::arg("virtual", str_if("virtual ", fb->is_virtual)),
                fmt::arg("static", str_if("static ", fb->is_static)),
                fmt::arg("class_name", cls.name),
                fmt::arg("const", str_if(" const ", fb->is_const)),
                fmt::arg("function_name", fb->name),
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
