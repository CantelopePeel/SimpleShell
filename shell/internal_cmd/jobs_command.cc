//
// Created by Liam Heeger on 8/30/18.
//

#include "jobs_command.h"

using namespace shell;

JobsCommand::JobsCommand(const Command& command) : InternalCommand(command) {}

void
JobsCommand::
Run(ShellInfo* shell_info, std::string* output) {
    // TODO implement printing of jobs.
}
