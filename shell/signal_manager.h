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

            void SetShellInfo(std::shared_ptr<ShellInfo> shell_info);

            bool BlockSignals();
            bool UnblockSignals();

            static bool ContinueJob(std::shared_ptr<ShellInfo> shell_info, int job_id);
            static bool SetJobStatus(std::shared_ptr<ShellInfo> shell_info, int job_id, const Job_Status& status);
        private:
            SignalManager();

            static SignalManager* instance;

            static bool CleanUpJob(std::shared_ptr <ShellInfo> shared_ptr, unsigned int job_id);

            static void SigIntHandler(int signal);
            static void SigTstpHandler(int signal);
            static void SigChldHandler(int signal);

            sigset_t signal_mask_;
            std::shared_ptr<ShellInfo> shell_info_;

    };
}


#endif //SHELL_SIGNAL_MANAGER_H
