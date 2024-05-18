include_directories(
    /opt/devil/include/
    /opt/homebrew/include/
    ../third_party/glfw/include/
    ../third_party/logman/include/
    ../src/
    )

link_directories(
    /opt/devil/slib/
    /opt/homebrew/lib/
    ${CMAKE_BINARY_DIR}/third_party/glfw/src
    ${CMAKE_BINARY_DIR}/third_party/logman/src
    )
link_libraries(
    logman
    glfw3
    GLEW
    IL
    # m wayland-client wayland-egl GLESv2 EGL 
)

find_library(OGL OpenGL)
find_library(COCOA Cocoa)
find_library(IOKIT IOKit)

message(OGL ${OpenGL})

add_executable(player 
    main_window.c 
    ssp_window.c 
    ssp_observer.c
    ssp_observer_fsevent.c
    ssp_image_loader.c
    ssp_shader.c
    ssp_render.c
    ssp_list.c 
    ssp_memory.c 
    ssp_helper.c
)

target_link_libraries(player ${OGL} ${COCOA} ${IOKIT})