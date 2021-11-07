cmake_minimum_required(VERSION 3.0)
project(IPC)

add_definitions(-DSOCKET_NAME="socket")

add_executable(client client.cpp)
add_executable(host host.cpp)