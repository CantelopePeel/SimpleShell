//
// Created by Liam Heeger on 9/1/18.
//

#include "signal_manager.h"

#include <csignal>

using namespace shell;

SignalManager::
SignalManager(std::shared_ptr<ShellInfo> shell_info)
        : shell_info_(shell_info) {}

bool
SignalManager::
Start() {
    std::signal(SIGINT, SIG_IGN);
    return true;
}

bool
SignalManager::
Stop() {
    std::signal(SIGINT, SIG_DFL);
    return true;
}
