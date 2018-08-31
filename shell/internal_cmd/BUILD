package(default_visibility = ["//shell:__subpackages__"])

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
        "cd_command.cc",
        "exit_command.cc",
        "jobs_command.cc",
    ],
    deps = [
        ":internal-command-lib",
    ],
)