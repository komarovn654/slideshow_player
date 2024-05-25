add_library(
    ssp_graphics STATIC
    ${CMAKE_CURRENT_LIST_DIR}/ssp_image_loader.c
    ${CMAKE_CURRENT_LIST_DIR}/ssp_render_gles20.c
    ${CMAKE_CURRENT_LIST_DIR}/ssp_render.c
    ${CMAKE_CURRENT_LIST_DIR}/ssp_shader.c
    ${CMAKE_CURRENT_LIST_DIR}/ssp_window.c
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_helper.c
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_memory.c
)

# find_library(OGL OpenGL)
# find_library(COCOA Cocoa)
# find_library(IOKIT IOKit)

target_include_directories(
    ssp_graphics PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party/glad/opengl33/include/
    ${CMAKE_SOURCE_DIR}/third_party/glfw/include/
    ${CMAKE_SOURCE_DIR}/third_party/stb/include/
    ${CMAKE_SOURCE_DIR}/third_party/logman/include/
    ${CMAKE_SOURCE_DIR}/src/utils/
)
target_link_directories(
    ssp_graphics PRIVATE
    ${CMAKE_BINARY_DIR}/third_party/glfw/src
    ${CMAKE_BINARY_DIR}/third_party/logman/src
)
link_libraries(
    logman
    glfw3
    ${OGL}
    ${COCOA}
    ${IOKIT}
    # m wayland-client wayland-egl GLESv2 EGL 
)

message(OGL ${OGL})
