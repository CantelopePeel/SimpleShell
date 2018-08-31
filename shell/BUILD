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
        ":shell-utils-lib",
    ],
)

cc_test(
    name = "shell-lib_test",
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
        "//shell:shell-lib",
    ],
)
