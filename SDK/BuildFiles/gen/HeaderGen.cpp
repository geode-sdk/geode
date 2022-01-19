#include "SharedGen.hpp"
#include <iostream>
#include <set>

using std::set;

namespace format_strings {
    char const* header_start = R"CAC(
// 
// Copyright camila314 & alk1m123 2022. 
// Automatically generated file
//
#pragma once
#include <HeaderBase.hpp>
)CAC";

	char const* class_predeclare = "struct {class_name};\n";
    // requires: base_classes, class_name
    char const* class_start = R"CAC(
struct {class_name}{base_classes} {{
)CAC";
    
    // requires: static, virtual, return_type, function_name, raw_parameters, const
    char const* function_definition = "\t{static}{virtual}{return_type} {function_name}({raw_arg_types}){const};\n";

    char const* structor_definition = "\t{function_name}({raw_arg_types});\n";
    
    // requires: type, member_name, array
    char const* member_definition = "\t{type} {member_name}{array};\n";

    // requires: hardcode_macro, type, member_name, hardcode
    char const* hardcode_definition = "\tCLASSPARAM({type}, {member_name}, {hardcode});\n";

    char const* class_end = "};\n";
}

void sortClass(Root& r, ClassDefinition* c, set<ClassDefinition*>& looked, vector<ClassDefinition*>& ordered) {
	if (c->name.find("cocos2d::") != string::npos) return; // cocos headers exist already
    if (looked.find(c) == looked.end()) {
    	looked.insert(c);
        for (string j : c->superclasses) {
        	if (j.find("cocos2d::") != string::npos) continue;
        	if (r.classes.count(c->name) == 0) {
		        cacerr("Expected class definition for %s\n", c->name.c_str());
		    }
		    else sortClass(r, &r.classes.at(j), looked, ordered);    
        }
        ordered.push_back(c);
    }
}

int main(int argc, char** argv) {
	set<ClassDefinition*> looked;
	vector<ClassDefinition*> ordered;
    string output(format_strings::header_start);
    Root root = CacShare::init(argc, argv);

    for (auto& [name, c] : root.classes) {
        sortClass(root, &c, looked, ordered);
    }

    for (auto& cp : ordered) {
    	output += fmt::format(format_strings::class_predeclare,
            fmt::arg("class_name", cp->name)
        );
    }

   	for (auto& cp : ordered) {
   		auto& cd = *cp;


        output += fmt::format(format_strings::class_start,
            fmt::arg("class_name", cd.name),
            fmt::arg("base_classes", CacShare::formatBases(cd.superclasses))
        );

        for (auto i : cd.inlines) {
        	output += "\t" + i.inlined + "\n";
        }

        for (auto f : cd.functions) {
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
        	output += fmt::format(used_format,
                fmt::arg("virtual", f.function_type == kVirtualFunction ? "virtual " : ""),
                fmt::arg("static", f.function_type == kStaticFunction ? "static " : ""),
                fmt::arg("return_type", CacShare::getReturn(f)),
                fmt::arg("function_name", f.name),
                fmt::arg("raw_arg_types", CacShare::formatRawArgTypes(f.args)),
                fmt::arg("const", f.is_const ? " const" : "")
            );
        }
        for (auto m : cd.members) {
        	char const* used_format;
        	if (m.hardcode) used_format = format_strings::hardcode_definition;
        	else used_format = format_strings::member_definition;
        	output += fmt::format(used_format,
                fmt::arg("type", m.type),
                fmt::arg("member_name", m.name.substr(m.hardcode ? 2 : 0, m.name.size())),
                fmt::arg("hardcode", CacShare::getHardcode(m)),
                fmt::arg("array", CacShare::getArray(m.count)) //why is this not tied to member
            );
        }

        output += format_strings::class_end;

        // queued.pop_front();
    }

    CacShare::writeFile(output);
}
