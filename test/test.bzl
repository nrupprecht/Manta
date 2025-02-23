
def manta_unit_test(src):
    native.cc_test(
        name = src.split('.')[0],
        size = "small",
        srcs = [src],
        copts = ["-std=c++20"],
        deps = [
            "@googletest//:gtest",
            "@googletest//:gtest_main",
            "@lightning//include:lightning",
            "//manta/lexer",
            "//manta:generator",
        ],
    )

def create_unit_tests(srcs):
    for src in srcs:
        manta_unit_test(src=src)