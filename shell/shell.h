//
// Created by Liam Heeger on 8/30/18.
//

#ifndef SHELL_SHELL_H
#define SHELL_SHELL_H

#include <memory>
#include <string>

#include "shell/proto/command.pb.h"
#include "shell/proto/shell_info.pb.h"
#include "command_manager.h"
#include "signal_manager.h"


namespace shell {
    class Shell {
        public:
            Shell();

            bool Start();
            bool Exit();

        private:
            bool ProcessCommandLine(const std::string& command_line);
            bool DelegateCommand(Command command);
            bool StartProcess();

            std::shared_ptr<ShellInfo> shell_info_;
            CommandManager command_manager_;
            SignalManager signal_manager_;
    };
}


#endif //SHELL_SHELL_H
