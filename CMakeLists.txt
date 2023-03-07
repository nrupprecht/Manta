cmake_minimum_required(VERSION 3.14)
project(MantaClion)

include_directories(include)

# ---------
include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
# --------

set(CMAKE_CXX_STANDARD 20)

add_executable(
        MantaClion
        applications/main.cpp
        # Other
        source/manta/lexer/DeterministicFiniteAutomata.cpp
        source/manta/lexer/LexerDFA.cpp
        source/manta/parser/ParserUtility.cpp
        source/manta/parser/LALRParser.cpp
        source/manta/parser/ParseNode.cpp
        source/manta/generator/LexerGenerator.cpp
        source/manta/generator/ParserGenerator.cpp
        source/manta/visualization/Display.cpp
)
)

# ======
enable_testing()

add_executable(
        UnitTest
        UnitTest/UT_Lexer.cpp
        UnitTest/UT_FirstFollow.cpp
        UnitTest/UT_SelfParse.cpp
        # Other
        source/manta/lexer/DeterministicFiniteAutomata.cpp
        source/manta/lexer/LexerDFA.cpp
        source/manta/parser/ParserUtility.cpp
        source/manta/parser/LALRParser.cpp
        source/manta/parser/ParseNode.cpp
        source/manta/generator/LexerGenerator.cpp
        source/manta/generator/ParserGenerator.cpp
        source/manta/visualization/Display.cpp
        UnitTest/UT_Precedence.cpp
)

target_link_libraries(
        UnitTest
        gtest_main
)



include(GoogleTest)
gtest_discover_tests(UnitTest)