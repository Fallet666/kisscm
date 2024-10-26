cmake_minimum_required(VERSION 3.28)
project(assembler)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_library(ass_lib
        Assemble.cpp
)
add_library(inter_lib
        Interpret.cpp
)

FIND_PACKAGE(Boost COMPONENTS json REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tests tests.cpp)

target_link_libraries(tests ass_lib inter_lib)
target_link_libraries(tests GTest::GTest GTest::Main)

add_executable(assembler assembler.cpp)
add_executable(interpreter interpreter.cpp)

target_link_libraries(assembler ass_lib)
target_link_libraries(interpreter inter_lib)
