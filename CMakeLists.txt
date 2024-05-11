cmake_minimum_required(VERSION 3.22.1)
project(ssp 
    VERSION 1.0.0
    DESCRIPTION "OpenGL slideshow player"
    LANGUAGES C CXX)

# add_subdirectory(third_party/glfw)
add_subdirectory(third_party/googletest)
add_subdirectory(third_party/logman)
add_subdirectory(tests)

# add_subdirectory(src)