cmake_minimum_required(VERSION 3.17)
project(Gomokubot CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB includes "include/*.hpp")
file(GLOB srcs "srcs/*.cpp")

add_executable(Cluster ${includes} ${srcs})
include_directories(include)
