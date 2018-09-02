//
// Created by Liam Heeger on 9/1/18.
//

#ifndef SHELL_SIGNAL_MANAGER_H
#define SHELL_SIGNAL_MANAGER_H

#include <memory>

#include "shell/proto/shell_info.pb.h"

namespace shell {
    class SignalManager {
        public:
            SignalManager(std::shared_ptr<ShellInfo> shell_info);

            bool Start();
            bool Stop();

        private:
            std::shared_ptr<ShellInfo> shell_info_;
    };
}

#endif //SHELL_SIGNAL_MANAGER_H
