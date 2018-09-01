//
// Created by Liam Heeger on 8/30/18.
//

#include "cd_command.h"

#include <cstdlib>

using namespace shell;

CdCommand::
CdCommand(const Command& command)
        : InternalCommand(command) {}

void
CdCommand::
Run(ShellInfo* shell_info, std::string* output) {
    int argument_count = command_.argument_size();

    // TODO need to check if is a valid path.
    if (argument_count == 0) {
        shell_info->set_working_directory(std::getenv("HOME"));
    } else {

    }
}

