add_library(glad gl.c)
target_include_directories(glad PUBLIC ${GLAD_SOURCE_DIR}/include)