//
// Created by Liam Heeger on 9/10/18.
//

#include "fg_command.h"

using namespace shell;

FgCommand::
FgCommand(const Command& command)
        : InternalCommand(command) {}

void FgCommand::Run(ShellInfo* shell_info, std::string* output) {
    int argument_count = command_.sub_command(0).argument_size();

    if (argument_count != 1) {
        std::cerr << "Error: fg command requires 1 argument." << std::endl;
    }
}
