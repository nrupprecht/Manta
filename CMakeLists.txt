##################################################    Project     ##################################################
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project               (Manta VERSION 1.0 LANGUAGES CXX)
list                  (APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set_property          (GLOBAL PROPERTY USE_FOLDERS ON)
set                   (CMAKE_CXX_STANDARD 20)
set                   (CMAKE_CXX_VISIBILITY_PRESET hidden)
set                   (CMAKE_VISIBILITY_INLINES_HIDDEN 1)

include               (set_max_warning_level)
set_max_warning_level ()

include               (FetchContent)

##################################################    Options     ##################################################
option(BUILD_SHARED_LIBS "Build shared (dynamic) libraries." OFF)
option(BUILD_TESTS "Build tests." ON)
option(BUILD_APPLICATIONS "Build applications." ON)

##################################################    Sources     ##################################################
file(GLOB_RECURSE PROJECT_HEADERS include/manta/**.h include/manta/**.hpp)
file(GLOB_RECURSE PROJECT_SOURCES source/manta/**.c source/manta/**.cpp)
file(GLOB_RECURSE PROJECT_CMAKE_UTILS cmake/*.cmake)
file(GLOB_RECURSE PROJECT_MISC *.md *.txt)
set (PROJECT_EXPORT_HPP include/${PROJECT_NAME}/export.h)
set (PROJECT_FILES
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES}
        ${PROJECT_CMAKE_UTILS}
        ${PROJECT_MISC}
        ${PROJECT_EXPORT_HPP})

include            (assign_source_group)
assign_source_group(${PROJECT_FILES})

##################################################  Dependencies  ##################################################
include(conan)
include(conan_utility)
include(import_library)

add_conan_remote(bincrafter      https://api.bintray.com/conan/bincrafters/public-conan)
add_conan_remote(conan-community https://api.bintray.com/conan/conan-community/conan   )

# ADD NON-CONAN LIBRARIES HERE.
# Examples:
# - Header Only:
#   find_package  (GLM REQUIRED)
#   import_library(GLM_INCLUDE_DIRS)
# - Identical Debug and Release:
#   find_package  (OpenGL REQUIRED)
#   import_library(OPENGL_INCLUDE_DIR OPENGL_LIBRARIES)
# - Separate Debug and Release:
#   find_package  (Boost REQUIRED iostreams)
#   import_library(Boost_INCLUDE_DIRS Boost_IOSTREAMS_LIBRARY_DEBUG Boost_IOSTREAMS_LIBRARY_RELEASE)
# - You may also set the PROJECT_INCLUDE_DIRS and PROJECT_LIBRARIES instead of using import_library.

FetchContent_Declare(
        Lightning
        GIT_REPOSITORY https://github.com/nrupprecht/Lightning.git
        GIT_TAG        9b2760c
)
FetchContent_MakeAvailable(Lightning)
include_directories(${Lightning_SOURCE_DIR}/include)


conan_cmake_run(CONANFILE conanfile.py BASIC_SETUP BUILD missing)
list           (APPEND PROJECT_INCLUDE_DIRS ${CONAN_INCLUDE_DIRS_RELEASE})
list           (APPEND PROJECT_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_RELEASE})
foreach        (_LIB ${CONAN_LIBS_DEBUG})
    message(STATUS "  * Adding library ${_LIB} as debug")
    list         (APPEND PROJECT_LIBRARIES debug ${_LIB})
endforeach     ()
foreach        (_LIB ${CONAN_LIBS_RELEASE})
    message(STATUS "  * Adding library ${_LIB} as release")
    list         (APPEND PROJECT_LIBRARIES optimized ${_LIB})
endforeach     ()
foreach        (_LIB ${CONAN_LIBS})
    message(STATUS "  * Adding library ${_LIB}")
    list       (APPEND PROJECT_LIBRARIES ${_LIB})
endforeach ()

##################################################    Targets     ##################################################

# If I don't include this, the programs can't find their dynamically linked libraries.
# See: https://stackoverflow.com/questions/47697761/cmake-run-time-error-dyld-library-not-loaded-for-dynamically-linked-resource
if (${APPLE})
    set(CMAKE_MACOSX_RPATH OFF)
endif()

add_library(${PROJECT_NAME} ${PROJECT_FILES})
#set(CMAKE_MACOSX_RPATH OFF)
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include> PRIVATE source)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
target_link_libraries     (${PROJECT_NAME} PUBLIC ${PROJECT_LIBRARIES})
target_link_libraries     (${PROJECT_NAME} PUBLIC ${CONAN_LIBS})

target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_COMPILE_DEFINITIONS})
set_target_properties     (${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if(NOT BUILD_SHARED_LIBS)
    message(STATUS "  * NOTE: Build shared libs is off")
    string               (TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -D${PROJECT_NAME_UPPER}_STATIC)
endif()

# Set all warnings as errors.
target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W3 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Werror>)

##################################################    Postbuild   ##################################################
include               (GenerateExportHeader)
string                (TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
generate_export_header(${PROJECT_NAME}
        EXPORT_FILE_NAME     ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/export.h
        EXPORT_MACRO_NAME    ${PROJECT_NAME_UPPER}_EXPORT
        STATIC_DEFINE        ${PROJECT_NAME_UPPER}_STATIC)

##################################################    Testing     ##################################################
if(BUILD_TESTS)
    message(STATUS "Building tests...")
    enable_testing       ()
    set                  (TEST_MAIN_NAME test_main)
    set                  (TEST_MAIN_SOURCES UnitTest/internal/main.cpp)
    add_library          (${TEST_MAIN_NAME} OBJECT ${TEST_MAIN_SOURCES})
    target_link_libraries(${TEST_MAIN_NAME} ${PROJECT_NAME})
    set_property         (TARGET ${TEST_MAIN_NAME} PROPERTY FOLDER tests)
    assign_source_group  (${TEST_MAIN_SOURCES})

    # Find tests even in nexted directories.
    file(GLOB_RECURSE PROJECT_TEST_CPPS UnitTest/**UT_*.cpp)

    foreach(_SOURCE ${PROJECT_TEST_CPPS})
        get_filename_component(_NAME ${_SOURCE} NAME_WE)
        add_executable        (${_NAME} ${_SOURCE} $<TARGET_OBJECTS:${TEST_MAIN_NAME}>)
        target_link_libraries (${_NAME} ${PROJECT_NAME})
        # NOTE: I used to just have add_test(${_NAME} ${_NAME}) here, but then when I would try to run all CTests,
        #       it couldn't find the executables.
        add_test              (NAME ${_NAME} COMMAND ${_NAME})
        set_property          (TARGET ${_NAME} PROPERTY FOLDER tests)
        assign_source_group   (${_SOURCE})
    endforeach()
endif()

if (${BUILD_APPLICATIONS})
    message(STATUS "Building applications...")
    add_subdirectory("${PROJECT_SOURCE_DIR}/applications" "applications")
endif()

##################################################  Installation  ##################################################
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT  ${PROJECT_NAME}-config DESTINATION cmake)
export (TARGETS ${PROJECT_NAME}        FILE        ${PROJECT_NAME}-config.cmake)

