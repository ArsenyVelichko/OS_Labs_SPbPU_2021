cmake_minimum_required(VERSION 3.0)
project(IPC)

add_definitions(-DSOCKET_NAME="socket")

file(GLOB SOURCE_FILES *.cpp *.h)

add_executable(host ${SOURCE_FILES})
target_link_libraries(host librt.so libpthread.so)
add_subdirectory(client)