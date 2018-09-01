//
// Created by Liam Heeger on 8/30/18.
//

#ifndef SHELL_JOBS_COMMAND_H
#define SHELL_JOBS_COMMAND_H

#include "internal_command.h"

namespace shell {
    class JobsCommand : public InternalCommand {
    public:
        JobsCommand(const Command& command);

        virtual void Run(ShellInfo* shell_info, std::string* output);
    };
}


#endif //SHELL_JOBS_COMMAND_H
