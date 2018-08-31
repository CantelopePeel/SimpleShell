//
// Created by Liam Heeger on 8/30/18.
//

#include "shell.h"

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

        std::cout << "Print: " << command_line << std::endl;

        ProcessCommandLine(command_line);
        if (command_line == "exit") {
            Exit();
        }
    }
}

bool Shell::Exit() {
    std::exit(0);
}

bool
Shell::
ProcessCommandLine(const std::string& command) {
    return false;
}


