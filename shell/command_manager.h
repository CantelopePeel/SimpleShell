//
// Created by Liam Heeger on 9/1/18.
//

#ifndef SHELL_COMMAND_MANAGER_H
#define SHELL_COMMAND_MANAGER_H

#include "shell/proto/command.pb.h"
#include "shell/proto/job.pb.h"

namespace shell {
    class CommandManager {
        public:
            bool Run(const Command& command, Job* job);

        private:
            unsigned int job_counter_ = 0;
    };
}


#endif //SHELL_COMMAND_MANAGER_H
