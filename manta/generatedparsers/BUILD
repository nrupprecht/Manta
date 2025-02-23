
cc_library(
    name="generatedparsers", 
    hdrs=glob(["*.h"]), 
    srcs=glob(["*.cpp"]), 
    visibility=["//visibility:public"], 
    copts = ["-std=c++20"], 
    deps=[
        "@lightning//include:lightning",
        "//manta/lexer",
        "//manta:generator",
    ],
)