cc_library(
    name = "command-manager-lib",
    hdrs = [
        "command_manager.h",
    ],
    srcs = [
        "command_manager.cc",
    ],
    deps = [
        ":signal-manager-lib",
        "//shell/proto:shell-info-proto-lib",
        "//shell/proto:command-proto-lib",
        "//shell/proto:job-proto-lib",
    ],
)

cc_library(
    name = "signal-manager-lib",
    hdrs = [
        "signal_manager.h",
    ],
    srcs = [
        "signal_manager.cc",
    ],
    deps = [
        "//shell/proto:shell-info-proto-lib",
        "//shell/proto:job-proto-lib",
    ],
)

cc_library(
    name = "shell-utils-lib",
    hdrs = [
        "shell_utils.h",
    ],
    srcs = [
        "shell_utils.cc",
    ],
    deps = [
        "//shell/proto:command-proto-lib",
        "//shell/proto:shell-info-proto-lib",
        "//shell/proto:job-proto-lib",
    ],
)

cc_library(
    name = "shell-lib",
    hdrs = [
        "shell.h",
    ],
    srcs = [
        "shell.cc",
    ],
    deps = [
        "//shell/internal_cmd:internal-cmds-lib",
        ":shell-utils-lib",
        ":command-manager-lib",
        ":signal-manager-lib",
    ],
)

cc_test(
    name = "shell-util_test",
    srcs = [
        "shell_utils_test.cc",
    ],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        ":shell-utils-lib",
        "@gtest//:gtest",
        "@gtest//:gtest_main",
    ],
)

cc_binary(
    name = "shell-exec",
    srcs = [
        "shell_exec.cc",
    ],
    deps = [
        ":shell-lib",
    ],
)
