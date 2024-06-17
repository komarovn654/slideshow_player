if (APPLE)
    find_library(COCOA_FW Cocoa REQUIRED)
    find_library(IOKIT_FW IOKit REQUIRED)

    set(RENDER_LIB ${COCOA_FW} ${IOKIT_FW})
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/src/observer/
    ${CMAKE_SOURCE_DIR}/src/list/
    ${CMAKE_SOURCE_DIR}/src/utils/
)
link_directories(
    ${CMAKE_BINARY_DIR}/lib/
    ${CMAKE_BINARY_DIR}/third_party/glfw/src/
)

add_executable(ssp_list_example ${CMAKE_CURRENT_LIST_DIR}/ssp_list_example.c)
target_link_libraries(ssp_list_example ssp_list)

if (APPLE OR UNIX)
    add_executable(
        ssp_observer_example 
        ${CMAKE_CURRENT_LIST_DIR}/ssp_observer_example.c
        ${CMAKE_SOURCE_DIR}/src/utils/ssp_test_storage.c
        )
    target_link_libraries(ssp_observer_example ssp_observer)
endif()

add_executable(
    ssp_graphics_example 
    ${CMAKE_CURRENT_LIST_DIR}/ssp_graphics_example.c
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_test_storage.c
)
target_include_directories(
    ssp_graphics_example PRIVATE
    ${CMAKE_SOURCE_DIR}/src/graphics/
    # ${CMAKE_SOURCE_DIR}/third_party/glad/opengles32/include/
    ${CMAKE_SOURCE_DIR}/third_party/glfw/include/
)
target_link_libraries(ssp_graphics_example ssp_graphics glfw3 m ${RENDER_LIB})
    # GLESv2)

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
#     glfw3
#     # EGL
#     GLESv2
#     m
# )
# add_dependencies(ssp_graphics_mtest ssp_list ssp_graphics)