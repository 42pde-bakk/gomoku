cmake_minimum_required(VERSION 3.17)
project(Gomokubot CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB includes "include/*.hpp")
file(GLOB srcs "srcs/*.cpp")
file(GLOB IO "srcs/IO/*.cpp" "include/IO/*.hpp")
file(GLOB Async "srcs/Async/*.cpp" "include/Async/*.hpp")
file(GLOB Gamerules "srcs/Gamerules/*.cpp" "include/Gamerules/*.hpp")

add_executable(Gomokubot ${includes} ${srcs} ${IO} ${Gamerules} ${Async})

target_include_directories(Gomokubot PRIVATE include)
target_include_directories(Gomokubot PRIVATE include/Gamerules)
target_include_directories(Gomokubot PRIVATE include/IO)
target_include_directories(Gomokubot PRIVATE include/Async)

add_subdirectory(tests)

