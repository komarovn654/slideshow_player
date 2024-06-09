cmake_minimum_required(VERSION 3.22.1)
project(ssp 
    VERSION 1.0.0
    DESCRIPTION "OpenGL slideshow player"
    LANGUAGES C CXX)

option(SSP_BUILD_TESTS "Build the test programs" ON)
option(SSP_BUILD_EXAMPLES "Build the example programs" ON)
option(SSP_BUILD_WAYLAND "Build an app for the Wayland" OFF)

if (SSP_BUILD_WAYLAND)
    set(GLFW_BUILD_X11 OFF)
else()
    set(GLFW_BUILD_X11 ON)
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/logman)
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
