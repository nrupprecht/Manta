load("//:tools.bzl", "default_opts")

cc_library(
    name="generator",
    hdrs=glob(["generator/**/*.h", "parser/**/*.h", "visualization/**/*.h"]),
    srcs=glob(["generator/**/*.cpp", "parser/**/*.cpp", "visualization/**/*.cpp"]),
    visibility=["//visibility:public"],
    copts = default_opts(),
    deps=[
        "//manta/lexer",
    ],
)