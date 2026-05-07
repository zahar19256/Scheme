cmake_minimum_required(VERSION 3.13)

project(scheme_basic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT DEFINED SCHEME_COMMON_DIR)
    set(SCHEME_COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif ()

if (NOT COMMAND add_catch)
    add_library(contrib_catch_main
        third_party/catch/catch_main.cpp)

    target_include_directories(contrib_catch_main
        PUBLIC third_party/catch)

    function(add_catch TARGET)
        add_executable(${TARGET} ${ARGN})
        target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
        target_link_libraries(${TARGET} contrib_catch_main)
    endfunction()
endif ()

set(BASIC_TESTS
    # from tokenizer
    tests/test_tokenizer.cpp

    # from parser
    tests/test_parser.cpp

    tests/test_boolean.cpp
    tests/test_eval.cpp
    tests/test_integer.cpp
    tests/test_list.cpp
    tests/test_fuzzing_2.cpp)

add_catch(test_scheme_basic
    ${BASIC_TESTS})

include(sources.cmake)

target_include_directories(scheme_basic PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SCHEME_COMMON_DIR})

target_link_libraries(test_scheme_basic scheme_basic)

add_executable(scheme_basic_repl repl/main.cpp)
target_link_libraries(scheme_basic_repl scheme_basic)
