include_directories(
    /opt/glfw/include/
    /opt/devil/include/
    ../third_party/logman/include/
    ../src/)

link_directories(../third_party/logman/src /opt/glfw/lib/ /opt/devil/lib)
link_libraries(logman glfw3 m wayland-client wayland-egl GLESv2 EGL IL)

add_executable(player 
    main.c 
    ssp_window.c 
    ssp_observer_inotify.c 
    ssp_image_loader.c
    ssp_shader.c
    ssp_render.c
    ssp_list.c 
    ssp_memory.c 
    ssp_helper.c
)

# target_link_libraries(ssp_observer ${CORE_SERVICE})
    