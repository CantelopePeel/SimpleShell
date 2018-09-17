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
        "cd_command.h",
        "exit_command.h",
        "jobs_command.h",
        "bg_command.h",
        "fg_command.h",
        "shell_info_command.h",
    ],
    srcs = [
        "cd_command.cc",
        "exit_command.cc",
        "jobs_command.cc",
        "bg_command.cc",
        "fg_command.cc",
        "shell_info_command.cc",
    ],
    deps = [
        ":internal-command-lib",
        "//shell:shell-macros-lib",
    ],
)