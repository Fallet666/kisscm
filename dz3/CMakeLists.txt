cmake_minimum_required(VERSION 3.28)
project(config_lang)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE( Boost COMPONENTS json REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(config_lang main.cpp)

TARGET_LINK_LIBRARIES(config_lang LINK_PUBLIC ${Boost_LIBRARIES} )