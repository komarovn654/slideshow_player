add_library(
    ssp_observer STATIC
    ${CMAKE_CURRENT_LIST_DIR}/ssp_observer.c
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_helper.c
    ${CMAKE_SOURCE_DIR}/src/utils/ssp_memory.c
)

target_include_directories(
    ssp_observer PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party/logman/include/
    ${CMAKE_SOURCE_DIR}/src/utils/
)
target_link_directories(
    ssp_observer PRIVATE
    ${CMAKE_BINARY_DIR}/third_party/logman/src
)
target_link_libraries(
    ssp_observer
    logman
)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_sources(
        ssp_observer PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/ssp_observer_inotify.c
    )
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    find_library(CORE_SERVICE CoreServices)
    target_sources(
        ssp_observer PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/ssp_observer_fsevent.c
    )
    target_link_libraries(ssp_observer ${CORE_SERVICE})
endif()
