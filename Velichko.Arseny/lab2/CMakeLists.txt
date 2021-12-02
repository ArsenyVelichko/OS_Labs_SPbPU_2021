cmake_minimum_required(VERSION 3.10)
project(IPC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_compile_definitions(PROJECT_DIR="${CMAKE_SOURCE_DIR}")

list(APPEND CONNECTIONS "sock" "fifo" "seg")

include_directories(Common)
file(GLOB CONN_INTERFACE Connections/Connection.h)

add_subdirectory(Common)
add_subdirectory(Connections)
add_subdirectory(Host)
add_subdirectory(Client)