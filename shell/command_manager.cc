//
// Created by Liam Heeger on 9/1/18.
//

#include "command_manager.h"

#include <unistd.h>
#include <sys/types.h>

using namespace shell;

bool
CommandManager::
Run(const Command& command, Job* job) {
    pid_t pid = fork();

    if (pid == -1) {
        // TODO handle error with message.
        return false;
    }

    char* arguments[command.argument_size() + 2];

    arguments[0] = const_cast<char*>(command.program().c_str());
    for (int i = 0; i < command.argument_size(); i++) {
        arguments[i + 1] = const_cast<char*>(command.argument(i).c_str());
    }
    arguments[command.argument_size() + 1] = nullptr;

    if (pid == 0) {
        int exec_val = execvp(command.program().c_str(), arguments);
        if (exec_val < 0) {
            std::cout << "ERROR" << std::endl;
        }
    } else if (pid > 0) {
        waitpid(pid, nullptr, 0);
    }
}
