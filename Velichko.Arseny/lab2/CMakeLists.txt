cmake_minimum_required(VERSION 3.0)
project(IPC)

set(CMAKE_CXX_STANDARD 17)

file(GLOB HOST_SRC_FILES Host/*.cpp Host/*.h)
file(GLOB CLIENT_SRC_FILES Client/*.cpp Client/*.h)

add_compile_definitions(CONN_IMPL_HEADER="conn_sock.h" SOCKET_NAME="socket")

add_executable(host_exe ${HOST_SRC_FILES} Connections/Connection.h Connections/Connection.cpp
        Connections/conn_sock.h Connections/conn_sock.cpp)
target_link_libraries(host_exe librt.so libpthread.so)

add_executable(client_exe ${CLIENT_SRC_FILES} Connections/Connection.h Connections/Connection.cpp
        Connections/conn_sock.h Connections/conn_sock.cpp)