//
// Created by Liam Heeger on 8/30/18.
//

#ifndef SHELL_SHELL_H
#define SHELL_SHELL_H

#include <string>

#include "shell/proto/command.pb.h"
#include "shell/proto/shell_info.pb.h"
#include "command_manager.h"


namespace shell {
    class Shell {
        public:
            bool Start();
            bool Exit();

        private:
            bool ProcessCommandLine(const std::string& command_line);
            bool DelegateCommand(Command command);
            bool StartProcess();

            ShellInfo shell_info_;
            CommandManager command_manager_;
    };
}


#endif //SHELL_SHELL_H
