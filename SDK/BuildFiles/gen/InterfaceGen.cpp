#include "SharedGen.hpp"
#include <iostream>

namespace format_strings {
    // requires: class_name
    char const* interface_start = R"CAC(
template<typename U=void, template <typename T, auto orig> class D = __unitSpec>
struct ${class_name} : {class_name}, InterfaceBase {{
    ${class_name}(const ${class_name}& c) : {class_name}(c) {{}}
    ${class_name}() = delete;

    static inline std::unordered_map<size_t, container_t<>*> fields;
    static inline size_t originalDestructor;
    static void fieldCleanup(size_t self) {{
    	const size_t begin = self + sizeof(${class_name});
    	const size_t end = self + sizeof(D<U, 0>);
    	for (size_t i = begin; i < end; ++i) {{
    		if (fields.find(i) != fields.end()) {{
    			delete fields.at(i);
    			fields.erase(i);
    		}}
    	}}
    	reinterpret_cast<void(*)(size_t)>(originalDestructor)(self);
    }}
)CAC";

    // requires: index, address
    char const* predefine_address = "    static inline auto address{index} = {address};";

    // requires: index, return
    char const* predefine_return = R"CAC(
    using ret{index} = {return};
)CAC";

    // requires: class_name, function_name, const, index, count, arg_types, parameters
    char const* declare_member = R"CAC(
    dupable ret{index} {function_name}({raw_args}) {const}{{
        return reinterpret_cast<ret{index}(*)(decltype(this){arg_types})>(address{index})(this{parameters});
    }}
)CAC";

    // requires: class_name, function_name, const, index, raw_args, arg_types, raw_parameters
    char const* declare_static = R"CAC(
    dupable static ret{index} {function_name}({raw_args}) {const}{{
        return reinterpret_cast<ret{index}(*)({raw_arg_types})>(address{index})({raw_parameters});
    }}
)CAC";

    // requires: function_name, raw_args, arg_types, index, parameters
    char const* declare_structor = R"CAC(
    dupable void {function_name}({raw_args}) {{
        reinterpret_cast<void(*)(decltype(this){arg_types})>(address{index})(this{parameters});
    }}
)CAC";

    // requires: class_name, function_name, const, index, raw_args, arg_types, parameters, convention
    char const* declare_meta_member = R"CAC(
    dupable ret{index} {function_name}({raw_args}) {const}{{
        return lilac::meta::Function<ret{index}(decltype(this){arg_types})>, lilac::meta::x86::{convention}>{{address{index}}(this{parameters});
    }}
)CAC";

    // requires: class_name, function_name, const, index, arg_types, raw_parameters, raw_args, convention
    char const* declare_meta_static = R"CAC(
    static dupable ret{index} {function_name}({raw_args}) {const}{{
        return lilac::meta::Function<ret{index}(decltype(this){arg_types})>, lilac::meta::x86::{convention}>{{address{index}}({raw_parameters});
    }}
)CAC";

    // requires: function_name, raw_args, arg_types, index, parameters, convention
    char const* declare_meta_structor = R"CAC(
    dupable void {function_name}({raw_args}) {{
        return lilac::meta::Function<void(decltype(this){arg_types})>, lilac::meta::x86::{convention}>{{address{index}}(this{parameters});
    }}
)CAC";

    char const* apply_start = R"CAC(
    static bool _apply() {
)CAC";
    // requires: index, class_name, arg_types, function_name, raw_arg_types, non_virtual
    char const* apply_function_member = R"CAC(
    	using baseType{index} = ret{index}(${class_name}::*)({raw_arg_types}) {const};
		constexpr auto baseAddress{index} = (baseType{index})(&${class_name}::{function_name});
		using derivedType{index} = ret{index}(D<U, baseAddress{index}>::*)({raw_arg_types}) {const};
		constexpr auto derivedAddress{index} = (derivedType{index})(&D<U, baseAddress{index}>::{function_name});
        if (baseAddress{index} != derivedAddress{index}) {{
            modContainer.registerHook(address{index}, FunctionScrapper::addressOf{non_virtual}Virtual(derivedAddress{index}));
        }}
)CAC";

	char const* apply_function_static = R"CAC(
		using baseType{index} = ret{index}(*)({raw_arg_types});
		constexpr auto baseAddress{index} = (baseType{index})(&${class_name}::{function_name});
		using derivedType{index} = ret{index}(*)({raw_arg_types});
		constexpr auto derivedAddress{index} = (derivedType{index})(&D<U, baseAddress{index}>::{function_name});
        if (baseAddress{index} != derivedAddress{index}) {{
            modContainer.registerHook(address{index}, FunctionScrapper::addressOfNonVirtual(derivedAddress{index}));
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

        output += fmt::format(format_strings::interface_start, fmt::arg("class_name", unqualifiedName));

        for (auto& f : c.functions) {
            if (f.binds[CacShare::platform].size() == 0)
                continue; // Function not supported for this platform, skip it

            output += fmt::format(format_strings::predefine_return,
                fmt::arg("index", f.index),
                fmt::arg("return", CacShare::getReturn(f))
            );

            output += fmt::format(format_strings::predefine_address, 
                fmt::arg("index",f.index),
                fmt::arg("address", CacShare::getAddress(f))
            );

            char const* used_format;

            switch (f.function_type) {
                case kVirtualFunction:
                case kRegularFunction:
                    used_format = CacShare::platform == kWindows ? format_strings::declare_meta_member : format_strings::declare_member;
                    break;
                case kStaticFunction:
                    used_format = CacShare::platform == kWindows ? format_strings::declare_meta_static : format_strings::declare_static;
                    break;
                case kDestructor:
               		f.name = "destructor";
                case kConstructor:
                	if (f.name != "destructor") f.name = "constructor";
                    used_format = CacShare::platform == kWindows ? format_strings::declare_meta_structor : format_strings::declare_structor;
                    break;
            }

            output += fmt::format(used_format,
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
        }

        output += format_strings::apply_start;

        for (auto& f : c.functions) {
            if (f.binds[CacShare::platform].size() == 0)
                continue; // Function not supported for this platform, skip it

            char const* used_format;
            switch (f.function_type) {
                case kStaticFunction:
                    used_format = format_strings::apply_function_static;
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
