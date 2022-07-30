#include "Shared.hpp"
#include <set>

namespace { namespace format_strings {

	char const* declare_member_type = R"GEN(
GEODE_WRAPPER_FOR_IDENTIFIER({function_name}) )GEN";
}}

using std::set;

std::string generateWrapperHeader(Root& root) {
	std::string output;
	std::set<std::string> used;

	for (auto& c : root.classes) {
		for (auto& f : c.fields) {
			if (auto fn = f.get_fn()) {
				if (fn->type == FunctionType::Normal && !used.count(fn->name)) {
					used.insert(fn->name);
					output += fmt::format(format_strings::declare_member_type,
						fmt::arg("function_name", fn->name)
					);
				}
			}
		}
	}

	return output;
}
