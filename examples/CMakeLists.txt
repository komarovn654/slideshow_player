include_directories(
    ${CMAKE_SOURCE_DIR}/third_party/logman/include/
    ${CMAKE_SOURCE_DIR}/src/observer/
    ${CMAKE_SOURCE_DIR}/src/list/
    ${CMAKE_SOURCE_DIR}/src/utils/
)
link_directories(
    ${CMAKE_BINARY_DIR}/lib/
    ${CMAKE_BINARY_DIR}/third_party/logman/src/
    ${CMAKE_BINARY_DIR}/third_party/glfw/src/
)

add_executable(ssp_list_example ${CMAKE_CURRENT_LIST_DIR}/ssp_list_example.c)
target_link_libraries(ssp_list_example ssp_list)

add_executable(ssp_observer_example ${CMAKE_CURRENT_LIST_DIR}/ssp_observer_example.c)
target_link_libraries(ssp_observer_example ssp_observer logman)

add_executable(ssp_graphics_example ${CMAKE_CURRENT_LIST_DIR}/ssp_graphics_example.c)
target_include_directories(
    ssp_graphics_example PRIVATE
    ${CMAKE_SOURCE_DIR}/src/graphics/
    # ${CMAKE_SOURCE_DIR}/third_party/glad/opengles32/include/
    ${CMAKE_SOURCE_DIR}/third_party/glfw/include/
)
target_link_libraries(ssp_graphics_example ssp_graphics logman glfw3 GLESv2 m)

# add_executable(
#     ssp_graphics_mtest
#     ${CMAKE_CURRENT_LIST_DIR}/ssp_graphics_mtest.c
#     ${CMAKE_SOURCE_DIR}/src/utils/ssp_helper.c
#     ${CMAKE_SOURCE_DIR}/src/utils/ssp_memory.c
#     ${CMAKE_SOURCE_DIR}/third_party/glad/opengles32/src/glad.c
# )
# target_link_libraries(
#     ssp_graphics_mtest
#     ssp_graphics
#     ssp_list
#     logman
#     glfw3
#     # EGL
#     GLESv2
#     m
# )
# add_dependencies(ssp_graphics_mtest ssp_list ssp_graphics)