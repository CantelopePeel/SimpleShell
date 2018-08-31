cc_library(
    name = "internal-command-lib",
    hdrs = [
        "internal_command.h",
    ],
    srcs = [
        "internal_command.cc",
    ],
    deps = [
        "//shell/proto:command-proto-lib",
        "//shell/proto:shell-info-proto-lib",
    ],
)

cc_library(
    name = "internal-cmds-lib",
    hdrs = [
        "jobs_command.h",
        "exit_command.h",
        "cd_command.h",
    ],
    srcs = [
        "jobs_command.cc",
        "exit_command.cc",
        "cd_command.cc",
    ],
    deps = [
        ":internal-command-lib",
    ],
)