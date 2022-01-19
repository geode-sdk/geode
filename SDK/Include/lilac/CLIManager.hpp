#pragma once

#include "Macros.hpp"
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

namespace lilac {
    #pragma warning(disable: 4251)
    
    struct CLIArgs {
        std::vector<std::string> args;
        std::unordered_map<std::string, std::string> flags;
    };

    using CLICommand = std::function<void(CLIArgs const&)>;

    class LILAC_DLL CLIManager {
        protected:
            std::vector<CLICommand> m_cmds;
        
        public:
            static CLIManager* get();

            void addCommand(CLICommand cmd);
            void execute(std::vector<std::string> args); 
    };
}
