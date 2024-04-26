#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <unistd.h>
#include <logman/logman.h>

#include "ssp_observer.h"
#include "ssp_list.h"

#define SSP_OBS_EVENT_BUFFER_SIZE (256)

#define IN_EVENT_DATA_LEN (sizeof(struct inotify_event))
#define IN_EVENT_NEXT(event, length)            \
  ((length) -= (event)->len,                    \
   (struct inotify_event*)(((char *)(event)) +	\
                           (event)->len))
#define IN_EVENT_OK(event, length)                  \
  ((long)(length) >= (long)IN_EVENT_DATA_LEN &&	    \
   (long)(event)->len >= (long)IN_EVENT_DATA_LEN && \
   (long)(event)->len <= (long)(length))

static struct observer {
    int inotify_instance;
    int watch_descriptor;
    struct pollfd fds[SSP_OBS_DIRS_COUNT];
    int event_mask;
    ssp_list image_list;
} observer;

int ssp_obs_init(const char* dir_path)
{
    if (observer.inotify_instance = inotify_init() < 0) {
        log_error("Couldn't initialize inotify");
        return 1;
    }

    if (ssp_obs_create_dir(dir_path) != 0) {
        return 1;
    }

    observer.event_mask = IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO;
    if (observer.watch_descriptor = inotify_add_watch(observer.inotify_instance, dir_path, observer.event_mask) == -1) {
        log_error("Couldn't add watch to %s", dir_path);
        return 1;
    }

    observer.fds[0].fd = observer.inotify_instance;
    observer.fds[0].events = POLLIN;

    log_info("Watching: %s", dir_path);
    return 0;
}

int ssp_obs_process(void)
{
    if (poll(observer.fds, 1, 0) < 0) {
        log_error("Couldn't poll(): '%s'\n", strerror (errno));
        return 1;
    }

    if (observer.fds[0].revents & POLLIN){
        char buffer[SSP_OBS_EVENT_BUFFER_SIZE];
        size_t length;
        if ((length = read(observer.fds[0].fd, buffer, SSP_OBS_EVENT_BUFFER_SIZE)) > 0) {
            struct inotify_event *event = (struct inotify_event *)buffer;
            while (IN_EVENT_OK (event, length)) {
                switch (event->mask)
                {
                case IN_CREATE:
                    if (observer.image_list == NULL) {
                        observer.image_list = ssp_list_init(event->name);
                        break;
                    }
                    ssp_list_insert(observer.image_list, event->name);
                    break;
                case IN_MOVED_TO:
                case IN_MOVED_FROM:
                case IN_DELETE:
                    ssp_list_remove_node(&observer.image_list, event->name);
                    /* code */
                    break;
                default:
                    log_warning("Something unkonwn(%d) was happened with <%s>:", event->mask, event->name);
                    break;
                }
                event = IN_EVENT_NEXT(event, length);
            }
        }
    }
}

int main(void)
{
    log_init_default();
    ssp_obs_init("/home/nikolay/dev/slideshow_player/test");

    while(true) {
        ssp_obs_process();
    }

    return 1;
}