#include <SharedGen.hpp>
#include <iostream>
#include <set>

using std::set;

namespace format_strings {
    // requires: base_classes, class_name
    char const* class_start = "class {class_name}{colon}{base_classes} {{\n";
    
    char const* address_definition = " = {address}";

    // requires: static, virtual, return_type, function_name, raw_parameters, const
    char const* function_definition = "\t{static}{virtual}{return_type} {function_name}({parameters}){const_whitespace}{const}{address};\n";

    char const* outofline_definition = "\t{static}{virtual}{return_type} {function_name}({parameters}){const_whitespace}{const} {definition}\n";

    char const* structor_definition = "\t{function_name}({parameters}){address};\n";
    

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
            fmt::arg("colon", cd.superclasses.size() ? " : " : ""),
            fmt::arg("base_classes", fmt::join(cd.superclasses, ", "))
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
            CacShare::editArguments(f);
            outoflinePad = true;
            output += fmt::format(format_strings::outofline_definition,
            	fmt::arg("virtual", f.function_type == kVirtualFunction ? "virtual " : ""),
                fmt::arg("static", f.function_type == kStaticFunction ? "static " : ""),
                fmt::arg("return_type", f.return_type),
                fmt::arg("function_name", CacShare::getFunctionName(f)),
                fmt::arg("parameters", CacShare::getParameters(f)),
                fmt::arg("const", CacShare::getConst(f)),
				fmt::arg("const_whitespace", CacShare::getConstWhitespace(f)),
                fmt::arg("definition", f.definition)
        	);
        }
        if (outoflinePad) output += "\n";

        bool functionPad = false;
        for (auto f : cd.functions) {
            if (f.is_defined) continue;
            CacShare::editArguments(f);
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
            string address;
            if (f.binds[kMac].size() || f.binds[kWindows].size() || f.binds[kIos].size()) {
            	vector<string> addresses;
            	if (f.binds[kMac].size()) addresses.push_back("mac " + f.binds[kMac]);
            	if (f.binds[kWindows].size()) addresses.push_back("win " + f.binds[kWindows]);
            	if (f.binds[kIos].size()) addresses.push_back("ios " + f.binds[kIos]);
            	address = fmt::format(" = {}", fmt::join(addresses, ", "));
            }
        	output += fmt::format(used_format,
                fmt::arg("virtual", f.function_type == kVirtualFunction ? "virtual " : ""),
                fmt::arg("static", f.function_type == kStaticFunction ? "static " : ""),
                fmt::arg("return_type", f.return_type),
                fmt::arg("function_name", CacShare::getFunctionName(f)),
                fmt::arg("parameters", CacShare::getParameters(f)),
                fmt::arg("const", CacShare::getConst(f)),
				fmt::arg("const_whitespace", CacShare::getConstWhitespace(f)),
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
            string address;
            if (m.hardcodes[kMacMember].size() || m.hardcodes[kWindowsMember].size() || m.hardcodes[kAndroidMember].size()) {
            	vector<string> addresses;
            	if (m.hardcodes[kMacMember].size()) addresses.push_back("mac " + m.hardcodes[kMacMember]);
            	if (m.hardcodes[kWindowsMember].size()) addresses.push_back("win " + m.hardcodes[kWindowsMember]);
            	if (m.hardcodes[kAndroidMember].size()) addresses.push_back("android " + m.hardcodes[kAndroidMember]);
            	address = fmt::format(" = {}", fmt::join(addresses, ", "));
            }
            string array = m.count > 0 ? fmt::format("[{}]", m.count) : "";
        	output += fmt::format(used_format,
                fmt::arg("type", m.type),
                fmt::arg("member_name", m.name),
                fmt::arg("address", address),
                fmt::arg("array", array) //why is this not tied to member
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
