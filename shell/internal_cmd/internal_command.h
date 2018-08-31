//
// Created by Liam Heeger on 8/30/18.
//

#ifndef SHELL_INTERNAL_COMMAND_H
#define SHELL_INTERNAL_COMMAND_H

#include "shell/proto/shell_info.pb.h"
#include "shell/proto/command.pb.h"

namespace shell {
    class InternalCommand : public Command {
        public:
            virtual void Run(ShellInfo* shell_info, std::string* output) = 0;
    };
}


#endif //SHELL_INTERNAL_COMMAND_H
