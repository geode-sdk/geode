#include "SharedGen.hpp"
#include <iostream>

namespace format_strings {
    // requires: class_name
    char const* interface_start = R"CAC(
template<template <auto orig> class D = __unitSpec>
struct ${class_name} : {raw_class_name}, InterfaceBase {{
    ${class_name}(const ${class_name}& c) : {class_name}(c) {{}}
    ${class_name}() = delete;

    GEODE_NOINLINE static inline auto& getAdditionalFields() {{
    	static std::unordered_map<size_t, container_t<>*> ret;
    	return ret;
    }}
    GEODE_NOINLINE static inline auto& getOriginalDestructor() {{
    	static size_t ret;
    	return ret;
    }}
    static void fieldCleanup(size_t self) {{
    	const size_t begin = self + sizeof(${class_name});
    	const size_t end = self + sizeof(D<0>);
    	for (size_t i = begin; i < end; ++i) {{
    		if (getAdditionalFields().find(i) != getAdditionalFields().end()) {{
    			delete getAdditionalFields().at(i);
    			getAdditionalFields().erase(i);
    		}}
    	}}
    	reinterpret_cast<void(*)(size_t)>(getOriginalDestructor())(self);
    }}
)CAC";

    // requires: index, address
    char const* predefine_address = R"CAC(
	GEODE_NOINLINE static inline auto address{index}() {{
		static auto ret = {address};
		return ret;
	}})CAC";

    // requires: index, return
    char const* predefine_return = R"CAC(
    using ret{index} = {return};)CAC";


    char const* declare_structor = R"CAC(
    GEODE_DUPABLE void {function_name}({raw_args}) {{
        reinterpret_cast<void(*)(decltype(this){arg_types})>(address{index})(this{parameters});
    }})CAC";

    char const* apply_start = R"CAC(
    static bool _apply() {
)CAC";
    // requires: index, class_name, arg_types, function_name, raw_arg_types, non_virtual
    char const* apply_function_member = R"CAC(
    	using baseType{index} = ret{index}({class_name}::*)({raw_arg_types}) {const};
		constexpr auto baseAddress{index} = (baseType{index})(&{class_name}::{function_name});
		using derivedType{index} = ret{index}(D<baseAddress{index}>::*)({raw_arg_types}) {const};
		constexpr auto derivedAddress{index} = (derivedType{index})(&D<baseAddress{index}>::{function_name});
        if (baseAddress{index} != derivedAddress{index}) {{
            Interface::get()->addHook((void*)address{index}(), (void*)FunctionScrapper::addressOf{non_virtual}Virtual(derivedAddress{index}));
        }}
)CAC";

	char const* apply_function_structor = R"CAC(
    	using baseType{index} = ret{index}(${class_name}::*)({raw_arg_types}) {const};
		constexpr auto baseAddress{index} = (baseType{index})(&${class_name}::{function_name});
		using derivedType{index} = ret{index}(D<baseAddress{index}>::*)({raw_arg_types}) {const};
		constexpr auto derivedAddress{index} = (derivedType{index})(&D<baseAddress{index}>::{function_name});
        if (baseAddress{index} != derivedAddress{index}) {{
            Interface::get()->addHook((void*)address{index}(), (void*)FunctionScrapper::addressOf{non_virtual}Virtual(derivedAddress{index}));
        }}
)CAC";

	char const* apply_function_static = R"CAC(
		using baseType{index} = ret{index}(*)({raw_arg_types});
		constexpr auto baseAddress{index} = (baseType{index})(&{class_name}::{function_name});
		using derivedType{index} = ret{index}(*)({raw_arg_types});
		constexpr auto derivedAddress{index} = (derivedType{index})(&D<baseAddress{index}>::{function_name});
        if (baseAddress{index} != derivedAddress{index}) {{
            Interface::get()->addHook((void*)address{index}(), (void*)FunctionScrapper::addressOfNonVirtual(derivedAddress{index}));
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
            if (!CacShare::functionExists(f))
                continue; // Function not supported for this platform, skip it

            output += fmt::format(format_strings::predefine_return,
                fmt::arg("index", f.index),
                fmt::arg("return", CacShare::getReturn(f))
            );

            output += fmt::format(format_strings::predefine_address, 
                fmt::arg("index",f.index),
                fmt::arg("address", CacShare::getAddress(f))
            );

            switch (f.function_type) {
            	case kDestructor:
            		f.name = "destructor";
            	case kConstructor:
            		if (f.name != "destructor") f.name = "constructor";
            		output += fmt::format(format_strings::declare_structor,
				        fmt::arg("arg_types", CacShare::formatArgTypes(f.args)),
				        fmt::arg("raw_arg_types", CacShare::formatRawArgTypes(f.args)),
				        fmt::arg("class_name", unqualifiedName),
				        fmt::arg("const", f.is_const ? "const " : ""),
				        fmt::arg("convention", CacShare::getConvention(f)),
				        fmt::arg("function_name",f.name),
				        fmt::arg("index",f.index),
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
            if (!CacShare::functionExists(f))
                continue; // Function not supported for this platform, skip it

            char const* used_format;
            switch (f.function_type) {
                case kStaticFunction:
                    used_format = format_strings::apply_function_static;
                    break;
                case kConstructor:
                case kDestructor:
                    used_format = format_strings::apply_function_structor;
                    break;
                default:
               		used_format = format_strings::apply_function_member;
                    break;
            }

            output += fmt::format(used_format,
                fmt::arg("index", f.index),
                fmt::arg("class_name", unqualifiedName),
                fmt::arg("arg_types", CacShare::formatArgTypes(f.args)),
                fmt::arg("function_name", f.name),
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
