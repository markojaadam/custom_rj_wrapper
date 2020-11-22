cmake_minimum_required(VERSION 3.15)
project(wrapper)

set(CMAKE_CXX_STANDARD 17)
find_package(PythonExtensions REQUIRED)
find_package(Cython REQUIRED)

add_compile_definitions(RAPIDJSON_NO_SIZETYPEDEFINE=1)
set(RAPIDJSON_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/cpp_wrapper/rapidjson/include")
include_directories(BEFORE "${RAPIDJSON_INCLUDE_DIR}" )
add_definitions(-DRAPIDJSON_HAS_STDSTRING)

set(CPP_WRAPPER_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/cpp_wrapper")
# set(CPP_WRAPPER_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/cpp_wrapper/include")
include_directories(BEFORE
        "${CPP_WRAPPER_INCLUDE_DIR}"
        )
add_subdirectory("${CPP_WRAPPER_INCLUDE_DIR}")

add_library(cpp_wrapper MODULE cpp_wrapper/wrapper.cpp)
python_extension_module(cpp_wrapper)
install(TARGETS cpp_wrapper LIBRARY DESTINATION cpp_wrapper)

add_cython_target(primes CXX)
add_library(primes MODULE ${primes})
python_extension_module(primes)

add_cython_target(messaging CXX)
add_library(messaging MODULE ${messaging})
python_extension_module(messaging)

install(TARGETS primes LIBRARY DESTINATION primes)
install(TARGETS messaging LIBRARY DESTINATION messaging)