//
// Created by Liam Heeger on 8/30/18.
//

#include "shell.h"
#include "shell_utils.h"
#include "internal_cmd/exit_command.h"

#include <iostream>

using namespace shell;

bool
Shell::
Start() {
    for (;;) {
        std::cout << "> ";
        std::string command_line;
        std::cin >> command_line;
        // TODO set a maximum length of command to avoid unreasonable commands / bad agents.

        Command command;
        if (!ShellUtils::ParseCommand(command_line, &command)) {
            // TODO handle if command is invalid.
        }

        if (!DelegateCommand(command)) {

        }
    }
}

bool
Shell::
DelegateCommand(Command command) {
    if (ShellUtils::IsInteralCommand(command)) {
        std::string program(command.program());
        std::string command_output;

        if (program == "exit") {
            auto exit_command = ExitCommand(command);
            exit_command.Run(new ShellInfo(), &command_output);

        }
    } else {
        std::cout << "Print: " << command.DebugString() << std::endl;
    }
}


