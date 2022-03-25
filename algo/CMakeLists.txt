cmake_minimum_required(VERSION 3.17)
project(Gomokubot CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB includes "include/*.hpp")
file(GLOB srcs "srcs/*.cpp")
file(GLOB IO "srcs/IO/*.cpp" "include/IO/*hpp")
file(GLOB Gamerules "srcs/Gamerules/*.cpp" "include/Gamerules/*.hpp")

add_executable(Cluster ${includes} ${srcs} ${IO} ${Gamerules})

target_include_directories(Cluster PRIVATE include)
target_include_directories(Cluster PRIVATE include/Gamerules)
target_include_directories(Cluster PRIVATE include/IO)

#add_subdirectory(tests)

