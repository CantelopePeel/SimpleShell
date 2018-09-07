//
// Created by Liam Heeger on 9/1/18.
//

#ifndef SHELL_SIGNAL_MANAGER_H
#define SHELL_SIGNAL_MANAGER_H

#include <memory>
#include <signal.h>
#include <sys/types.h>

#include "shell/proto/shell_info.pb.h"

namespace shell {
    class SignalManager {
        public:
            static SignalManager* GetInstance();

            bool Start();
            bool Stop();

            // TODO add a private init function which shell::Shell as friend can call to provide Dep Injection.
            // TODO impl a reset function.

            void SetShellInfo(std::shared_ptr<ShellInfo> shell_info);

            bool BlockSignals();
            bool UnblockSignals();

        private:
            SignalManager();

            static SignalManager* instance;

            static void SigIntHandler(int signal);
            static void SigTstpHandler(int signal);
            static void SigChldHandler(int signal);

            sigset_t signal_mask_;
            std::shared_ptr<ShellInfo> shell_info_;
    };
}


#endif //SHELL_SIGNAL_MANAGER_H
