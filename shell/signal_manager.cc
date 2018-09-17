//
// Created by Liam Heeger on 9/1/18.
//

#include "signal_manager.h"
#include "shell_macros.h"

#include <csignal>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace shell;

namespace {

bool
GetForegroundJob(std::shared_ptr<ShellInfo> shell_info, Job* fg_job) {
    for (const Job& job : shell_info->job()) {
        if (job.status() == Job_Status_FOREGROUND) {
            fg_job->CopyFrom(job);
            return true;
        }
    }

    return false;
}

bool
SetForegroundJobStatus(std::shared_ptr<ShellInfo> shell_info, const Job_Status& status) {
    for (auto job_iter = shell_info->mutable_job()->pointer_begin();
         job_iter !=  shell_info->mutable_job()->pointer_end();
         job_iter++) {
        if ((*job_iter)->status() == Job_Status_FOREGROUND) {
            (*job_iter)->set_status(status);
            return true;
        }
    }

    return false;
}

bool
GetJobIdByPid(std::shared_ptr<ShellInfo> shell_info, pid_t pid, int* job_id) {
    for (const Job& job : shell_info->job()) {
        for (int process_id : job.process_id()) {
            if (process_id == pid) {
                *job_id = job.job_id();
                return true;
            }
        }
    }

    return false;
}

}

SignalManager* SignalManager::instance = nullptr;

SignalManager::
SignalManager() {}

SignalManager*
SignalManager::
GetInstance() {
    if (instance == nullptr) {
        instance = new SignalManager();
    }

    return instance;
}

bool
SignalManager::
Start() {
    // Set up signal mask.
    SYSCALL_RET(sigemptyset(&signal_mask_));
    SYSCALL_RET(sigaddset(&signal_mask_, SIGINT));
    SYSCALL_RET(sigaddset(&signal_mask_, SIGTSTP));
    SYSCALL_RET(sigaddset(&signal_mask_, SIGCHLD));

    // Set up signal handlers.
    std::signal(SIGINT, &SignalManager::SigIntHandler);
    std::signal(SIGTSTP, &SignalManager::SigTstpHandler);
    std::signal(SIGCHLD, &SignalManager::SigChldHandler);

    return true;
}

bool
SignalManager::
Stop() {
    return true;
}

void
SignalManager::
SetShellInfo(std::shared_ptr<ShellInfo> shell_info) {
    shell_info_ = shell_info;
}

bool
SignalManager::
BlockSignals() {
    SYSCALL_RET(sigprocmask(SIG_BLOCK, &signal_mask_, nullptr));
    return true;
}

bool
SignalManager::
UnblockSignals() {
    SYSCALL_RET(sigprocmask(SIG_UNBLOCK, &signal_mask_, nullptr));
    return true;
}

void
SignalManager::
SigIntHandler(int signal) {
    Job fg_job;
    if (GetForegroundJob(instance->shell_info_, &fg_job)) {
        SYSCALL(kill(-(fg_job.process_group_id()), SIGINT));
    }
}

void
SignalManager::
SigTstpHandler(int signal) {
    Job fg_job;
    if (GetForegroundJob(instance->shell_info_, &fg_job)) {
        SYSCALL(kill(-(fg_job.process_group_id()), SIGTSTP));
    }
}

void
SignalManager::
SigChldHandler(int signal) {
    pid_t child_pid;
    int status;

    while ((child_pid = waitpid(-1, &status, (WNOHANG | WUNTRACED))) > 0) {
        int job_id;

        // TODO Support different wait statuses.
        if (GetJobIdByPid(instance->shell_info_, child_pid, &job_id)) {
            if (WIFSTOPPED(status)) {
                // TODO print a message about job changing.
                SetJobStatus(instance->shell_info_, job_id, Job::STOPPED);
            } else if (WIFSIGNALED(status)) {
                SetJobStatus(instance->shell_info_, job_id, Job::UNDEFINED);
                CleanUpJob(instance->shell_info_, job_id);
            } else if (WIFEXITED(status)) {
                SetJobStatus(instance->shell_info_, job_id, Job::UNDEFINED);
                CleanUpJob(instance->shell_info_, job_id);
            } else {
                std::cerr << "Error: Job status could not be determined." << std::endl;
            }

        } else {
            std::cerr << "Error: Job not found with a PID when signaled." << std::endl;
        }
    }
}

bool
SignalManager::
ContinueJob(std::shared_ptr<ShellInfo> shell_info, int job_id) {
    for (auto job_iter = shell_info->mutable_job()->pointer_begin();
         job_iter !=  shell_info->mutable_job()->pointer_end();
         job_iter++) {
        if ((*job_iter)->job_id() == job_id && (*job_iter)->status() != Job::UNDEFINED) {
            SYSCALL_RET(kill(-(*job_iter)->process_group_id(), SIGCONT));
            return true;
        }
    }

    std::cerr << "Error: No such job was found. No job was continued." << std::endl;
    return false;
}

bool
SignalManager::
SetJobStatus(std::shared_ptr<ShellInfo> shell_info, int job_id, const Job_Status& status) {
    for (auto job_iter = shell_info->mutable_job()->pointer_begin();
         job_iter !=  shell_info->mutable_job()->pointer_end();
         job_iter++) {
         if ((*job_iter)->job_id() == job_id) {
            bool is_bg = (status == Job::BACKGROUND || (*job_iter)->status() == Job::BACKGROUND);
            (*job_iter)->set_status(status);

            if (is_bg) {
                std::printf("\n[%d]: pgid: %d status: %s : %s\n", (*job_iter)->job_id(), (*job_iter)->process_group_id(),
                            Job::Status_Name((*job_iter)->status()).c_str(), (*job_iter)->command().c_str());
            }
            return true;
        }
    }

    std::cerr << "Error: No such job was found. Status not set." << std::endl;
    return false;
}

bool
SignalManager::
CleanUpJob(std::shared_ptr<ShellInfo> shell_info, int job_id) {
    for (auto job_iter = shell_info->mutable_job()->pointer_begin();
         job_iter !=  shell_info->mutable_job()->pointer_end();
         job_iter++) {
        if ((*job_iter)->job_id() == job_id) {
            for (int open_file_descriptor : (*job_iter)->open_file_descriptor()) {
                SYSCALL_RET(close(open_file_descriptor));
            }
            return true;
        }
    }

    std::cerr << "Error: No such job was found. No job was cleaned up." << std::endl;
    return false;
}
