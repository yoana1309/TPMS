cmake_minimum_required(VERSION 3.18.0)

set(This Var)

project(${This} C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

enable_testing()

add_compile_options(-fprofile-abs-path)

add_subdirectory(googletest)
add_subdirectory(src)
add_subdirectory(test)

