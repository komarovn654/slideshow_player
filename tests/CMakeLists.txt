# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GoogleTest)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
option(INSTALL_GTEST "Enable installation of googletest." OFF)

enable_testing()

include_directories(
    ../third_party/googletest/googletest/include/ 
    ../third_party/googletest/logman/include/
    ../src)
link_directories(../build/lib ../third_party/logman/src)
link_libraries(gtest_main gtest logman)

add_executable(ssp_list_test ssp_list_test.cpp ../src/ssp_list.c ../src/ssp_memory.c)
add_executable(ssp_memory_test ssp_memory_test.cpp ../src/ssp_memory.c)

gtest_discover_tests(ssp_list_test)
gtest_discover_tests(ssp_memory_test)
    