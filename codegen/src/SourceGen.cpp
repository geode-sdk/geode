#include "Shared.hpp"
#include "TypeOpt.hpp"

namespace { namespace format_strings {
	char const* source_start = R"CAC(
#include <stdexcept>
#include <Geode/Bindings.hpp>
#include <Geode/utils/addresser.hpp>
#include <Geode/modify/Addresses.hpp>
#include <Geode/modify/Traits.hpp>
#include <Geode/loader/Tulip.hpp>

using namespace geode;
using namespace geode::modifier;
using cocos2d::CCDestructor;

std::unordered_map<void*, bool>& CCDestructor::destructorLock() {{
	static auto ret = new std::unordered_map<void*, bool>;
	return *ret;
}}
bool& CCDestructor::globalLock() {{
	static thread_local bool ret = false;
	return ret; 
}}
bool& CCDestructor::lock(void* self) {
	return destructorLock()[self];
}
CCDestructor::~CCDestructor() {{
	destructorLock().erase(this);
}}

auto wrapFunction(uintptr_t address, tulip::hook::WrapperMetadata const& metadata) {
	auto wrapped = geode::hook::createWrapper(reinterpret_cast<void*>(address), metadata);
	if (wrapped.isErr()) {{
		throw std::runtime_error(wrapped.unwrapErr());
	}}
	return wrapped.unwrap();
}
)CAC";

	char const* declare_member = R"GEN(
auto {class_name}::{function_name}({parameters}){const} -> decltype({function_name}({arguments})) {{
	using FunctionType = decltype({function_name}({arguments}))(*)({class_name}{const}*{parameter_comma}{parameter_types});
	static auto func = wrapFunction(address<{addr_index}>(), tulip::hook::WrapperMetadata{{
		.m_convention = geode::hook::createConvention(tulip::hook::TulipConvention::{convention}),
		.m_abstract = tulip::hook::AbstractFunction::from(FunctionType(nullptr)),
	}});
	return reinterpret_cast<FunctionType>(func)(this{parameter_comma}{arguments});
}}
)GEN";

	char const* declare_virtual = R"GEN(
auto {class_name}::{function_name}({parameters}){const} -> decltype({function_name}({arguments})) {{
	auto self = addresser::thunkAdjust(Resolve<{parameter_types}>::func(&{class_name}::{function_name}), this);
	using FunctionType = decltype({function_name}({arguments}))(*)({class_name}{const}*{parameter_comma}{parameter_types});
	static auto func = wrapFunction(address<{addr_index}>(), tulip::hook::WrapperMetadata{{
		.m_convention = geode::hook::createConvention(tulip::hook::TulipConvention::{convention}),
		.m_abstract = tulip::hook::AbstractFunction::from(FunctionType(nullptr)),
	}});
	return reinterpret_cast<FunctionType>(func)(self{parameter_comma}{arguments});
}}
)GEN";

	char const* declare_static = R"GEN(
auto {class_name}::{function_name}({parameters}){const} -> decltype({function_name}({arguments})) {{
	using FunctionType = decltype({function_name}({arguments}))(*)({parameter_types});
	static auto func = wrapFunction(address<{addr_index}>(), tulip::hook::WrapperMetadata{{
		.m_convention = geode::hook::createConvention(tulip::hook::TulipConvention::{convention}),
		.m_abstract = tulip::hook::AbstractFunction::from(FunctionType(nullptr)),
	}});
	return reinterpret_cast<FunctionType>(func)({arguments});
}}
)GEN";

	char const* declare_destructor = R"GEN(
{class_name}::{function_name}({parameters}) {{
	// basically we destruct it once by calling the gd function, 
	// then lock it, so that other gd destructors dont get called
	if (CCDestructor::lock(this)) return;
	using FunctionType = void(*)({class_name}*{parameter_comma}{parameter_types});
	static auto func = wrapFunction(address<{addr_index}>(), tulip::hook::WrapperMetadata{{
		.m_convention = geode::hook::createConvention(tulip::hook::TulipConvention::{convention}),
		.m_abstract = tulip::hook::AbstractFunction::from(FunctionType(nullptr)),
	}});
	reinterpret_cast<FunctionType>(func)(this{parameter_comma}{arguments});
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
	using FunctionType = void(*)({class_name}*{parameter_comma}{parameter_types});
	static auto func = wrapFunction(address<{addr_index}>(), tulip::hook::WrapperMetadata{{
		.m_convention = geode::hook::createConvention(tulip::hook::TulipConvention::{convention}),
		.m_abstract = tulip::hook::AbstractFunction::from(FunctionType(nullptr)),
	}});
	reinterpret_cast<FunctionType>(func)(this{parameter_comma}{arguments});
}}
)GEN";

	char const* ool_function_definition = R"GEN(
{return} {class_name}::{function_name}({parameters}){const} {definition}
)GEN";

	char const* ool_structor_function_definition = R"GEN(
{class_name}::{function_name}({parameters}){const} {definition}
)GEN";
}}

std::string generateBindingSource(Root& root) {
	std::string output(format_strings::source_start);

	TypeBank bank;
	bank.loadFrom(root);

	for (auto& c : root.classes) {

		for (auto& f : c.fields) {
			if (auto i = f.get_as<InlineField>()) {
				if (codegen::platform == Platform::Mac || codegen::platform == Platform::iOS) {
					if (can_find(c.name, "cocos2d"))
						output += i->inner + "\n";
				}
			} else if (auto fn = f.get_as<OutOfLineField>()) {
				if (codegen::getStatus(f) != BindStatus::Unbindable)
					continue;

				// no cocos2d definitions on windows
				if (codegen::platform == Platform::Windows && f.parent.rfind("cocos2d::", 0) == 0) {
					continue;
				}

				switch (fn->beginning.type) {
					case FunctionType::Ctor:
					case FunctionType::Dtor:
						output += fmt::format(format_strings::ool_structor_function_definition,
							fmt::arg("function_name", fn->beginning.name),
							fmt::arg("const", str_if(" const ", fn->beginning.is_const)),
							fmt::arg("class_name", c.name),
		                    fmt::arg("parameters", codegen::getParameters(fn->beginning)),
							fmt::arg("definition", fn->inner)
						);
						break;
					default:
						output += fmt::format(format_strings::ool_function_definition,
							fmt::arg("function_name", fn->beginning.name),
							fmt::arg("const", str_if(" const ", fn->beginning.is_const)),
							fmt::arg("class_name", c.name),
		                    fmt::arg("parameters", codegen::getParameters(fn->beginning)),
							fmt::arg("definition", fn->inner),
						    fmt::arg("return", fn->beginning.ret.name)
						);
						break;
				}
				
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

				auto ids = bank.getIDs(fn->beginning, c.name);

				output += fmt::format(used_declare_format,
					fmt::arg("class_name", c.name),
					fmt::arg("unqualified_class_name", codegen::getUnqualifiedClassName(c.name)),
					fmt::arg("const", str_if(" const ", fn->beginning.is_const)),
					fmt::arg("convention", codegen::getModifyConventionName(f)),
					fmt::arg("function_name", fn->beginning.name),
					fmt::arg("meta_index", ids.meta),
					fmt::arg("member_index", ids.member),
					fmt::arg("ret_index", ids.ret),
					fmt::arg("addr_index", f.field_id),
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
