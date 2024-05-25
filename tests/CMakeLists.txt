# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GoogleTest)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
option(INSTALL_GTEST "Enable installation of googletest." OFF)

enable_testing()
add_compile_definitions(UTEST_BUILD=1)

include_directories(
    ${CMAKE_SOURCE_DIR}/third_party/googletest/googletest/include/
    ${CMAKE_SOURCE_DIR}/third_party/glfw/include/
    ${CMAKE_SOURCE_DIR}/third_party/logman/include/
    ${CMAKE_SOURCE_DIR}/third_party/glad/opengl33/include/
    ${CMAKE_SOURCE_DIR}/third_party/stb/include/
    ${CMAKE_SOURCE_DIR}/src/graphics/
    ${CMAKE_SOURCE_DIR}/src/observer/
    ${CMAKE_SOURCE_DIR}/src/list/
    ${CMAKE_SOURCE_DIR}/src/utils/
)
link_directories(
    ${CMAKE_BINARY_DIR}/lib 
    ${CMAKE_BINARY_DIR}/third_party/logman/src
    ${CMAKE_BINARY_DIR}/third_party/glfw/src
)

# List tests
add_executable(
    ssp_list_utest
    ${CMAKE_CURRENT_LIST_DIR}/ssp_list_utest.cpp
    ${CMAKE_SOURCE_DIR}/src/list/ssp_list.c
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_memory.c
)
target_link_libraries(
    ssp_list_utest
    ssp_list
    gtest_main gtest logman
)
add_dependencies(ssp_list_utest ssp_list)

# Utils tests
add_executable(
    ssp_memory_utest
    ${CMAKE_CURRENT_LIST_DIR}/ssp_memory_utest.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_memory.c
)
target_link_libraries(ssp_memory_utest gtest_main gtest logman)

add_executable(
    ssp_helper_utest
    ${CMAKE_CURRENT_LIST_DIR}/ssp_helper_utest.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_helper.c
)
target_link_libraries(ssp_helper_utest gtest_main gtest logman)

# Observer tests
add_executable(
    ssp_observer_utest
    ${CMAKE_CURRENT_LIST_DIR}/ssp_observer_utest.cpp
    ${CMAKE_SOURCE_DIR}/src/observer/ssp_observer.c
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_helper.c
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_memory.c
)
target_link_libraries(
    ssp_observer_utest
    ssp_list
    gtest_main gtest
)
add_dependencies(ssp_observer_utest ssp_list)

add_executable(
    ssp_observer_ps_utest
    ${CMAKE_CURRENT_LIST_DIR}/ssp_observer_ps_utest.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_helper.c
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_memory.c
)
target_link_libraries(
    ssp_observer_ps_utest
    ssp_observer
    gtest_main gtest
)

#Graphics tests
#Image loader test
add_executable(
    ssp_image_loader_utest
    ${CMAKE_CURRENT_LIST_DIR}/ssp_image_loader_utest.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/ssp_image_loader.c
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_helper.c
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_memory.c
)
target_link_libraries(
    ssp_image_loader_utest
    gtest_main gtest logman
)

# Module tests
add_executable(
    ssp_observer_mtest
    ${CMAKE_CURRENT_LIST_DIR}/ssp_observer_mtest.c
)
target_link_libraries(
    ssp_observer_mtest
    ssp_observer
)
add_dependencies(ssp_observer_mtest ssp_observer)

add_executable(
    ssp_list_mtest
    ${CMAKE_CURRENT_LIST_DIR}/ssp_list_mtest.c
)
target_link_libraries(
    ssp_list_mtest
    ssp_list
)
add_dependencies(ssp_list_mtest ssp_list)

# add_executable(
#     ssp_graphics_mtest
#     ${CMAKE_CURRENT_LIST_DIR}/ssp_graphics_mtest.c
#     ${CMAKE_SOURCE_DIR}/third_party/glad/opengl33/src/glad.c
#     ${CMAKE_SOURCE_DIR}/src/utils/ssp_helper.c
#     ${CMAKE_SOURCE_DIR}/src/utils/ssp_memory.c    
# )
# target_link_libraries(
#     ssp_graphics_mtest
#     ssp_graphics
#     ssp_list
#     logman
#     glfw3
# )
# add_dependencies(ssp_graphics_mtest ssp_list ssp_graphics)

gtest_discover_tests(ssp_list_utest)
gtest_discover_tests(ssp_memory_utest)
gtest_discover_tests(ssp_helper_utest)
gtest_discover_tests(ssp_observer_utest)
gtest_discover_tests(ssp_observer_ps_utest)
