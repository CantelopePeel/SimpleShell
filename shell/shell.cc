//
// Created by Liam Heeger on 8/30/18.
//

#include "shell.h"

#include "shell_utils.h"
#include "internal_cmd/exit_command.h"
#include "internal_cmd/cd_command.h"
#include "internal_cmd/jobs_command.h"
#include "command_manager.h"
#include "shell/proto/job.pb.h"

#include <iostream>
#include <unistd.h>

using namespace shell;

bool
Shell::
Start() {
    std::string working_directory(std::getenv("PWD"));

    shell_info_.set_working_directory(working_directory);

    for (;;) {
        std::cout << shell_info_.working_directory() << "> ";
        std::string command_line;
        std::getline(std::cin, command_line);
        // TODO set a maximum length of command to avoid unreasonable commands / bad agents.

        ProcessCommandLine(command_line);
    }
}

bool
Shell::
ProcessCommandLine(const std::string& command_line) {
    Command command;
    if (!ShellUtils::ParseCommand(command_line, &command)) {
        // TODO handle if command is invalid.
        return false;
    }

    if (!DelegateCommand(command)) {
        return false;
    }

    return true;
}

bool
Shell::
DelegateCommand(Command command) {
    if (ShellUtils::IsInteralCommand(command)) {
        std::string program(command.program());
        std::string command_output;

        if (program == "cd") {
            auto cd_command = CdCommand(command);
            cd_command.Run(&shell_info_, &command_output);
        } else if (program == "exit") {
            auto exit_command = ExitCommand(command);
            exit_command.Run(&shell_info_, &command_output);
        } else if (program == "jobs") {
            auto jobs_command = JobsCommand(command);
            jobs_command.Run(&shell_info_, &command_output);
        }
        return true;
    } else {
        Job job;
        return command_manager_.Run(command, &job);
    }
}


