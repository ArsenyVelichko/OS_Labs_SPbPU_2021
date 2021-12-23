cmake_minimum_required(VERSION 3.10)
project(Concurrent_structures)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(PROJECT_DIR="${CMAKE_SOURCE_DIR}")

file(GLOB SOURCE_FILES *.cpp *.h)

add_executable(run_tests ${SOURCE_FILES})
target_link_libraries(run_tests PRIVATE Threads::Threads)