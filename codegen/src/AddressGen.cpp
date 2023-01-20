#include "Shared.hpp"
#include "TypeOpt.hpp"

namespace { namespace format_strings {

	char const* address_begin = R"GEN(
#include <Geode/Bindings.hpp>
#include <Geode/modify/Addresses.hpp>
#include <Geode/modify/Traits.hpp>
)GEN";

	char const* declare_address = R"GEN(
template <>
uintptr_t geode::modifier::address<{index}>() {{
	static uintptr_t ret = {address};
	return ret;
}}
)GEN";

}}

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
				const auto ids = bank.getIDs(fn->beginning, c.name);

				address_str = fmt::format("addresser::get{}Virtual(Resolve<{}>::func(&{}::{}))",
					str_if("Non", !fn->beginning.is_virtual),
					codegen::getParameterTypes(fn->beginning),
					field.parent,
					fn->beginning.name
				);
			} else if (codegen::getStatus(field) == BindStatus::NeedsBinding) {
				address_str = fmt::format("base::get() + 0x{:x}", codegen::platformNumber(fn->binds));
			} else { 
				continue;
			}

			output += fmt::format(::format_strings::declare_address,
				fmt::arg("address", address_str),
				fmt::arg("index", field.field_id)
			);
		}
	}
	return output;
}
