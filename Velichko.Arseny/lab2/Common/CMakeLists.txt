project(Common)

file(GLOB SOURCE_FILES *.cpp *.h)
add_library(Common ${SOURCE_FILES} ${CONN_INTERFACE})