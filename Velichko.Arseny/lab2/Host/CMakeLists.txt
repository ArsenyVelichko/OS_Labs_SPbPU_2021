project(Host)

file(GLOB SOURCE_FILES *.cpp *.h)

foreach(CONN_NAME ${CONNECTIONS})
    add_executable(host_${CONN_NAME} ${SOURCE_FILES} ${CONN_INTERFACE})
    target_link_libraries(host_${CONN_NAME} conn_${CONN_NAME} Common librt.so libpthread.so)
endforeach()