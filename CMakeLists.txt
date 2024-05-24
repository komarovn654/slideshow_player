cmake_minimum_required(VERSION 3.22.1)
project(ssp 
    VERSION 1.0.0
    DESCRIPTION "OpenGL slideshow player"
    LANGUAGES C CXX)

add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/googletest)
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/logman)
add_subdirectory(${CMAKE_SOURCE_DIR}/tests)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/observer)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/list)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/graphics)
