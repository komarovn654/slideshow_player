add_library(
    ssp_list STATIC
    ${CMAKE_CURRENT_LIST_DIR}/ssp_list.c
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_helper.c
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_memory.c
)

target_include_directories(
    ssp_list PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party/logman/include/
    ${CMAKE_SOURCE_DIR}/src/utils/
)
target_link_directories(
    ssp_observer PRIVATE
    ${CMAKE_BINARY_DIR}/third_party/logman/src
)
target_link_libraries(
    ssp_list
    logman
)
