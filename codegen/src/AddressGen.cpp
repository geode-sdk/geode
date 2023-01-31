#include "Shared.hpp"
#include "TypeOpt.hpp"

namespace {
    namespace format_strings {

        char const* address_begin = R"GEN(
#include <Geode/Bindings.hpp>
#include <Geode/modify/Addresses.hpp>
#include <Geode/modify/Traits.hpp>
#include <Geode/loader/Tulip.hpp>

using namespace geode;
)GEN";

        char const* declare_address = R"GEN(
template <>
uintptr_t geode::modifier::address<{index}>() {{
	static uintptr_t ret = {address};
	return ret;
}}
)GEN";

        char const* declare_metadata_begin = R"GEN(
Result<tulip::hook::HandlerMetadata> geode::modifier::handlerMetadataForAddress(uintptr_t address) {
	static auto s_value = []() {
		std::map<uintptr_t, tulip::hook::HandlerMetadata(*)()> ret;
)GEN";

        char const* declare_metadata = R"GEN(
        {{
        	using FunctionType = {return}(*)({class_name}{const}*{parameter_comma}{parameter_types});
			ret[modifier::address<{index}>()] = +[](){{
				return tulip::hook::HandlerMetadata{{
					.m_convention = geode::hook::createConvention(tulip::hook::TulipConvention::{convention}),
					.m_abstract = tulip::hook::AbstractFunction::from(FunctionType(nullptr)),
				}}; 
			}};
        }}  
)GEN";

        char const* declare_metadata_static = R"GEN(
        {{
        	using FunctionType = {return}(*)({parameter_types});
			ret[modifier::address<{index}>()] = +[](){{
				return tulip::hook::HandlerMetadata{{
					.m_convention = geode::hook::createConvention(tulip::hook::TulipConvention::{convention}),
					.m_abstract = tulip::hook::AbstractFunction::from(FunctionType(nullptr)),
				}};
			}};
        }}  
)GEN";

        char const* declare_metadata_structor = R"GEN(
        {{
        	using FunctionType = void(*)({class_name}*{parameter_comma}{parameter_types});
			ret[modifier::address<{index}>()] = +[](){{
				return tulip::hook::HandlerMetadata{{
					.m_convention = geode::hook::createConvention(tulip::hook::TulipConvention::{convention}),
					.m_abstract = tulip::hook::AbstractFunction::from(FunctionType(nullptr)),
				}};
			}};
        }}  
)GEN";

        char const* declare_metadata_end = R"GEN(
        return ret;
    }();
    if (s_value.count(address) > 0) return geode::Ok(std::move(s_value[address]()));
    return geode::Err("Address is not registered for wrapper");
}
)GEN";

    }
}

std::string generateAddressHeader(Root& root) {
    std::string output;

    TypeBank bank;
    bank.loadFrom(root);
    output += format_strings::address_begin;

    for (auto& c : root.classes) {
        for (auto& field : c.fields) {
            std::string address_str;

            auto fn = field.get_as<FunctionBindField>();

            if (!fn) {
                continue;
            }

            if (codegen::getStatus(field) == BindStatus::Binded) {
                auto const ids = bank.getIDs(fn->beginning, c.name);

                address_str = fmt::format(
                    "addresser::get{}Virtual(Resolve<{}>::func(&{}::{}))",
                    str_if("Non", !fn->beginning.is_virtual),
                    codegen::getParameterTypes(fn->beginning),
                    field.parent,
                    fn->beginning.name
                );
            }
            else if (codegen::getStatus(field) == BindStatus::NeedsBinding) {
                address_str = fmt::format("base::get() + 0x{:x}", codegen::platformNumber(fn->binds));
            }
            else {
                continue;
            }

            output += fmt::format(
                ::format_strings::declare_address,
                fmt::arg("address", address_str),
                fmt::arg("index", field.field_id)
            );
        }
    }

    output += format_strings::declare_metadata_begin;

    for (auto& c : root.classes) {
        for (auto& field : c.fields) {
            std::string address_str;

            auto fn = field.get_as<FunctionBindField>();

            if (!fn) {
                continue;
            }

            if (codegen::getStatus(field) == BindStatus::Binded) {
                address_str = fmt::format(
                    "addresser::get{}Virtual(Resolve<{}>::func(&{}::{}))",
                    str_if("Non", !fn->beginning.is_virtual),
                    codegen::getParameterTypes(fn->beginning),
                    field.parent,
                    fn->beginning.name
                );
            }
            else if (codegen::getStatus(field) == BindStatus::NeedsBinding) {
                address_str = fmt::format("base::get() + 0x{:x}", codegen::platformNumber(fn->binds));
            }
            else {
                continue;
            }

            char const* used_declare_format;

            switch (fn->beginning.type) {
                case FunctionType::Normal:
                    used_declare_format = format_strings::declare_metadata;
                    break;
                case FunctionType::Ctor:
                case FunctionType::Dtor:
                    used_declare_format = format_strings::declare_metadata_structor;
                    break;
            }

            if (fn->beginning.is_static)
                used_declare_format = format_strings::declare_metadata_static;

            output += fmt::format(
                used_declare_format,
                fmt::arg("address", address_str),
                fmt::arg("class_name", c.name),
                fmt::arg("const", str_if(" const ", fn->beginning.is_const)),
                fmt::arg("convention", codegen::getModifyConventionName(field)),
                fmt::arg("return", bank.getReturn(fn->beginning, c.name)),
                fmt::arg("parameters", codegen::getParameters(fn->beginning)),
                fmt::arg("parameter_types", codegen::getParameterTypes(fn->beginning)),
                fmt::arg("arguments", codegen::getParameterNames(fn->beginning)),
                fmt::arg("parameter_comma", str_if(", ", !fn->beginning.args.empty())),
                fmt::arg("index", field.field_id)
            );
        }
    }

    output += format_strings::declare_metadata_end;
    return output;
}
