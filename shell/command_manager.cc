//
// Created by Liam Heeger on 9/1/18.
//

#include "command_manager.h"

#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char** environ;

using namespace shell;

CommandManager::
CommandManager(std::shared_ptr<ShellInfo> shell_info)
        : shell_info_(shell_info) {}

bool
CommandManager::
Run(const Command& command, Job* job) {
    int read_fd = -1;
    for (auto command_iter = command.sub_command().begin(); command_iter != command.sub_command().end();
            command_iter++) {
        pid_t pid = fork();

        if (pid == -1) {
            // TODO handle error with message.
            return false;
        }

        char* arguments[command.sub_command(0).argument_size() + 2];

        arguments[0] = const_cast<char*>(command.sub_command(0).program().c_str());
        for (int i = 0; i < command.sub_command(0).argument_size(); i++) {
            arguments[i + 1] = const_cast<char*>(command.sub_command(0).argument(i).c_str());
        }
        arguments[command.sub_command(0).argument_size() + 1] = nullptr;


        if (pid == 0) { // Child Process
            if (command.sub_command_size() > 1) {
                if (command_iter != command.sub_command().begin()) {
                    // Pipe input to prior output file descriptor.
                    dup2(read_fd, STDIN_FILENO);
                }

                if (command_iter + 1 != command.sub_command().end()) {
                    // Pipe input/output.
                    int pipe_fd[2];

                    int pipe_val = pipe(pipe_fd);
                    if (pipe_val == -1) {
                        // TODO handle pipe creation error.
                        return false;
                    }

                    // Add file descriptors to job.
                    job->add_open_file_descriptor(pipe_fd[0]);
                    job->add_open_file_descriptor(pipe_fd[1]);

                    // Change stdout to write end of pipe.
                    dup2(pipe_fd[1], STDOUT_FILENO);
                    read_fd = pipe_fd[0];
                }
            }

            int exec_val = execvp(command.sub_command(0).program().c_str(), arguments);
            if (exec_val < 0) {
                // TODO give a verbose error message.
                std::cout << "ERROR" << std::endl;
                std::exit(0);
                return false;
            }
        } else if (pid > 0) { // Parent Process
            job->add_process_id(pid);

            if (command_iter + 1 == command.sub_command().end()) {
                // TODO handle if an error occurs.
                waitpid(pid, nullptr, 0);
                CleanUpJob(*job);
            }
        }
    }
    job->set_job_id(job_counter_++);
    return true;
}

bool
CommandManager::
CleanUpJob(const Job& job) {
    for (int file_descriptor : job.open_file_descriptor()) {
        int close_val = close(file_descriptor);

        if (close_val == -1) {
            // TODO handle error value.
            return false;
        }
    }

    return true;
}