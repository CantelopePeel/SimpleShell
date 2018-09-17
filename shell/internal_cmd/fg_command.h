//
// Created by Liam Heeger on 9/10/18.
//

#ifndef SHELL_FG_COMMAND_H
#define SHELL_FG_COMMAND_H

#include "internal_command.h"

namespace shell {

class FgCommand : public InternalCommand {
public:
    FgCommand(const Command& command);

    virtual void Run(ShellInfo* shell_info, std::string* output);

};

}

#endif //SHELL_FG_COMMAND_H
