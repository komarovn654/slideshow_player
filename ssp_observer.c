#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <logger.h>

#define IN_EVENT_DATA_LEN (sizeof(struct inotify_event))
#define IN_EVENT_NEXT(event, length)            \
  ((length) -= (event)->len,                    \
   (struct inotify_event*)(((char *)(event)) +	\
                           (event)->len))
#define IN_EVENT_OK(event, length)                  \
  ((long)(length) >= (long)IN_EVENT_DATA_LEN &&	    \
   (long)(event)->len >= (long)IN_EVENT_DATA_LEN && \
   (long)(event)->len <= (long)(length))

static int event_mask = (
    // IN_ACCESS |         /* File accessed */
    // IN_ATTRIB |         /* File attributes changed */
    // IN_OPEN   |         /* File was opened */
    // IN_CLOSE_WRITE |    /* Writtable File closed */
    // IN_CLOSE_NOWRITE |  /* Unwrittable File closed */
    // IN_CREATE |         /* File created in directory */
    // IN_DELETE |         /* File deleted in directory */
    // IN_DELETE_SELF |    /* Directory deleted */
    // IN_MODIFY         /* File modified */
    // IN_MOVE_SELF |      /* Directory moved */
    IN_MOVED_FROM |     /* File moved away from the directory */
    IN_MOVED_TO         /* File moved into the directory */
);

int inotify_instance, wd;
struct pollfd fds[1];

static int ssp_image_list_update()
{

}

static int ssp_observer_init(const char* dir_path)
{
    inotify_instance = inotify_init();
    if (inotify_instance < 0) {
        log_error("couldn't initialize inotify");
        return 1;
    }

    wd = inotify_add_watch(inotify_instance, dir_path, event_mask);
    if (wd == -1) {
        log_error("couldn't add watch to %s\n", dir_path);
        return 1;
    }

    fds[0].fd = inotify_instance;
    fds[0].events = POLLIN;

    log_info("watching: %s", dir_path);
    return 0;
}

void ssp_observe_image_dir(void)
{
    if (poll (fds, 1, 0) < 0) {
        fprintf (stderr, "Couldn't poll(): '%s'\n", strerror (errno));
        exit (EXIT_FAILURE);
    }

    if (fds[0].revents & POLLIN){
        char buffer[8192];
        size_t length;
        /* Read from the FD. It will read all events available up to
        * the given buffer size. */
        if ((length = read (fds[0].fd, buffer, 8192)) > 0) {
            struct inotify_event *event;
   
            event = (struct inotify_event *)buffer;
            while (IN_EVENT_OK (event, length)) {
                printf("smth happened\n");
                event = IN_EVENT_NEXT (event, length);
            }
        }
    }
}
