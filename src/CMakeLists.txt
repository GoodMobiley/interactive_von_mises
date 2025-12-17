add_library(von_mises_lib
    von_mises.cpp

    geometry.cpp
    user_interface.cpp
    shader.cpp
    pallet.cpp

    cylinder.cpp
    render_cylinder.cpp

    airfoil.cpp
    render_airfoil.cpp

    velocities.cpp
    render_velocities.cpp
)

target_include_directories(von_mises_lib PUBLIC
    ${INTERACTIVE_VON_MISES_SOURCE_DIR}/include
    ${INTERACTIVE_VON_MISES_SOURCE_DIR}/extern/linmath
    ${INTERACTIVE_VON_MISES_SOURCE_DIR}/extern/tinyfiledialogs
    ${INTERACTIVE_VON_MISES_SOURCE_DIR}/extern/glad/include
    ${INTERACTIVE_VON_MISES_SOURCE_DIR}/extern/glfw/include
)

target_link_libraries(von_mises_lib PUBLIC OpenGL::GL)
target_link_libraries(von_mises_lib PUBLIC glad)
target_link_libraries(von_mises_lib PUBLIC glfw)
target_link_libraries(von_mises_lib PUBLIC tiny_file_dialogs)