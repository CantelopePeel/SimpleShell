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

#include <vector>
#include <regex>
#include <iostream>
#include <unistd.h>

using namespace shell;

Shell::
Shell()
        : shell_info_(std::shared_ptr<ShellInfo>(new ShellInfo())),
          command_manager_(shell_info_),
          signal_manager_(shell_info_) {
}

bool
Shell::
Start() {
    signal_manager_.Start();

    std::string working_directory(std::getenv("PWD"));

    shell_info_->set_working_directory(working_directory);

    for (;;) {
        std::cout << shell_info_->working_directory() << "> ";
        std::string command_line;
        // TODO set a maximum length of command to avoid unreasonable commands / bad agents.
        if (std::getline(std::cin, command_line)) {
            ProcessCommandLine(command_line);
        } else {
            std::cout << std::endl;
            break;
        }
    }

    return true;
}

bool
Shell::
ProcessCommandLine(const std::string& command_line) {
    std::regex empty_line_re("\\s*");
    if (std::regex_match(command_line, empty_line_re)) {
        return true;
    }

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
        std::string program(command.sub_command(0).program());
        std::string command_output;

        if (program == "cd") {
            auto cd_command = CdCommand(command);
            cd_command.Run(shell_info_.get(), &command_output);
        } else if (program == "exit") {
            signal_manager_.Stop();
            auto exit_command = ExitCommand(command);
            exit_command.Run(shell_info_.get(), &command_output);
        } else if (program == "jobs") {
            auto jobs_command = JobsCommand(command);
            jobs_command.Run(shell_info_.get(), &command_output);
        }
        return true;
    } else {
        Job job;
        if (!command_manager_.Run(command, &job)) {
            // TODO do something on error.
        }
    }
}


