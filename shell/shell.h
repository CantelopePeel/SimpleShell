//
// Created by Liam Heeger on 8/30/18.
//

#ifndef SHELL_SHELL_H
#define SHELL_SHELL_H

#include <string>

#include "shell/proto/command.pb.h"

namespace shell {
    class Shell {
        public:
            bool Start();
            bool Exit();

        private:
            bool DelegateCommand(Command command);
            bool StartProcess();
    };
}


#endif //SHELL_SHELL_H
