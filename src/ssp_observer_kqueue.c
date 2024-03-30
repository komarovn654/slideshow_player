#include <dirent.h> 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>
#include <logman/logman.h>
#include "ssp_list.h"
#include "ssp_memory.h"
 
#define NUM_EVENT_SLOTS 1
#define NUM_EVENT_FDS 1
 
extern bool ssp_is_file_image(const char *file_name);
char *flagstring(int flags);
 
/* A simple routine to return a string for a set of flags. */
char *flagstring(int flags)
{
    static char ret[512];
    char *or = "";
 
    ret[0]='\0'; // clear the string.
    if (flags & NOTE_DELETE) {strcat(ret,or);strcat(ret,"NOTE_DELETE");or="|";}
    if (flags & NOTE_WRITE) {strcat(ret,or);strcat(ret,"NOTE_WRITE");or="|";}
    if (flags & NOTE_EXTEND) {strcat(ret,or);strcat(ret,"NOTE_EXTEND");or="|";}
    if (flags & NOTE_ATTRIB) {strcat(ret,or);strcat(ret,"NOTE_ATTRIB");or="|";}
    if (flags & NOTE_LINK) {strcat(ret,or);strcat(ret,"NOTE_LINK");or="|";}
    if (flags & NOTE_RENAME) {strcat(ret,or);strcat(ret,"NOTE_RENAME");or="|";}
    if (flags & NOTE_REVOKE) {strcat(ret,or);strcat(ret,"NOTE_REVOKE");or="|";}
 
    return ret;
}

static struct observer {
    int kq; // kernel queue
    int event_fd;
    struct kevent event_to_monitor;
    struct kevent event_data;
    const char *dir_path;
    void *user_data;
    struct timespec timeout;
    unsigned int vnode_events;
    ssp_list images;
    
    int prev_count;
    int actual_count;
} observer;

static int ssp_obs_open_dir(const char *dir_path)
{
    struct stat st = {0};
    if (stat(dir_path, &st) == -1) {
        mkdir(dir_path, 0700);
        log_info("The directory <%s> has been created.", dir_path);
    }
    observer.event_fd = open(dir_path, O_EVTONLY);
    log_info("The directory <%s> has been opened; fd: %d.", dir_path, observer.event_fd);
    
    if (observer.event_fd <=0) {
        log_error("The directory <%s> could not be opened for monitoring.  Error was %s.", dir_path, strerror(errno));
        return 1;
    }

    return 0;
}

static void print(const char* fn)
{
    printf("FILE: %s\n", fn);
}

static void ssp_obs_update_image_list()
{
    DIR *d;
    struct dirent *dir;
    struct stat st = {0};
    d = opendir(observer.dir_path);
    while ((dir = readdir(d)) != NULL) {
        
        // char *name[100];
        // sprintf((char *)name, "%s%s", "/Users/nikolajkomarov/dev/slideshow_player/test/", dir->d_name);

        // if (!ssp_is_file_image(name)) {
        //     continue;
        // }

        // if (observer.images == NULL) {
        //     observer.images = ssp_list_init(dir->d_name);
        //     continue;
        // }
        
        // ssp_list_insert(observer.images, dir->d_name);
    }

    ssp_list_print(observer.images, print);
    printf("\n");
    closedir(d);
}

int ssp_obs_init(const char *dir_path)
{
    if ((observer.kq = kqueue()) < 0) {
        log_error("Could not open kernel queue.  Error was %s.", strerror(errno));
        return 1;
    }
 
    if (ssp_obs_open_dir(dir_path)) {
        return 1;
    }

    observer.dir_path = dir_path;
    observer.user_data = (void *)dir_path;
 
    observer.timeout.tv_sec = 0;        // 0 seconds
    observer.timeout.tv_nsec = 500000000;    // 500 milliseconds
 
    observer.vnode_events = NOTE_DELETE |  NOTE_WRITE | NOTE_EXTEND | NOTE_ATTRIB | NOTE_LINK |
        NOTE_RENAME | NOTE_REVOKE;
    EV_SET(&observer.event_to_monitor, observer.event_fd, EVFILT_VNODE, EV_ADD | EV_CLEAR, 
        observer.vnode_events, 0, observer.user_data);

    ssp_obs_update_image_list();

    return 0;
}

int ssp_obs_deinit(void)
{
    return close(observer.event_fd);
}

void ssp_obs_process(void)
{
    int event_count = kevent(observer.kq, &observer.event_to_monitor, NUM_EVENT_SLOTS, &observer.event_data, 10, &observer.timeout);
    if ((event_count < 0) || (observer.event_data.flags == EV_ERROR)) {
        /* An error occurred. */
        log_error("An error occurred (event count %d).  The error was %s.\n", event_count, strerror(errno));
        return;
    }
    if (event_count) {
        printf("Event %" PRIdPTR " occurred.  Filter %d, flags %d, filter flags %s, filter data %" PRIdPTR ", path %s\n",
            observer.event_data.ident,
            observer.event_data.filter,
            observer.event_data.flags,
            flagstring(observer.event_data.fflags),
            observer.event_data.data,
            (char *)observer.event_data.udata);
        if (observer.event_data.flags == 33) {
            ssp_obs_update_image_list();
        }
    } else {
        // printf("No event.\n");
    }
}

int main(void)
{
    const char *dir = "../../test";
    log_init_default();

    ssp_obs_init(dir);

    while(1) {
        ssp_obs_process();
    }
}