//
// Created by Liam Heeger on 8/30/18.
//

#ifndef SHELL_SHELL_UTILS_H
#define SHELL_SHELL_UTILS_H

#include "shell/proto/command.pb.h"
#include "shell/proto/shell_info.pb.h"
#include "shell/proto/job.pb.h"

namespace shell {
    class ShellUtils {
        public:
            static bool ParseCommand(const std::string& command_line, Command* command);
            static bool IsInteralCommand(const Command& command);

            static const Job* GetForegroundJob(std::shared_ptr<ShellInfo> shell_info);
    };
}


#endif //SHELL_SHELL_UTILS_H
