load("@org_pubref_rules_protobuf//python:rules.bzl", "py_proto_library")

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

py_proto_library(
    name = "job-proto-py-lib",
    protos = [
        "job.proto",
    ],
)

py_proto_library(
    name = "shell-info-proto-py-lib",
    protos = [
        "shell_info.proto",
    ],
    proto_deps = [
        ":job-proto-py-lib"
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
    deps = [
        ":job-proto",
    ],
)