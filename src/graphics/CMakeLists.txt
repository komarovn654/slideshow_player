add_library(
    ssp_graphics STATIC
    ${RENDER_SRC}
    ${GLAD_PATH}/src/glad.c
    ${CMAKE_CURRENT_LIST_DIR}/ssp_glfw.c
    ${CMAKE_CURRENT_LIST_DIR}/ssp_gl.c
    ${CMAKE_CURRENT_LIST_DIR}/ssp_image_loader.c
    ${CMAKE_CURRENT_LIST_DIR}/ssp_render.c
    ${CMAKE_CURRENT_LIST_DIR}/ssp_shader.c
    ${CMAKE_CURRENT_LIST_DIR}/ssp_window.c
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_helper.c
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_memory.c
)

target_include_directories(
    ssp_graphics PRIVATE
    ${GLAD_PATH}/include/
    ${CMAKE_SOURCE_DIR}/third_party/glfw/include/
    ${CMAKE_SOURCE_DIR}/third_party/stb/
    ${CMAKE_SOURCE_DIR}/src/utils/
)

# target_link_directories(
#     ssp_graphics PRIVATE
#     ${CMAKE_BINARY_DIR}/third_party/glfw/src
# )

# link_libraries(
#     glfw3
#     m
#     # ${RENDER_LIB}
#     # GLESv2
#     # EGL
#     # ${OGL}
#     # ${COCOA}
#     # ${IOKIT}
#     # m wayland-client wayland-egl GLESv2 EGL 
# )
