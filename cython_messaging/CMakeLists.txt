cmake_minimum_required(VERSION 3.15)
project(messaging)

set(CMAKE_CXX_STANDARD 17)
find_package(PythonExtensions REQUIRED)
find_package(Cython REQUIRED)

add_cython_target(messaging CXX PY3)
add_library(messaging MODULE ${messaging})
target_link_libraries(messaging cpp-module-static)
python_extension_module(messaging)

install(TARGETS messaging LIBRARY DESTINATION messaging)