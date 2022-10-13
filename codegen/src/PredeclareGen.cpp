#include "Shared.hpp"
#include <iostream>
#include <set>

namespace { namespace format_strings {
	char const* class_predeclare = "class {class_name};\n";
}}

std::string generatePredeclareHeader(Root& root) {
    std::string output("#pragma once\n");

    for (auto& cls : root.classes) {
        if (can_find(cls.name, "cocos2d"))
            continue;

        output += fmt::format(::format_strings::class_predeclare,
            fmt::arg("class_name", cls.name)
        );
    }

    return output;
}
