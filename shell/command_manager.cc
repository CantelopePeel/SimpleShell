//
// Created by Liam Heeger on 9/1/18.
//

#include "command_manager.h"

#include <vector>
#include <util>
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
    int prior_read_fd = -1;
    int write_fd = -1;
    int read_fd = -1;
    for (auto command_iter = command.sub_command().begin(); command_iter != command.sub_command().end();
            command_iter++) {
        if (command.sub_command_size() > 1) {
            if (command_iter + 1 != command.sub_command().end()) {
                // Pipe input/output.
                int pipe_fd[2];

                int pipe_val = pipe(pipe_fd);
                if (pipe_val == -1) {
                    // TODO handle pipe creation error.
                    return false;
                }

                read_fd = pipe_fd[0];
                write_fd = pipe_fd[1];
                // Add file descriptors to job.
                job->add_open_file_descriptor(pipe_fd[0]);
                job->add_open_file_descriptor(pipe_fd[1]);
            }
        }

        pid_t pid = fork();

        if (pid == -1) {
            // TODO handle error with message.
            return false;
        }

        char* arguments[command_iter->argument_size() + 2];

        arguments[0] = const_cast<char*>(command_iter->program().c_str());
        for (int i = 0; i < command_iter->argument_size(); i++) {
            arguments[i + 1] = const_cast<char*>(command_iter->argument(i).c_str());
        }
        arguments[command_iter->argument_size() + 1] = nullptr;


        if (pid == 0) { // Child Process
            std::cout << "Child Proc: " << command_iter->DebugString() << std::endl;
            if (command.sub_command_size() > 1) {
                if (command_iter != command.sub_command().begin()) {
                    std::cout << command_iter->program() << " IN FD: " << prior_read_fd << std::endl;
                    // Pipe input to prior read file descriptor.
                    int dup2_val = dup2(STDIN_FILENO, prior_read_fd);

                    if (dup2_val == -1) {
                        // TODO error message and proper clean up.
                        std::cout << "err2" << std::endl;
                        return false;
                    }

                    close(prior_read_fd);
                    close(write_fd);
                } else {
                    std::cout << command_iter->program() << " IN FD: " << STDIN_FILENO << std::endl;
                }

                if (command_iter + 1 != command.sub_command().end()) {
                    // Change stdout to write end of pipe.
                    std::cout << "OUT FD: " << write_fd << std::endl;
                    int dup2_val = dup2(STDOUT_FILENO, write_fd);

                    if (dup2_val == -1) {
                        // TODO error message and proper clean up.
                        std::cout << "err1" << std::endl;
                        return false;
                    }

                    close(read_fd);
                    close(write_fd);
                } else {
                    std::cout << "OUT FD: " << STDOUT_FILENO << std::endl; // TODO remove
                }
            }

            int exec_val = execvp(command_iter->program().c_str(), arguments);
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

        prior_read_fd = read_fd;
    }
    job->set_job_id(job_counter_++);
    return true;
}

bool
CommandManager::
CreatePipes(const Command& command, Job* job) {
    for (int i = 0; i < (command.sub_command_size() - 1); i++) {
        // TODO implement
    }
}

bool
CommandManager::
ClosePipes(JobsCommand) {
    // TODO implement
    return false;
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