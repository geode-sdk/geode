#include "Shared.hpp"

namespace { namespace format_strings {
	char const* source_start = R"CAC(
#include <Geode/Bindings.hpp>
#include <Geode/utils/addresser.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/modify/Addresses.hpp>
#include <Geode/modify/Types.hpp>
using namespace geode;
using namespace geode::cast;
using cocos2d::CCDestructor;
using namespace geode::core::meta; // Default convention
using namespace geode::core::meta::x86; // Windows x86 conventions, Function
using namespace geode::modifier; // types
)CAC";

	char const* declare_member = R"GEN(
types::ret{index} {class_name}::{function_name}({parameters}){const} {{
	auto func = Function<types::meta{index}, {convention}>({{addresses::address{index}()}});
	return func(this{parameter_comma}{arguments});
}}
)GEN";

	char const* declare_virtual = R"GEN(
types::ret{index} {class_name}::{function_name}({parameters}){const} {{
	auto self = addresser::thunkAdjust((types::member{index})(&{class_name}::{function_name}), this);
	auto func = Function<types::meta{index}, {convention}>({{addresses::address{index}()}});
	return func(self{parameter_comma}{arguments});
}}
)GEN";

	char const* declare_static = R"GEN(
types::ret{index} {class_name}::{function_name}({parameters}){const} {{
	auto func = Function<types::meta{index}, {convention}>({{addresses::address{index}()}});
	return func({arguments});
}}
)GEN";

	char const* declare_destructor = R"GEN(
{class_name}::{function_name}({parameters}) {{
	// basically we destruct it once by calling the gd function, 
	// then lock it, so that other gd destructors dont get called
	if (CCDestructor::lock(this)) return;
	auto func = Function<types::meta{index}, {convention}>({{addresses::address{index}()}});
	func(this{parameter_comma}{arguments});
	// we need to construct it back so that it uhhh ummm doesnt crash
	// while going to the child destructors
	auto thing = new (this) {class_name}(std::monostate(), sizeof({class_name}));
	CCDestructor::lock(this) = true;
}}
)GEN";

	char const* declare_constructor = R"GEN(
{class_name}::{function_name}({parameters}) : {class_name}(std::monostate(), sizeof({class_name})) {{
	// here we construct it as normal as we can, then destruct it
	// using the generated functions. this ensures no memory gets leaked
	// no crashes :pray:
	CCDestructor::lock(this) = true;
	{class_name}::~{unqualified_class_name}();
	auto func = Function<types::meta{index}, {convention}>({{addresses::address{index}()}});
	func(this{parameter_comma}{arguments});
}}
)GEN";

	// requires: static, return_type, function_name, raw_parameters, const, class_name, definition
	char const* ool_function_definition = R"GEN(
{return} {class_name}::{function_name}({parameters}){const} {definition}
)GEN";
}}

std::string generateGDSource(Root& root) {
	std::string output(format_strings::source_start);

	for (auto& c : root.classes) {

		for (auto& f : c.fields) {
			if (auto i = f.get_as<InlineField>()) {
				if (codegen::platform == Platform::Mac || codegen::platform == Platform::iOS) {
					if (can_find(c.name, "cocos2d"))
						output += i->inner + "\n";
				}
			} else if (auto fn = f.get_as<OutOfLineField>()) {
				output += fmt::format(format_strings::ool_function_definition,
					fmt::arg("function_name", fn->beginning.name),
					fmt::arg("const", str_if(" const ", fn->beginning.is_const)),
					fmt::arg("class_name", c.name),
                    fmt::arg("parameters", codegen::getParameters(fn->beginning)),
                    fmt::arg("index", f.field_id),
					fmt::arg("definition", fn->inner),
				    fmt::arg("return", fn->beginning.ret.name)
				);
			} else if (auto fn = f.get_as<FunctionBindField>()) {
				if (codegen::getStatus(f) != BindStatus::NeedsBinding)
					continue;

				char const* used_declare_format;

				switch (fn->beginning.type) {
					case FunctionType::Normal:
						used_declare_format = format_strings::declare_member;
						break;
					case FunctionType::Ctor:
						used_declare_format = format_strings::declare_constructor;
						break;
					case FunctionType::Dtor:
						used_declare_format = format_strings::declare_destructor;
						break;
				}

				if (fn->beginning.is_static)
					used_declare_format = format_strings::declare_static;
				if (fn->beginning.is_virtual && fn->beginning.type != FunctionType::Dtor)
					used_declare_format = format_strings::declare_virtual;

				output += fmt::format(used_declare_format,
					fmt::arg("class_name", c.name),
					fmt::arg("unqualified_class_name", codegen::getUnqualifiedClassName(c.name)),
					fmt::arg("const", str_if(" const ", fn->beginning.is_const)),
					fmt::arg("convention", codegen::getConvention(f)),
					fmt::arg("function_name", fn->beginning.name),
					fmt::arg("index", f.field_id),
					fmt::arg("parameters", codegen::getParameters(fn->beginning)),
					fmt::arg("parameter_types", codegen::getParameterTypes(fn->beginning)),
					fmt::arg("arguments", codegen::getParameterNames(fn->beginning)),
					fmt::arg("parameter_comma", str_if(", ", !fn->beginning.args.empty()))
				);
			}
		}
	}
	return output;
}
