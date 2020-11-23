cmake_minimum_required(VERSION 3.15)
project(custom_rj_wrapper)

set(CMAKE_CXX_STANDARD 17)
find_package(PythonExtensions REQUIRED)
find_package(Cython REQUIRED)

add_compile_definitions(RAPIDJSON_NO_SIZETYPEDEFINE=1)
set(RAPIDJSON_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/cpp_messaging/rapidjson/include")
message("${RAPIDJSON_INCLUDE_DIR}")
include_directories(BEFORE "${RAPIDJSON_INCLUDE_DIR}")
add_definitions(-DRAPIDJSON_HAS_STDSTRING)
set(CPP_MODULE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/cpp_messaging")
set(CYTHON_MODULE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/cython_messaging")

include_directories(BEFORE
        "${RAPIDJSON_INCLUDE_DIR}"
        )

include_directories(BEFORE
        "${CPP_MODULE_INCLUDE_DIR}"
        )

include_directories(BEFORE
        "${CYTHON_MODULE_INCLUDE_DIR}"
        )

add_subdirectory("${CPP_MODULE_INCLUDE_DIR}")
add_subdirectory("${CYTHON_MODULE_INCLUDE_DIR}")

target_link_libraries(messaging cpp-module-static)