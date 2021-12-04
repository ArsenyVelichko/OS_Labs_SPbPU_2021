project(Connections)

add_library(ConnInterface INTERFACE)
target_include_directories(ConnInterface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

foreach(CONN_NAME ${CONNECTIONS})
    add_library(conn_${CONN_NAME} Core/conn_${CONN_NAME}.h Core/conn_${CONN_NAME}.cpp
                Connection.h Connection.cpp IODevice.h)
    target_compile_definitions(conn_${CONN_NAME} PRIVATE CONN_IMPL_HEADER="Core/conn_${CONN_NAME}.h")
    target_link_libraries(conn_${CONN_NAME} ConnInterface Utility)
endforeach()