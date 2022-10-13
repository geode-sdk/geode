#include "Shared.hpp"

namespace { namespace format_strings {

	char const* declare_address = R"GEN(
GEODE_INLINE GEODE_HIDDEN static uintptr_t address{index}() {{
	static uintptr_t ret = {address};
	return ret;
}}
)GEN";

}}

std::string generateAddressHeader(Root& root) {
	std::string output;

	for (auto& c : root.classes) {

		for (auto& field : c.fields) {
			std::string address_str;

			auto fn = field.get_as<FunctionBindField>();

			if (!fn) {
				continue;
			}

			if (codegen::getStatus(field) == BindStatus::Binded) {

				address_str = fmt::format("addresser::get{}Virtual((types::member{})(&{}::{}))",
					str_if("Non", !fn->beginning.is_virtual),
					field.field_id,
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
