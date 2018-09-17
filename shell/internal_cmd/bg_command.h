//
// Created by Liam Heeger on 9/10/18.
//

#ifndef SHELL_BG_COMMAND_H
#define SHELL_BG_COMMAND_H

#include "internal_command.h"

namespace shell {

class BgCommand : public InternalCommand {
public:
    BgCommand(const Command& command);

    virtual void Run(ShellInfo* shell_info, std::string* output);

};

}

#endif //SHELL_BG_COMMAND_H
