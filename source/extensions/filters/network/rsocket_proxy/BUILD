licenses(["notice"])  # Apache 2

# RSocket L4 network filter. This is primarily a simplistic example.
# Public docs: docs/root/configuration/network_filters/rsocket_proxy.rst

load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
    "envoy_package",
)

envoy_package()

envoy_cc_library(
    name = "rsocket",
    srcs = ["frame.cpp","payload.cpp","rsocket.cc"],
    hdrs = ["frame.h","payload.h","rsocket.h"],
    deps = [
        "//include/envoy/buffer:buffer_interface",
        "//include/envoy/network:connection_interface",
        "//include/envoy/network:filter_interface",
        "//source/common/common:assert_lib",
        "//source/common/common:minimal_logger_lib",
    ],
)

envoy_cc_library(
    name = "config",
    srcs = ["config.cc"],
    deps = [
        ":rsocket",
        "//include/envoy/registry",
        "//include/envoy/server:filter_config_interface",
        "//source/extensions/filters/network:well_known_names",
        "//source/extensions/filters/network/common:factory_base_lib",
    ],
)
