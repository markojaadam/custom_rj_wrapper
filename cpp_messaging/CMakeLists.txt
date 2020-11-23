cmake_minimum_required(VERSION 3.15)
project(cpp_wrapper)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(RAPIDJSON_NO_SIZETYPEDEFINE=1)
set(RAPIDJSON_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include")
message("${RAPIDJSON_INCLUDE_DIR}")
include_directories(BEFORE "${RAPIDJSON_INCLUDE_DIR}")
add_definitions(-DRAPIDJSON_HAS_STDSTRING)
set(rapidjson_src rapidjson.hpp)

set (SRC messaging.cpp messaging.hpp)
add_library(cpp-module-static STATIC ${SRC})

add_executable(bench_parsing bench_parsing.cpp)
add_executable(bench_construct bench_construction.cpp)
add_executable(test_parsing test_parsing.cpp)
add_executable(test_construct test_construction.cpp)
