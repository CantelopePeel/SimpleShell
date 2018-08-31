//
// Created by Liam Heeger on 8/30/18.
//

#include "exit_command.h"

using namespace shell;

ExitCommand::
ExitCommand(const Command& command)
        : InternalCommand(command) {}

void
ExitCommand::
Run(ShellInfo* shell_info, std::string* output) {
    std::exit(0);
}