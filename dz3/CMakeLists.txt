cmake_minimum_required(VERSION 3.28)
project(config_lang)

set(CMAKE_CXX_STANDARD 17)

enable_testing()
include_directories(translator)
add_library(trans_lib
        translator/Translator.cpp
)

FIND_PACKAGE(Boost COMPONENTS json REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tests tests/tests.cpp)

target_link_libraries(tests trans_lib)
target_link_libraries(tests GTest::GTest GTest::Main)

add_executable(config_lang src/main.cpp)
target_link_libraries(config_lang trans_lib)
TARGET_LINK_LIBRARIES(trans_lib LINK_PUBLIC ${Boost_LIBRARIES})
