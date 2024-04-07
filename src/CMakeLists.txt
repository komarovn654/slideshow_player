include_directories(
    ../third_party/googletest/logman/include/
    ../src)
link_directories(../third_party/logman/src)
link_libraries(logman)

add_executable(ssp_observer ssp_observer_inotify.c ssp_list.c ssp_memory.c ssp_helper.c)
    