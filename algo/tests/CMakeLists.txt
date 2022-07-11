project(Catch_tests_run CXX)
cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 20)


# Testing source code
file(GLOB test_srcs "srcs/*.cpp")
list(FILTER test_srcs EXCLUDE REGEX "MinimaxTest.cpp")
file(GLOB MinimaxTests "srcs/MinimaxTest.cpp")

# Source code
file(GLOB includes "../include/*.hpp")
file(GLOB srcs "../srcs/*.cpp")
list(FILTER srcs EXCLUDE REGEX "../srcs/main.cpp")
file(GLOB IO "../srcs/IO/*.cpp" "../include/IO/*.hpp")
file(GLOB Async "../srcs/Async/*.cpp" "../include/Async/*.hpp")
file(GLOB Gamerules "../srcs/Gamerules/*.cpp" "../include/Gamerules/*.hpp")

add_executable(Gomokutests.out ${test_srcs} ${includes} ${srcs} ${Gamerules} ${IO} ${Async})
add_executable(Minimaxtests.out ${MinimaxTests} ${includes} ${srcs} ${Gamerules} ${IO} ${Async})
target_compile_options(Minimaxtests.out PRIVATE "-D THREADED=0 -D TIMEOUT_VALUE=2000")
target_compile_options(Gomokutests.out PRIVATE "-D TIMEOUT_VALUE=2000")
#target_compile_definitions(Minimaxtests.out PUBLIC -DTHREAD=0)

target_include_directories(Gomokutests.out PRIVATE ../include ../include/Gamerules ../include/Async)
target_include_directories(Minimaxtests.out PRIVATE ../include ../include/Gamerules ../include/Async)

#set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra")
