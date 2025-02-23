
def manta_binary(src):
    name = src.split('.')[0]

    native.cc_binary(
        name=name,
        srcs=[src],
        deps=[
            "@lightning//include:lightning",
            "//manta/lexer",
            "//manta:generator",
            "//manta/generatedparsers"
        ],
        visibility=["//visibility:public"],
        copts = ["-std=c++20"],
    )