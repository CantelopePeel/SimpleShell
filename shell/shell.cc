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
#include "internal_cmd/fg_command.h"
#include "shell_macros.h"
#include "internal_cmd/bg_command.h"
#include "internal_cmd/shell_info_command.h"

#include <vector>
#include <regex>
#include <iostream>
#include <unistd.h>

using namespace shell;

Shell::
Shell()
        : shell_info_(std::shared_ptr<ShellInfo>(new ShellInfo())),
          signal_manager_(SignalManager::GetInstance()) {
    signal_manager_->SetShellInfo(shell_info_);
    command_manager_ = new CommandManager(shell_info_, signal_manager_);
}

bool
Shell::
Start() {
    signal_manager_->Start();

    std::string working_directory(std::getenv("PWD"));

    shell_info_->set_working_directory(working_directory);

    for (;;) {
        std::cout << shell_info_->working_directory() << "> ";
        std::string command_line;

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
            signal_manager_->Stop();
            auto exit_command = ExitCommand(command);
            exit_command.Run(shell_info_.get(), &command_output);
        } else if (program == "jobs") {
            auto jobs_command = JobsCommand(command);
            jobs_command.Run(shell_info_.get(), &command_output);
        } else if (program == "fg") {
            int argument_count = command.sub_command(0).argument_size();
            auto fg_command = FgCommand(command);
            fg_command.Run(shell_info_.get(), &command_output);
            if (argument_count == 1) {
                try {
                    int job_id = std::stoi(command.sub_command(0).argument(0));
                    if (signal_manager_->ContinueJob(shell_info_, job_id)) {
                        if (!signal_manager_->SetJobStatus(shell_info_, job_id, Job::FOREGROUND)) {
                            return false;
                        }
                    } else {
                        return false;
                    }
                } catch (const std::invalid_argument& ia) {
                    std::cerr << "Error: Bad job id." << std::endl;
                } catch (const std::out_of_range& oor) {
                    std::cerr << "Error: Bad job id." << std::endl;
                }
            }

            for (auto job_iter = shell_info_->mutable_job()->pointer_begin();
                 job_iter !=  shell_info_->mutable_job()->pointer_end();
                 job_iter++) {
                if ((*job_iter)->status() == Job::FOREGROUND) {
                    command_manager_->BlockForegroundJob(*job_iter);
                }
            }
        } else if (program == "bg") {
            int argument_count = command.sub_command(0).argument_size();
            auto bg_command = BgCommand(command);
            bg_command.Run(shell_info_.get(), &command_output);
            if (argument_count == 1) {
                try {
                    int job_id = std::stoi(command.sub_command(0).argument(0));
                    if (signal_manager_->ContinueJob(shell_info_, job_id)) {
                        if (!signal_manager_->SetJobStatus(shell_info_, job_id, Job::BACKGROUND)) {
                            return false;
                        }
                    } else {
                        return false;
                    }
                } catch (const std::invalid_argument& ia) {
                    std::cerr << "Error: Bad job id." << std::endl;
                } catch (const std::out_of_range& oor) {
                    std::cerr << "Error: Bad job id." << std::endl;
                }
            }
        } else if (program == "shell_info") {
            auto shell_info_command = ShellInfoCommand(command);
            shell_info_command.Run(shell_info_.get(), &command_output);
        }
        return true;
    } else {
        return command_manager_->Run(command);
    }
}


