//
// Created by Liam Heeger on 8/30/18.
//

#ifndef SHELL_SHELL_INFO_COMMAND_H
#define SHELL_SHELL_INFO_COMMAND_H

#include "internal_command.h"

namespace shell {
    class ShellInfoCommand : public InternalCommand {
    public:
        ShellInfoCommand(const Command& command);

        virtual void Run(ShellInfo* shell_info, std::string* output);
    };
}


#endif //SHELL_SHELL_INFO_COMMAND_H
