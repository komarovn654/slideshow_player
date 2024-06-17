#include <errno.h>
#include <poll.h>
#include <string.h>
#include <sys/inotify.h>
#include <syslog.h>
#include <unistd.h>

#include "ssp_observer.h"
#include "ssp_observer_ps.h"

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
    int watch_descriptor[SSP_OBS_DIRS_MAX_COUNT];
    struct pollfd fd;
    int event_mask;

    ssp_observer* obs;
} obs_inotify;

ssp_static int ssp_obsps_add_directories(void)
{
    for (size_t i = 0; i < obs_inotify.obs->dirs_count; i++) {
        obs_inotify.watch_descriptor[i] = inotify_add_watch(
            obs_inotify.inotify_instance, obs_inotify.obs->dirs[i], obs_inotify.event_mask);
        if (obs_inotify.watch_descriptor[i] <= 0) {
            syslog(LOG_ERR, "SSP IObserver. Couldn't add to observer inotify <%s>: %s", obs_inotify.obs->dirs[i], 
                strerror(errno));
            return 1;
        }
    }

    return 0;
}

ssp_static int ssp_obsps_find_wd_index(int wd)
{
    for (size_t i = 0; i < SSP_OBS_DIRS_MAX_COUNT; i++) {
        if (obs_inotify.watch_descriptor[i] == wd) {
            return i;
        }
    }
    return -1;
}

int ssp_obsps_init(ssp_observer settings)
{
    if ((obs_inotify.obs = ssp_obs_init(settings)) == NULL) {
        syslog(LOG_ERR, "SSP IObserver. Common observer initialization error");
        return 1;
    }

    if ((obs_inotify.inotify_instance = inotify_init()) < 0) {
        syslog(LOG_ERR, "SSP IObserver. Inotify initialization error: %s", strerror(errno));
        return 1;
    }

    if ((ssp_obs_dirs_create(obs_inotify.obs)) != 0) {
        syslog(LOG_ERR, "SSP IObserver. Observer directories creation error");
        return 1;
    }

    obs_inotify.event_mask = IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO;

    if (ssp_obsps_add_directories() != 0) {
        syslog(LOG_ERR, "SSP IObserver. Failed to sign catalogs on inotify");
        return 1;
    }

    obs_inotify.fd.fd = obs_inotify.inotify_instance;
    obs_inotify.fd.events = POLLIN;

    if (ssp_obs_dirs_traversal(obs_inotify.obs) != 0) {
        syslog(LOG_ERR, "SSP IObserver. Directory traversal error");
        return 1;
    }

    for (size_t i = 0; i < obs_inotify.obs->dirs_count; i++) {
        syslog(LOG_INFO, "SSP IObserver. Observing: %s", obs_inotify.obs->dirs[i]);
    }

    return 0;
}

void ssp_obsps_destruct(void)
{
    close(obs_inotify.inotify_instance);
    ssp_obs_destruct(obs_inotify.obs);
    syslog(LOG_INFO, "SSP IObserver. IObserver was destructed");
}

ssp_static void ssp_obsps_event_handle(uint32_t mask, const char* event_name)
{
    switch (mask)
    {
    case IN_MOVED_TO:
    case IN_CREATE:
        if (obs_inotify.obs->filter(event_name) == false) {
            log_warning("Observer. <%s> was filtred", event_name);
            break;
        }

        ssp_obs_storage_insert(obs_inotify.obs, event_name);
        syslog(LOG_INFO, "SSP IObserver. The file <%s> has been moved to the storage", event_name);
        break;
    case IN_MOVED_FROM:
    case IN_DELETE:
        ssp_obs_storage_remove(obs_inotify.obs, event_name);
        syslog(LOG_INFO, "SSP IObserver. The file <%s> has been deleted", event_name);
        break;
    default:
        syslog(LOG_WARNING, "SSP IObserver. Something(%d) has happened with <%s>", mask, event_name);
        break;
    }
}

int ssp_obsps_process(void)
{
    if (poll(&obs_inotify.fd, 1, 0) < 0) {
        syslog(LOG_ERR, "SSP IObserver. Obeserver couldn't poll(): %s", strerror(errno));
        return 1;
    }

    if (obs_inotify.fd.revents & POLLIN){
        char buffer[SSP_OBS_EVENT_BUFFER_SIZE];
        size_t length;
        if ((length = read(obs_inotify.fd.fd, buffer, SSP_OBS_EVENT_BUFFER_SIZE)) > 0) {
            struct inotify_event *event = (struct inotify_event *)buffer;
            while (IN_EVENT_OK(event, length)) {
                int wd_index = ssp_obsps_find_wd_index(event->wd);
                if (wd_index < 0) {
                    syslog(LOG_WARNING, "SSP IObserver. Observer catched an event from unknown wd: %i", event->wd);
                    event = IN_EVENT_NEXT(event, length);
                    continue;
                }

                char dirname[SSP_PATH_MAX_LEN];
                if (ssp_dir_absolute_path(obs_inotify.obs->dirs[wd_index], dirname) != 0) {
                    syslog(LOG_WARNING, "SSP IObserver. <%s> is too long and was truncated", dirname);
                    event = IN_EVENT_NEXT(event, length);
                    continue;
                }

                char fullname[SSP_FULL_NAME_MAX_LEN];
                if (snprintf(fullname, SSP_FULL_NAME_MAX_LEN, "%s/%s", dirname, event->name) >= SSP_FULL_NAME_MAX_LEN) {
                    syslog(LOG_WARNING, "SSP IObserver. <%s> is too long and was truncated", fullname);
                    event = IN_EVENT_NEXT(event, length);
                    continue;
                };

                ssp_obsps_event_handle(event->mask, fullname);
                event = IN_EVENT_NEXT(event, length);
            }
        }
    }

    return 0;
}
