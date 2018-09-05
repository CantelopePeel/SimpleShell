//
// Created by Liam Heeger on 9/1/18.
//

#include "signal_manager.h"

#include <csignal>
#include <signal.h>
#include <sys/types.h>

using namespace shell;

namespace {

// TODO move to ShellInfoService
bool
GetForegroundJob(std::shared_ptr<ShellInfo> shell_info, Job* fg_job) {
    for (Job job : shell_info->job()) {
        if (job.status() == Job_Status_FOREGROUND) {
            fg_job = &job;
            return true;
        }
    }

    return false;
}

// TODO move to ShellInfoService
bool
GetJobByPid(std::shared_ptr<ShellInfo> shell_info, pid_t pid, Job* ret_job) {
    for (Job job : shell_info->job()) {
        for (int process_id : job.process_id()) {
            if (process_id == pid) {
                ret_job = &job;
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
    // Set up signal handlers.
    std::signal(SIGINT, &SignalManager::SigIntHandler);
    std::signal(SIGSTOP, &SignalManager::SigStopHandler);
    std::signal(SIGCHLD, &SignalManager::SigChldHandler);

    return true;
}

bool
SignalManager::
Stop() {
    // TODO reset signal handlers to default.
    return true;
}

void
SignalManager::
SetShellInfo(std::shared_ptr<ShellInfo> shell_info) {
    shell_info_ = shell_info;
}

void
SignalManager::
SigIntHandler(int signal) {
    Job fg_job;
    if (GetForegroundJob(instance->shell_info_, &fg_job)) {
        // TODO handle sys call error
        kill(-(fg_job.process_group_id()), SIGINT);
    } // TODO handle no fg job found.
}

void
SignalManager::
SigStopHandler(int signal) {
    Job fg_job;
    if (GetForegroundJob(instance->shell_info_, &fg_job)) {
        // TODO handle sys call error
        kill(-(fg_job.process_group_id()), SIGSTOP);
    } // TODO handle no fg job found.
}

void
SignalManager::
SigChldHandler(int signal) {
    // TODO implement.

    pid_t child_pid;
    int status;

    while ((child_pid = waitpid(-1, &status, (WNOHANG | WUNTRACED))) > 0) {
        // TODO do something with this. Possibly reap in a new JobService?
        Job job;

        if (GetJobByPid(instance->shell_info_, child_pid, &job)) {
            // TODO delete the job. Deliver a message for killed procs.
        }
    }
}