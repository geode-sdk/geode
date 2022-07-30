#include "Shared.hpp"

namespace { namespace format_strings {

	char const* declare_address = R"GEN(
GEODE_NOINLINE GEODE_HIDDEN inline static uintptr_t address{index}() {{
	static uintptr_t ret = {address};
	return ret;
}}
)GEN";

}}

std::string generateAddressHeader(Root& root) {
	std::string output;

	for (auto& c : root.classes) {

		for (auto& field : c.fields) {
			if (codegen::getStatus(field) == BindStatus::Unbindable)
				continue;

			std::string address_str;

			if (auto fn = field.get_as<FunctionBindField>()) {
				address_str = fmt::format("base::get() + {}", codegen::platformNumber(fn->binds));
			} else if (auto fn = field.get_as<OutOfLineField>()) {
				address_str = fmt::format("addresser::get{}Virtual((types::member{})(&{}::{}))",
					str_if("Non", !fn->beginning.is_virtual),
					field.field_id,
					field.parent,
					fn->beginning.name
				);
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
