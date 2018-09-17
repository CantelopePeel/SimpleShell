//
// Created by Liam Heeger on 9/12/18.
//

#ifndef SHELL_SHELL_MACROS_H
#define SHELL_SHELL_MACROS_H

#include <errno.h>

#define SYSCALL(call) \
if ((call) < 0) { \
    perror("Error"); \
}

#define SYSCALL_RET(call) \
if ((call) < 0) { \
    perror("Error"); \
    return false; \
}

#define SYSCALL_EXIT(call) \
if ((call) < 0) { \
    int error_code = errno; \
    perror("Error"); \
    std::exit(error_code); \
}


#endif //SHELL_SHELL_MACROS_H
