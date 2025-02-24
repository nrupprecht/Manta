load("//:tools.bzl", "default_opts")

cc_library(
    name="generatedparsers", 
    hdrs=glob(["*.h"]), 
    srcs=glob(["*.cpp"]), 
    visibility=["//visibility:public"], 
    copts = default_opts(), 
    deps=[
        "//manta:generator",
    ],
)