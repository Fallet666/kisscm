cmake_minimum_required(VERSION 3.28)
project(assembler)

set(CMAKE_CXX_STANDARD 17)

enable_testing()
include_directories(interpret)
include_directories(assemble)
add_library(ass_lib
        assemble/Assemble.cpp
)
add_library(inter_lib
        interpret/Interpret.cpp
)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tests tests/tests.cpp)

target_link_libraries(tests ass_lib inter_lib)
target_link_libraries(tests GTest::GTest GTest::Main)

add_executable(assembler src/assembler.cpp)
add_executable(interpreter src/interpreter.cpp)

target_link_libraries(assembler ass_lib)
target_link_libraries(interpreter inter_lib)
