//
// Created by Liam Heeger on 9/1/18.
//

#include "command_manager.h"
#include "shell_macros.h"

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

        if (command.sub_command_size() > 1) {
            if (command_iter + 1 != command.sub_command().end()) {
                // Pipe input/output.

                SYSCALL_RET(pipe(pipe_fd));

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

        if (!signal_manager_->BlockSignals()) {
            return false;
        }

        pid_t pid;
        SYSCALL_RET(pid = fork());

        // Convert command arguments.
        char* arguments[command_iter->argument_size() + 2];

        arguments[0] = const_cast<char*>(command_iter->program().c_str());
        for (int i = 0; i < command_iter->argument_size(); i++) {
            arguments[i + 1] = const_cast<char*>(command_iter->argument(i).c_str());
        }
        arguments[command_iter->argument_size() + 1] = nullptr;


        if (pid == 0) { // Child Process
            if (!signal_manager_->UnblockSignals()) {
                std::exit(1);
            }

            if (current_read_fd != STDIN_FILENO) {
                SYSCALL_EXIT(dup2(current_read_fd, STDIN_FILENO));
                SYSCALL_EXIT(close(current_read_fd));
            }

            if (current_write_fd != STDOUT_FILENO) {
                SYSCALL_EXIT(dup2(current_write_fd, STDOUT_FILENO));
                SYSCALL_EXIT(close(current_write_fd));
            }

            SYSCALL_EXIT(execvp(command_iter->program().c_str(), arguments))
        } else if (pid > 0) { // Parent Process
            if (current_write_fd != STDOUT_FILENO) {
                SYSCALL_RET(close(current_write_fd));
            }

            if (command_iter == command.sub_command().begin()) {
                job->set_command(command.original_command());
                job->set_job_id(job_counter_++);

                if (command.background()) {
                    job->set_status(Job::BACKGROUND);
                } else {
                    job->set_status(Job::FOREGROUND);
                }
                job->set_process_group_id(pid);
            }

            job->add_process_id(pid);

            SYSCALL_RET(setpgid(pid, job->process_group_id()));

            if (command_iter + 1 == command.sub_command().end()) {
                if (!signal_manager_->UnblockSignals()) {
                    return false;
                }

                if (command.background()) {
                    std::printf("[%d]: pgid: %d status: %s : %s\n", job->job_id(), job->process_group_id(),
                                Job::Status_Name(job->status()).c_str(), job->command().c_str());
                } else {
                    BlockForegroundJob(job);
                }
            }
        }
    }
    return true;
}

void
CommandManager::
BlockForegroundJob(Job* job) {
    while (job->status() == Job_Status_FOREGROUND) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

