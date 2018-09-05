//
// Created by Liam Heeger on 9/1/18.
//

#ifndef SHELL_COMMAND_MANAGER_H
#define SHELL_COMMAND_MANAGER_H

#include <memory>

#include "shell/proto/command.pb.h"
#include "shell/proto/shell_info.pb.h"
#include "shell/proto/job.pb.h"

namespace shell {
    // TODO make singleton with DI
    class CommandManager {
        public:
            CommandManager(std::shared_ptr<ShellInfo> shell_info);

            bool Run(const Command& command, Job* job);

        private:
            bool CleanUpJob(const Job& job);
            bool ClosePipes(const Job& job);
            bool CreatePipes(const Command& command, Job* job);

            std::shared_ptr<ShellInfo> shell_info_;
            unsigned int job_counter_ = 0;

    };
}


#endif //SHELL_COMMAND_MANAGER_H
