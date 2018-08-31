//
// Created by Liam Heeger on 8/30/18.
//

#ifndef SHELL_CD_COMMAND_H
#define SHELL_CD_COMMAND_H

#include "internal_command.h"

namespace shell {
    class CdCommand : public InternalCommand {
        public:
            virtual void Run(ShellInfo* shell_info, std::string* output);
    };
}


#endif //SHELL_CD_COMMAND_H
