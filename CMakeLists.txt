cmake_minimum_required(VERSION 3.10.0)
project(INTERACTIVE_VON_MISES VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_INTERACTIVE_VON_MISES_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)
add_subdirectory(extern/glad)
add_subdirectory(extern/glfw)
add_subdirectory(extern/tinyfiledialogs)

add_subdirectory(src)

add_executable(interactive_von_mises main.cpp)

target_link_libraries(interactive_von_mises PRIVATE von_mises_lib)