git_repository(
    name = "gtest",
    remote = "https://github.com/google/googletest",
    commit = "3306848f697568aacf4bcca330f6bdd5ce671899",
)

new_http_archive(
    name = "six_archive",
    url = "https://pypi.python.org/packages/16/d8/bc6316cf98419719bd59c91742194c111b6f2e85abac88e496adefaf7afe/six-1.11.0.tar.gz",
    build_file_content = """
py_library(
    name = "six",
    srcs = ["six.py"],
    visibility = ["//visibility:public"],
)
    """,
    strip_prefix = "six-1.11.0",
)

bind(
    name = "six",
    actual = "@six_archive//:six",
)

http_archive(
    name = "org_pubref_rules_protobuf",
    strip_prefix = "rules_protobuf-0.8.1",
    urls = ["https://github.com/pubref/rules_protobuf/archive/v0.8.1.tar.gz"],
    sha256 = "fb9852446b5ba688cd7178a60ff451623e4112d015c6adfe0e9a06c5d2dedc08"
)

load("@org_pubref_rules_protobuf//python:rules.bzl", "py_proto_repositories")
py_proto_repositories()


# TODO change to newer commit.


# new_http_archive(
#    name = "gtest",
#    url = "https://github.com/google/googletest/archive/release-1.7.0.zip",
#    sha256 = "b58cb7547a28b2c718d1e38aee18a3659c9e3ff52440297e965f5edffe34b6d0",
#    strip_prefix = "googletest-release-1.7.0",
#    build_file = "gtest.BUILD",
# )
