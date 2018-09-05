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
            static SignalManager* GetInstance();

            bool Start();
            bool Stop();

            void SetShellInfo(std::shared_ptr<ShellInfo> shell_info);

        private:
            SignalManager();

            static SignalManager* instance;

            static void SigIntHandler(int signal);
            static void SigStopHandler(int signal);
            static void SigChldHandler(int signal);

            std::shared_ptr<ShellInfo> shell_info_;

    };
}


#endif //SHELL_SIGNAL_MANAGER_H
