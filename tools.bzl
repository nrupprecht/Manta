
def default_opts():
    return ["-std=c++20", "-Wpedantic", "-Wall", "-Wextra"]


def manta_binary(src):
    """Create a manta based binary."""
    name = src.split('.')[0]

    native.cc_binary(
        name=name,
        srcs=[src],
        deps=[
            "//manta/generatedparsers"
        ],
        visibility=["//visibility:public"],
        copts = default_opts(),
    )


def manta_unit_test(src):
    """Create a manta based unit test."""
    native.cc_test(
        name = src.split('.')[0],
        size = "small",
        srcs = [src],
        copts = default_opts(),
        deps = [
            "@googletest//:gtest",
            "@googletest//:gtest_main",
            "@lightning//:lightning",
            "//manta:generator",
        ],
    )


def create_unit_tests(srcs):
    for src in srcs:
        manta_unit_test(src=src)