#include "SharedGen.hpp"
#include <iostream>
#include <set>

using std::set;

namespace format_strings {
    // requires: base_classes, class_name
    char const* class_start = "class {class_name}{base_classes} {{\n";
    
    char const* function_address_definition = " = mac {mac_address}, win {win_address}, ios {ios_address}";

    // requires: static, virtual, return_type, function_name, raw_parameters, const
    char const* function_definition = "\t{static}{virtual}{return_type} {function_name}({raw_params}){const}{address};\n";

    char const* outofline_definition = "\t{static}{virtual}{return_type} {function_name}({raw_params}){const} {definition}\n";

    char const* structor_definition = "\t{function_name}({raw_params}){address};\n";
    

	char const* member_address_definition = " = mac {mac_address}, win {win_address}, android {android_address}";

    char const* member_definition = "\t{type} {member_name}{array};\n";
    char const* hardcode_definition = "\t{type} {member_name}{address};\n";

    char const* pad_definition = "\tPAD{address};\n";


    char const* inline_definition = "\tinline{inline}\n";

    char const* class_end = "}\n";
}

int main(int argc, char** argv) {
	string output("");
    Root root = CacShare::init(argc, argv);
    vector<ClassDefinition> sorted_classes;
    for (auto& [name, cd] : root.classes) {
    	sorted_classes.push_back(cd);
    }

    sort(sorted_classes.begin(), sorted_classes.end(), 
    	[](const auto& a, const auto& b) -> bool { 
	    return a.name < b.name; 
	});

   	for (auto& cd : sorted_classes) {
        output += fmt::format(format_strings::class_start,
            fmt::arg("class_name", cd.name),
            fmt::arg("base_classes", CacShare::formatBases(cd.superclasses))
        );

        bool inlinePad = false;
        for (auto i : cd.inlines) {
        	inlinePad = true;
        	output += fmt::format(format_strings::inline_definition,
	            fmt::arg("inline", i.inlined)
	        );
        }
        if (inlinePad) output += "\n";

        bool outoflinePad = false;
        for (auto f : cd.functions) {
            if (!f.is_defined) continue;
            outoflinePad = true;
            output += fmt::format(format_strings::outofline_definition,
            	fmt::arg("virtual", f.function_type == kVirtualFunction ? "virtual " : ""),
                fmt::arg("static", f.function_type == kStaticFunction ? "static " : ""),
                fmt::arg("return_type", CacShare::getReturn(f)),
                fmt::arg("function_name", f.name),
                fmt::arg("raw_params", CacShare::formatRawParams(f.args, f.argnames)),
                fmt::arg("const", f.is_const ? " const" : ""),
                fmt::arg("definition", f.definition)
        	);
        }
        if (outoflinePad) output += "\n";

        bool functionPad = false;
        for (auto f : cd.functions) {
            if (f.is_defined) continue;
            functionPad = true;
        	char const* used_format;
        	switch (f.function_type) {
                case kDestructor:
                case kConstructor:
                    used_format = format_strings::structor_definition;
                    break;
                default:
               		used_format = format_strings::function_definition;
                    break;
            }
            auto address = fmt::format(format_strings::function_address_definition,
            	fmt::arg("mac_address", f.binds[kMac].size() ? f.binds[kMac] : "0x0"),
                fmt::arg("win_address", f.binds[kWindows].size() ? f.binds[kWindows] : "0x0"),
                fmt::arg("ios_address", f.binds[kIos].size() ? f.binds[kIos] : "0x0")
            );
        	output += fmt::format(used_format,
                fmt::arg("virtual", f.function_type == kVirtualFunction ? "virtual " : ""),
                fmt::arg("static", f.function_type == kStaticFunction ? "static " : ""),
                fmt::arg("return_type", f.return_type),
                fmt::arg("function_name", f.name),
                fmt::arg("raw_params", CacShare::formatRawParams(f.args, f.argnames)),
                fmt::arg("const", f.is_const ? " const" : ""),
                fmt::arg("address", address)
            );
        }
        if (functionPad) output += "\n";

        bool memberPad = false;
        for (auto m : cd.members) {
        	memberPad = true;
        	char const* used_format;
        	switch (m.member_type) {
                case kDefault:
                	used_format = format_strings::member_definition;
                	break;
                case kHardcode:
                	used_format = format_strings::hardcode_definition;
                	break;
                case kPad:
                	used_format = format_strings::pad_definition;
                	break;
            }
            auto address = fmt::format(format_strings::member_address_definition,
            	fmt::arg("mac_address", m.hardcodes[kMacMember].size() ? m.hardcodes[kMacMember] : "0x0"),
                fmt::arg("win_address", m.hardcodes[kWindowsMember].size() ? m.hardcodes[kWindowsMember] : "0x0"),
                fmt::arg("android_address", m.hardcodes[kAndroidMember].size() ? m.hardcodes[kAndroidMember] : "0x0")
            );
        	output += fmt::format(used_format,
                fmt::arg("type", m.type),
                fmt::arg("member_name", m.name.substr(m.member_type == kHardcode ? 2 : 0, m.name.size())),
                fmt::arg("address", address),
                fmt::arg("array", CacShare::getArray(m.count)) //why is this not tied to member
            );
        }
        if (memberPad) output += "\n";

        output = output.substr(0, output.size()-1);
        output += format_strings::class_end;
        output += "\n";

        // queued.pop_front();
    }

    CacShare::writeFile(output);
}
