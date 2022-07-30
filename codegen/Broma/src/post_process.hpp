#pragma once

#include "state.hpp"
#include <algorithm>
#include <iostream>

namespace broma {
    inline void sortClass(Class cls, Root& root, std::vector<Class>& output) {
        root.classes.erase(std::remove(root.classes.begin(), root.classes.end(), cls), root.classes.end());
        for (auto name : cls.depends) {
            if (root[name])
                sortClass(*root[name], root, output);
        }
        output.push_back(cls);
    }

    inline void post_process(Root& root) {
        std::vector<Class> out;

        while (root.classes.size())
            sortClass(root.classes[0], root, out);

        root.classes = out;
    }
}