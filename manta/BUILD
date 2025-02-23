
cc_library(
    name="generator",
    hdrs=glob(["generator/**/*.h", "parser/**/*.h", "visualization/**/*.h", "typesystem/**/*.h", "*.h"]),
    srcs=glob(["generator/**/*.cpp", "parser/**/*.cpp", "visualization/**/*.cpp", "typesystem/**/*.cpp", "*.cpp"]),
    visibility=["//visibility:public"],
    copts = ["-std=c++20"],
    deps=[
        "//manta/lexer",
        "@lightning//include:lightning",
    ],
)