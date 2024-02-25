cmake_minimum_required(VERSION 3.22.1)

project(slideshow_player VERSION 0.1)

set(PROTO_TARGETS 
    wayland_client/xdg-shell-protocol.c
    wayland_client/xdg-shell-client-protocol.h
)

set(SOURCES 
    main.c
    image_reader/image.c
    wayland_client/window.c
    wayland_client/xdg_wm_base.c
    wayland_client/shm.c
)

set(INCLUDES 
    /usr/include/pixman-1
    image_reader
    wayland_client
)

add_custom_command(
    OUTPUT ${PROTO_TARGETS}
    COMMAND ${CMAKE_SOURCE_DIR}/proto_gen.sh
)

add_executable(${PROJECT_NAME} ${SOURCES} ${PROTO_TARGETS})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})
target_link_libraries(${PROJECT_NAME} wayland-client pixman-1 jpeg pthread)
