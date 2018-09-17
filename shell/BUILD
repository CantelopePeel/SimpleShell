package(default_visibility = ["//shell:__subpackages__"])

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
        ":shell-macros-lib",
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
        ":shell-macros-lib",
        "//shell/proto:shell-info-proto-lib",
        "//shell/proto:job-proto-lib",
    ],
)

cc_library(
    name = "shell-macros-lib",
    hdrs = [
        "shell_macros.h",
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

py_binary(
    name = "shell-test",
    srcs = [
        "shell-test.py",
    ],
    deps = [
        "//shell/proto:shell-info-proto-py-lib",
        "@com_google_protobuf//:protobuf_python",
    ],
    data = [
        ":success_commands.sh",
        ":failure_commands.sh",
        ":shell-exec",
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
