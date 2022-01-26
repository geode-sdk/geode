#include "SharedGen.hpp"
#include <iostream>

namespace format_strings {
    // requires: class_name
    char const* interface_start = R"CAC(
template<template <auto, typename> class D = BlankBase, typename UUID = void>
struct ${class_name} : {raw_class_name}, ModifierBase {{
    ${class_name}(const ${class_name}& c) : {class_name}(c) {{}}
    ${class_name}() = delete;

    GEODE_NOINLINE static inline auto& getAdditionalFields() {{
    	static std::unordered_map<uintptr_t, container_t<>*> ret;
    	return ret;
    }}
    GEODE_NOINLINE static inline auto& getOriginalDestructor() {{
    	static uintptr_t ret;
    	return ret;
    }}
    static void fieldCleanup(uintptr_t self) {{
    	const uintptr_t begin = self + sizeof(${class_name});
    	const uintptr_t end = self + sizeof(D<0, UUID>);
    	for (uintptr_t i = begin; i < end; ++i) {{
    		if (getAdditionalFields().find(i) != getAdditionalFields().end()) {{
    			delete getAdditionalFields().at(i);
    			getAdditionalFields().erase(i);
    		}}
    	}}
    	reinterpret_cast<void(*)(uintptr_t)>(getOriginalDestructor())(self);
    }}
)CAC";

    char const* declare_structor = R"CAC(
    GEODE_DUPABLE void {function_name}({raw_args}) {{
        reinterpret_cast<void(*)(decltype(this){arg_types})>(temp_name_find_better::address{global_index})(this{parameters});
    }})CAC";

    char const* apply_start = R"CAC(
    static bool _apply() {
)CAC";
    // requires: index, class_name, arg_types, function_name, raw_arg_types, non_virtual
    char const* apply_function_member = R"CAC(
    	using baseType{global_index} = temp_name_find_better::ret{global_index}({class_name}::*)({raw_arg_types}) {const};
		constexpr auto baseAddress{global_index} = (baseType{global_index})(&{class_name}::{function_name});
		using derivedType{global_index} = temp_name_find_better::ret{global_index}(D<baseAddress{global_index}, UUID>::*)({raw_arg_types}) {const};
		constexpr auto derivedAddress{global_index} = (derivedType{global_index})(&D<baseAddress{global_index}, UUID>::{function_name});
        if (baseAddress{global_index} != derivedAddress{global_index}) {{
        	Interface::get()->logInfo("Adding hook at function {class_name}::{function_name}", Severity::Debug);
            Interface::get()->addHook("{class_name}::{function_name}", (void*)temp_name_find_better::address{global_index}(), (void*)addresser::get{non_virtual}Virtual(derivedAddress{global_index}));
        }}
)CAC";

	char const* apply_function_structor = R"CAC(
    	using baseType{global_index} = temp_name_find_better::ret{global_index}(${class_name}::*)({raw_arg_types}) {const};
		constexpr auto baseAddress{global_index} = (baseType{global_index})(&${class_name}::{function_name});
		using derivedType{global_index} = temp_name_find_better::ret{global_index}(D<baseAddress{global_index}, UUID>::*)({raw_arg_types}) {const};
		constexpr auto derivedAddress{global_index} = (derivedType{global_index})(&D<baseAddress{global_index}, UUID>::{function_name});
        if (baseAddress{global_index} != derivedAddress{global_index}) {{
        	Interface::get()->logInfo("Adding hook at function {class_name}::{function_name}", Severity::Debug);
            Interface::get()->addHook("{class_name}::{function_name}", (void*)temp_name_find_better::address{global_index}(), (void*)addresser::get{non_virtual}Virtual(derivedAddress{global_index}));
        }}
)CAC";

	char const* apply_function_static = R"CAC(
		using baseType{global_index} = temp_name_find_better::ret{global_index}(*)({raw_arg_types});
		constexpr auto baseAddress{global_index} = (baseType{global_index})(&{class_name}::{function_name});
		using derivedType{global_index} = temp_name_find_better::ret{global_index}(*)({raw_arg_types});
		constexpr auto derivedAddress{global_index} = (derivedType{global_index})(&D<baseAddress{global_index}, UUID>::{function_name});
        if (baseAddress{global_index} != derivedAddress{global_index}) {{
        	Interface::get()->logInfo("Adding hook at function {class_name}::{function_name}", Severity::Debug);
            Interface::get()->addHook("{class_name}::{function_name}", (void*)temp_name_find_better::address{global_index}(), (void*)addresser::getNonVirtual(derivedAddress{global_index}));
        }}
)CAC";

    char const* apply_end = R"CAC(
        return true;
    }
)CAC";
}


int main(int argc, char** argv) {
    auto root = CacShare::init(argc, argv);
    string output;

    for (auto& [name, c] : root.classes) {
        string unqualifiedName = CacShare::toUnqualified(name);

        output += fmt::format(format_strings::interface_start, fmt::arg("class_name", unqualifiedName), fmt::arg("raw_class_name", name));

        for (auto& f : c.functions) {
        	if (f.name == "colorAllCharactersTo") cout << "colorAllCharactersTo " << CacShare::functionDefined(f) << endl;
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
				        fmt::arg("global_index",f.hash()),
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
