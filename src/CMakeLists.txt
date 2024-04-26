include_directories(
    ../third_party/googletest/logman/include/
    ../src)

find_library(CORE_SERVICE CoreServices)    
link_directories(../third_party/logman/src)
link_libraries(logman)

add_executable(ssp_observer ssp_observer_fsevent.c ssp_list.c ssp_memory.c ssp_helper.c)

target_link_libraries(ssp_observer ${CORE_SERVICE})
    