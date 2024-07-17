cmake_minimum_required(VERSION 3.22.1)
project(ssp 
    VERSION 1.0.0
    DESCRIPTION "OpenGL slideshow player"
    LANGUAGES C CXX)

option(SSP_BUILD_TESTS "Build the test programs" ON)
option(SSP_BUILD_EXAMPLES "Build the example programs" ON)
# todo: delete it
option(SSP_BUILD_WAYLAND "Build with Wayland support" OFF)

set(SSP_PLATFORM_LISTS 
    SSP_DP_WIN32
    SSP_DP_COCOA
    SSP_DP_X11
    SSP_DP_WAYLAND
)

list(FIND SSP_PLATFORM_LISTS ${SSP_PLATFORM} index)
if(index GREATER -1)
    list(GET SSP_PLATFORM_LISTS ${index} ssp_platform_value)
    add_compile_definitions(_DISPLAY_PLATFORM=${ssp_platform_value})
else()
    message(FATAL_ERROR "Unsupported SSP_PLATFORM: ${SSP_PLATFORM}. Supported platforms are: ${SSP_PLATFORM_LISTS}")
endif()

if (SSP_BUILD_WAYLAND)
    set(RENDER_SRC ${CMAKE_SOURCE_DIR}/src/graphics/ssp_render_gles20.c)
    set(GLAD_PATH ${CMAKE_SOURCE_DIR}/third_party/glad/opengles32/)
else()
    set(RENDER_SRC ${CMAKE_SOURCE_DIR}/src/graphics/ssp_render_gl33.c)
    set(GLAD_PATH ${CMAKE_SOURCE_DIR}/third_party/glad/opengl33/)
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/observer)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/list)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/graphics)
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

if (SSP_BUILD_TESTS)
    include(GoogleTest)

    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(BUILD_GTEST ON CACHE BOOL "" FORCE)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    option(INSTALL_GTEST "Enable installation of googletest." OFF)

    enable_testing()
    add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/googletest)
    add_subdirectory(${CMAKE_SOURCE_DIR}/tests)
endif()

if (SSP_BUILD_EXAMPLES)
    add_subdirectory(${CMAKE_SOURCE_DIR}/examples)
endif()
