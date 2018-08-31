package(default_visibility = ["//shell:__subpackages__"])

cc_proto_library(
    name = "command-proto-lib",
    deps = [
        ":command-proto",
    ],
)

cc_proto_library(
    name = "job-proto-lib",
    deps = [
        ":job-proto",
    ],
)

cc_proto_library(
    name = "shell-info-proto-lib",
    deps = [
        ":shell-info-proto",
    ],
)

proto_library(
    name = "command-proto",
    srcs = [
        "command.proto",
    ],
)

proto_library(
    name = "job-proto",
    srcs = [
        "job.proto",
    ],
)

proto_library(
    name = "shell-info-proto",
    srcs = [
        "shell_info.proto",
    ],
)