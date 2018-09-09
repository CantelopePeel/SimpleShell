//
// Created by Liam Heeger on 9/1/18.
//

#include "command_manager.h"

#include <vector>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace shell;

CommandManager::
CommandManager(std::shared_ptr<ShellInfo> shell_info, SignalManager* signal_manager)
        : signal_manager_(signal_manager), shell_info_(shell_info) {}

bool
CommandManager::
Run(const Command& command) {
    Job* job = shell_info_->add_job();

    int read_fd = STDIN_FILENO;
    int current_read_fd = STDIN_FILENO;
    int current_write_fd = STDOUT_FILENO;
    int pipe_fd[2];

    for (auto command_iter = command.sub_command().begin(); command_iter != command.sub_command().end();
            command_iter++) {

        // TODO some of this will be replaced with CreatePipes call.
        if (command.sub_command_size() > 1) {
            if (command_iter + 1 != command.sub_command().end()) {
                // Pipe input/output.

                int pipe_val = pipe(pipe_fd);

                if (pipe_val == -1) {
                    // TODO handle pipe creation error.
                    return false;
                }

                current_read_fd = read_fd;
                current_write_fd = pipe_fd[1];

                read_fd = pipe_fd[0];
                // Add file descriptors to job.
                job->add_open_file_descriptor(pipe_fd[0]);
                job->add_open_file_descriptor(pipe_fd[1]);
            } else {
                current_read_fd = read_fd;
                current_write_fd = STDOUT_FILENO;
            }
        }

        // TODO handle sig service ret error state.
        signal_manager_->BlockSignals();
        pid_t pid = fork();


        if (pid == -1) {
            // TODO handle error with message.
            return false;
        }


        // Convert command arguments.
        char* arguments[command_iter->argument_size() + 2];

        arguments[0] = const_cast<char*>(command_iter->program().c_str());
        for (int i = 0; i < command_iter->argument_size(); i++) {
            arguments[i + 1] = const_cast<char*>(command_iter->argument(i).c_str());
        }
        arguments[command_iter->argument_size() + 1] = nullptr;


        if (pid == 0) { // Child Process
            // TODO remove debug msg
            // std::cout << "Child Proc: " << command_iter->DebugString() << " " << current_read_fd << " " << current_write_fd << std::endl; // TODO remove.
            signal_manager_->UnblockSignals();
            // Set child process's PGID.
            // TODO handle sys call error.
            setpgid(0, 0);

            if (current_read_fd != STDIN_FILENO) {
                // TODO handle syscall error
                dup2(current_read_fd, STDIN_FILENO);
                close(current_read_fd);
            }

            if (current_write_fd != STDOUT_FILENO) {
                // TODO handle syscall error
                dup2(current_write_fd, STDOUT_FILENO);
                close(current_write_fd);
            }

            int exec_val = execvp(command_iter->program().c_str(), arguments);
            if (exec_val < 0) {
                // TODO give a verbose error message.
                std::cout << "ERROR" << std::endl;
                std::exit(0);
                return false;
            }
        } else if (pid > 0) { // Parent Process
            // TODO handle syscall error.

            if (current_write_fd != STDOUT_FILENO) {
                close(current_write_fd);
            }

            if (command_iter == command.sub_command().begin()) {
                job->set_job_id(job_counter_++);

                // TODO will impl bg jobs (&).
                job->set_status(Job::FOREGROUND);
                job->set_process_group_id(pid);
            }

            job->add_process_id(pid);


            if (command_iter + 1 == command.sub_command().end()) {
                // TODO handle if an error occurs.
                // TODO only unblock if command is not bg.
                signal_manager_->UnblockSignals();
                BlockForegroundJob(job);
                CleanUpJob(job);
            }
        }
    }
    return true;
}

bool
CommandManager::
CleanUpJob(Job* job) {
    for (int file_descriptor : job->open_file_descriptor()) {
        int close_val = close(file_descriptor);

        if (close_val == -1) {
            // TODO handle error value.
            return false;
        }
    }

    return true;
}

bool
CommandManager::
BlockForegroundJob(Job* job) {
    while (job->status() == Job_Status_FOREGROUND) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return true;
}

bool
CommandManager::
CreatePipes(const Command& command, Job* job) {
    for (int i = 0; i < (command.sub_command_size() - 1); i++) {
        // TODO implement
    }
    return false;
}

bool
CommandManager::
ClosePipes(const Job& job) {
    // TODO implement
    return false;
}

