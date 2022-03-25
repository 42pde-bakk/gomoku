project(Catch_tests_run CXX)
cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 20)


# Testing source code
file(GLOB test_srcs "*.cpp")

# Source code
file(GLOB includes "../include/*.hpp")
file(GLOB srcs "../srcs/*.cpp")
list(FILTER srcs EXCLUDE REGEX "../srcs/main.cpp")
file(GLOB IO "../srcs/IO/*.cpp" "../include/IO/*.hpp")
file(GLOB Gamerules "../srcs/Gamerules/*.cpp" "../include/Gamerules/*.hpp")

add_executable(Catch_tests_run ${test_srcs} ${includes} ${srcs} ${Gamerules} ${IO})
target_include_directories(Catch_tests_run PRIVATE ../include ../include/Gamerules )
