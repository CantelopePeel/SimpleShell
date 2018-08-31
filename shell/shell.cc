//
// Created by Liam Heeger on 8/30/18.
//

#include "shell.h"

using namespace shell;

bool
Shell::Start() {
    for (;;) {
        std::string command_line = "foo";
        ProcessCommandLine(command_line);
    }
}

bool
Shell::
ProcessCommandLine(const std::string& command) {
    return false;
}
