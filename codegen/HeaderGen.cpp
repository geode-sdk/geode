#include "SharedGen.hpp"
#include <iostream>
#include <set>

using std::set;

namespace format_strings {
	char const* class_predeclare = "class {class_name};\n";
    // requires: base_classes, class_name
    char const* class_start = R"GEN(
class {class_name}{base_classes} {{
public:
)GEN";

    char const* function_definition = R"GEN(
    {docs}GEODE_CODEGEN_DLL {static}{virtual}{return_type} {function_name}({parameters}){const_whitespace}{const};
)GEN";

    char const* error_definition = R"GEN(
    template <bool T=false>
    {static}{return_type} {function_name}({parameters}){const_whitespace}{const}{{
        static_assert(T, "Implement {class_name}::{function_name}");
    }}
)GEN";

    char const* structor_definition = R"GEN(
        GEODE_CODEGEN_DLL {function_name}({parameters});)GEN";
    
    // requires: type, member_name, array
    char const* member_definition = R"GEN(
        {type} {member_name}{array};)GEN";

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
}

void sortClass(Root& r, ClassDefinition* c, set<ClassDefinition*>& looked, vector<ClassDefinition*>& ordered) {
	if (c->name.find("DS_Dictionary") != string::npos) return;
	if (c->name.find("cocos2d") != string::npos) return; // cocos headers exist already
    if (looked.find(c) == looked.end()) {
    	looked.insert(c);
        for (string j : c->depends) {
        	if (j.find("cocos2d") != string::npos) continue;
        	if (r.classes.count(c->name) == 0) {
		        cacerr("Expected class definition for %s\n", c->name.c_str());
		    }
		    else {
		    	if (r.classes.find(j) == r.classes.end()) {
		    		cacerr("Create class definition for %s\n", j.c_str());
		    	}
		    	else sortClass(r, &r.classes.at(j), looked, ordered);
		    }    
        }
        ordered.push_back(c);
    }
}

int main(int argc, char** argv) {
	set<ClassDefinition*> looked;
	vector<ClassDefinition*> ordered;

    string output = "#pragma once\n";
    Root root = CacShare::init(argc, argv);

    for (auto& [name, c] : root.classes) {
        sortClass(root, &c, looked, ordered);
    }

    for (auto& cp : ordered) {
        if (cp->name != "") {
            output += fmt::format(format_strings::class_predeclare,
                fmt::arg("class_name", cp->name)
            );
        }
    }


   	for (auto& cp : ordered) {
   		auto& cd = *cp;

        vector<string> base_strings;
        for (string i : cd.superclasses) {
            base_strings.push_back(fmt::format("public {}", i));
        }

        string bases = cd.superclasses.size() > 0 ? fmt::format(" : {}", fmt::join(base_strings, ", ")) : "";

        output += fmt::format(format_strings::class_start,
            fmt::arg("class_name", cd.name),
            fmt::arg("base_classes", bases)
        );

        for (auto i : cd.inlines) {
        	output += "\t" + i.inlined + "\n";
        }

        for (auto f : cd.functions) {
            CacShare::editArguments(f);
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

            if (f.binds[CacShare::platform].size() == 0 && !f.is_defined) {
                //continue;
                // ok new idea

                if (f.function_type == kDestructor || f.function_type == kConstructor)
                    continue;
                used_format = format_strings::error_definition;
            }

        	output += fmt::format(used_format,
                fmt::arg("virtual", f.function_type == kVirtualFunction ? "virtual " : ""),
                fmt::arg("static", f.function_type == kStaticFunction ? "static " : ""),
                fmt::arg("class_name", CacShare::getClassName(f)),
				fmt::arg("const", CacShare::getConst(f)),
				fmt::arg("const_whitespace", CacShare::getConstWhitespace(f)),
				fmt::arg("function_name", CacShare::getFunctionName(f)),
				fmt::arg("index", CacShare::getIndex(f)),
				fmt::arg("parameters", CacShare::getParameters(f)),
                fmt::arg("return_type", f.return_type),
                fmt::arg("docs", f.docs)
            );
        }

        for (auto m : cd.members) {
            string hardcode;
            switch (CacShare::platform) {
                case kIos:
                    hardcode = m.hardcodes[kIosMember];
                    break;
                case kAndroid:
                    hardcode = m.hardcodes[kAndroidMember];
                    break;
                case kMac:
                    hardcode = m.hardcodes[kMacMember];
                    break;
                case kWindows:
                    hardcode = m.hardcodes[kWindowsMember];
                    break;
            }
            if (m.member_type == kHardcode && hardcode.size() == 0)
                continue; // Not Implemented on platform

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
                	if (hardcode.size() == 0) used_format = format_strings::unimplemented_definition;
                	break;
            }
            
            string array = m.count > 0 ? fmt::format("[{}]", m.count) : "";
            string member_name = m.name.substr(m.member_type == kHardcode && m.name.substr(0, 2) == "m_" ? 2 : 0, m.name.size());
        	output += fmt::format(used_format,
                fmt::arg("type", m.type),
                fmt::arg("member_name", member_name),
                fmt::arg("hardcode", hardcode),
                fmt::arg("array", array) //why is this not tied to member
            );
        }

        // if (hasClass)
        output += format_strings::class_end;

        // queued.pop_front();
    }

    CacShare::writeFile(output);
}
