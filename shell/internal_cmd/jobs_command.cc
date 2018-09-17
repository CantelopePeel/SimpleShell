//
// Created by Liam Heeger on 8/30/18.
//

#include "jobs_command.h"
#include "shell/proto/job.pb.h"

#include <cstdlib>

using namespace shell;

JobsCommand::JobsCommand(const Command& command) : InternalCommand(command) {}

void
JobsCommand::
Run(ShellInfo* shell_info, std::string* output) {
    for (const Job& job : shell_info->job()) {
        if (job.status() != Job::UNDEFINED) {
            std::printf("[%d]: pgid: %d status: %s : %s\n", job.job_id(), job.process_group_id(),
                        Job::Status_Name(job.status()).c_str(), job.command().c_str());
            for (int process_id : job.process_id()) {
                std::printf("  (pid: %d)\n", process_id);
            }
        }
    }
}
