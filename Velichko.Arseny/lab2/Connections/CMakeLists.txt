project(Connections)

foreach(CONN_NAME ${CONNECTIONS})
    add_library(conn_${CONN_NAME} conn_${CONN_NAME}.h conn_${CONN_NAME}.cpp
            Connection.h Connection.cpp IODevice.h)
    target_compile_definitions(conn_${CONN_NAME} PRIVATE CONN_IMPL_HEADER="conn_${CONN_NAME}.h")
endforeach()