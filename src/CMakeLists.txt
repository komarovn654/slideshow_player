add_executable(ssp main.c)

if (APPLE)
    find_library(OGL OpenGL)
    find_library(COCOA Cocoa)
    find_library(IOKIT IOKit)

    set(RENDER_LIB ${OGL} ${COCOA} ${IOKIT})
endif()

target_include_directories(
    ssp PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party/logman/include/
    ${CMAKE_SOURCE_DIR}/third_party/glfw/include/

    ${CMAKE_CURRENT_LIST_DIR}/graphics/
    ${CMAKE_CURRENT_LIST_DIR}/list/
    ${CMAKE_CURRENT_LIST_DIR}/observer/
    ${CMAKE_CURRENT_LIST_DIR}/utils/
)

target_link_directories(
    ssp PRIVATE
    ${CMAKE_BINARY_DIR}/third_party/glfw/src/
)

target_link_libraries(
    ssp PRIVATE
    ssp_observer
    ssp_list
    ssp_graphics
    glfw3
    ${RENDER_LIB}
)