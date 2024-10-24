# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD )
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(UTEST_BUILD=1 EXC_PATH="${CMAKE_BINARY_DIR}")
include_directories(
    ${CMAKE_SOURCE_DIR}/third_party/googletest/googletest/include/
    ${CMAKE_SOURCE_DIR}/third_party/googletest/googlemock/include/
    ${CMAKE_SOURCE_DIR}/third_party/glfw/include/
    ${GLAD_PATH}/include/
    ${CMAKE_SOURCE_DIR}/third_party/stb/
    ${CMAKE_SOURCE_DIR}/src/graphics/
    ${CMAKE_SOURCE_DIR}/src/observer/
    ${CMAKE_SOURCE_DIR}/src/list/
    ${CMAKE_SOURCE_DIR}/src/utils/
    ${CMAKE_SOURCE_DIR}/tests/mocks/
)
link_directories(
    ${CMAKE_BINARY_DIR}/lib/
)
set(UTILS_SOURCES
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_helper.c
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_memory.c
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_test_storage.c
)

# List tests
add_executable(ssp_list_utest ${CMAKE_CURRENT_LIST_DIR}/ssp_list_utest.cpp ${UTILS_SOURCES})
target_link_libraries(ssp_list_utest ssp_list gtest_main gtest)
gtest_discover_tests(ssp_list_utest)
# List tests

# Utils tests
add_executable(ssp_memory_utest ${CMAKE_CURRENT_LIST_DIR}/ssp_memory_utest.cpp ${UTILS_SOURCES})
target_link_libraries(ssp_memory_utest gtest_main gtest)
gtest_discover_tests(ssp_memory_utest)

add_executable(ssp_helper_utest ${CMAKE_CURRENT_LIST_DIR}/ssp_helper_utest.cpp ${UTILS_SOURCES})
target_link_libraries(ssp_helper_utest gtest_main gtest)
gtest_discover_tests(ssp_helper_utest)

add_executable(ssp_test_storage_utest ${CMAKE_CURRENT_LIST_DIR}/ssp_test_storage_utest.cpp ${UTILS_SOURCES})
target_link_libraries(ssp_test_storage_utest gtest_main gtest)
gtest_discover_tests(ssp_test_storage_utest)
# Utils tests

# Observer tests
add_executable(
    ssp_observer_utest
    ${CMAKE_CURRENT_LIST_DIR}/ssp_observer_utest.cpp
    ${CMAKE_SOURCE_DIR}/src/observer/ssp_observer.c
    ${UTILS_SOURCES}
)
target_link_libraries(ssp_observer_utest ssp_list gtest_main gtest)
gtest_discover_tests(ssp_observer_utest)

if (APPLE OR UNIX)
    add_executable(ssp_observer_ps_utest ${CMAKE_CURRENT_LIST_DIR}/ssp_observer_ps_utest.cpp ${UTILS_SOURCES})
    target_link_libraries(ssp_observer_ps_utest ssp_observer gtest_main gtest)
    gtest_discover_tests(ssp_observer_ps_utest)
endif()
# Observer tests

# Graphics tests
add_executable(
    ssp_image_loader_utest
    ${CMAKE_CURRENT_LIST_DIR}/ssp_image_loader_utest.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/ssp_image_loader.c
    ${UTILS_SOURCES}
)
target_link_libraries(ssp_image_loader_utest gtest_main gtest)
gtest_discover_tests(ssp_image_loader_utest)

add_executable(
    ssp_render_gl33_utest
    ${CMAKE_CURRENT_LIST_DIR}/ssp_render_gl33_utest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/mocks/ssp_glfw_mock.c
    ${CMAKE_CURRENT_LIST_DIR}/mocks/ssp_gl_mock.c
    ${UTILS_SOURCES}
)
target_link_libraries(ssp_render_gl33_utest ssp_graphics glfw gtest_main gtest)
gtest_discover_tests(ssp_render_gl33_utest)

add_executable(
    ssp_render_utest
    ${CMAKE_CURRENT_LIST_DIR}/ssp_render_utest.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/ssp_render.c
    ${CMAKE_CURRENT_LIST_DIR}/mocks/ssp_render_mock.c
    ${CMAKE_CURRENT_LIST_DIR}/mocks/ssp_glfw_mock.c
    ${CMAKE_CURRENT_LIST_DIR}/mocks/ssp_gl_mock.c
    ${UTILS_SOURCES}
)
target_link_libraries(ssp_render_utest ssp_graphics glfw gtest_main gtest)
gtest_discover_tests(ssp_render_utest)
# Graphics tests
