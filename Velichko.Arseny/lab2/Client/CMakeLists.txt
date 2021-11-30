project(Client)

file(GLOB SOURCE_FILES *.cpp *.h)

foreach(CONN_NAME ${CONNECTIONS})
    add_executable(client_${CONN_NAME} ${SOURCE_FILES} ${CONN_INTERFACE})
    target_link_libraries(client_${CONN_NAME} conn_${CONN_NAME} Common libpthread.so)
endforeach()