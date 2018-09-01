//
// Created by Liam Heeger on 8/30/18.
//

#ifndef SHELL_EXIT_COMMAND_H
#define SHELL_EXIT_COMMAND_H

#include "internal_command.h"

namespace shell {
    class ExitCommand : public InternalCommand {
        public:
            ExitCommand(const Command& command);

            virtual void Run(ShellInfo* shell_info, std::string* output);
    };
}


#endif //SHELL_EXIT_COMMAND_H
