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

static struct obs_inotify {
    int inotify_instance;
    int watch_descriptor;
    struct pollfd fds;
    int event_mask;

    observer obs;
} obs_inotify;

char **ssp_obs_dirs(void)
{
    return obs_inotify.obs.dirs;
}

size_t ssp_obs_dirs_count(void)
{
    return obs_inotify.obs.dirs_count;
}

void *ssp_obs_storage(void)
{
    return obs_inotify.obs.storage;
}

int ssp_obs_init(observer obs)
{
    memcpy(&obs_inotify.obs, &obs, sizeof(observer));

    return 0;
}

// int ssp_obs_init(const char** dirs)
// {
//     log_init_default();
//     if ((observer.inotify_instance = inotify_init()) < 0) {
//         char *errorbuf = strerror(errno);
//         log_error("Couldn't initialize inotify: %s", errorbuf);
//         return 1;
//     }

//     if (ssp_dir_create(dir_path) != 0) {
//         log_error("Couldn't create directory for observer: %s");
//         return 1;
//     }

//     memcpy(observer.dir_path, dir_path, strlen(dir_path));

//     observer.event_mask = IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO;
//     if ((observer.watch_descriptor = inotify_add_watch(observer.inotify_instance, dir_path, observer.event_mask)) == -1) {
//         char *errorbuf = strerror(errno);
//         log_error("Couldn't add watch to <%s>: %s", dir_path, errorbuf);
//         return 1;
//     }

//     observer.fds[0].fd = observer.inotify_instance;
//     observer.fds[0].events = POLLIN;

//     if ((observer.images_list = ssp_list_init()) == NULL) {
//         log_error("Couldn't initialize images list for observer");
//         return 1;
//     }

//     if ((ssp_dir_traversal(dir_path, (void (*)(void *, const char *))ssp_list_insert, observer.images_list)) != 0) {
//         return 1;
//     }    

//     log_info("Watching: %s", dir_path);
//     return 0;
// }

// int ssp_obs_process(void)
// {
//     if (poll(observer.fds, 1, 0) < 0) {
//         log_error("Couldn't poll(): '%s'\n", strerror (errno));
//         return 1;
//     }

//     if (observer.fds[0].revents & POLLIN){
//         char buffer[SSP_OBS_EVENT_BUFFER_SIZE];
//         size_t length;
//         if ((length = read(observer.fds[0].fd, buffer, SSP_OBS_EVENT_BUFFER_SIZE)) > 0) {
//             struct inotify_event *event = (struct inotify_event *)buffer;
//             while (IN_EVENT_OK(event, length)) {
//                 char fullname[SSP_FILE_NAME_MAX_LEN];
//                 if (snprintf(fullname, SSP_FILE_NAME_MAX_LEN, "%s%s", observer.dir_path, event->name) >= SSP_FILE_NAME_MAX_LEN) {
// 			        log_warning("<%s> is too long and was truncated", event->name);
// 			        continue;
// 		        };

//                 switch (event->mask)
//                 {
//                 case IN_MOVED_TO:
//                 case IN_CREATE:
//                     if (ssp_is_file_image(fullname) == false) {
//                         break;
//                     }
//                     ssp_list_insert(observer.images_list, fullname);
//                     break;
//                 case IN_MOVED_FROM:
//                 case IN_DELETE:              
//                     ssp_list_remove_node(&observer.images_list, fullname);
//                     break;
//                 default:
//                     log_warning("Something unkonwn(%d) was happened with <%s>:", event->mask, event->name);
//                     break;
//                 }
//                 event = IN_EVENT_NEXT(event, length);
//             }
//         }
//     }
// }

// int ssp_obs_images_list(char **images)
// {
//     if (images == NULL) {
//         log_error("Unable to save images to null storage");
//         return 1;
//     }

//     if (ssp_list_traversal(observer.images_list, images, SSP_FILE_NAME_MAX_LEN) != 0) {
//         log_error("Unable to traversal the observer's file list");
//         return 1;
//     }

//     return 0;
// }

// ssp_list ssp_obs_images(void)
// {
//     return observer.images_list;
// }

// int main(void)
// {
//     log_init_default();

//     ssp_obs_init("../../tests/images/");

//     while(1) {
//         ssp_obs_process();
//     }
// }