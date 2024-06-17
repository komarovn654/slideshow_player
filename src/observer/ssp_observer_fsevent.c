#include <CoreServices/CoreServices.h>
#include <stdio.h>
#include <sys/stat.h>
#include <syslog.h>

#include "ssp_memory.h"
#include "ssp_observer.h"
#include "ssp_observer_ps.h"

static struct obs_fsevent {
    FSEventStreamRef stream;
    CFAbsoluteTime latency;
    CFArrayRef paths;

    ssp_observer* obs;
} obs_fsevent;

void ssp_fsevent_callback(ConstFSEventStreamRef streamRef, void *clientCallBackInfo, size_t numEvents, void *eventPaths,
    const FSEventStreamEventFlags eventFlags[],
    const FSEventStreamEventId eventIds[])
{
    int i;
    char **paths = eventPaths;
 
    for (i=0; i<numEvents; i++) {
        if ((eventFlags[i] & kFSEventStreamEventFlagItemIsFile) == 0) {
            syslog(LOG_WARNING, "SSP FSObserver. Something(0x%X) has happened with <%s>", eventFlags[i], paths[i]);
            continue;
        }

        struct stat file_stat;
        if (stat(paths[i], &file_stat) != 0) {
            ssp_obs_storage_remove(obs_fsevent.obs, paths[i]);
            syslog(LOG_INFO, "SSP FSObserver. The file <%s> has been deleted", paths[i]);
            continue;
        }

        if (obs_fsevent.obs->filter(paths[i]) == false) {
            syslog(LOG_WARNING, "SSP FSObserver. <%s> was filtred", paths[i]);
            continue;
        }

        ssp_obs_storage_insert(obs_fsevent.obs, paths[i]);
        syslog(LOG_INFO, "SSP FSObserver. File <%s> has been created", paths[i]);
   }
}

int ssp_obsps_process(void)
{
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, obs_fsevent.latency, true);

    return 0;
}

int ssp_obsps_init(ssp_observer settings)
{
    if ((obs_fsevent.obs = ssp_obs_init(settings)) == NULL) {
        syslog(LOG_ERR, "SSP FSObserver. Common observer initialization error");
        return 1;
    }

    if ((ssp_obs_dirs_create(obs_fsevent.obs)) != 0) {
        syslog(LOG_ERR, "SSP FSObserver. Observer directories creation error");
        return 1;
    }

    CFStringRef path[SSP_OBS_DIRS_MAX_COUNT] = { NULL };
    for (size_t i = 0; i < obs_fsevent.obs->dirs_count; i++) {
        path[i] = CFStringCreateWithCString(NULL, obs_fsevent.obs->dirs[i], kCFStringEncodingASCII);
    }

    obs_fsevent.paths = CFArrayCreate(NULL, (const void **)&path, obs_fsevent.obs->dirs_count, NULL);
    obs_fsevent.latency = 0.01; // Latency in seconds
    obs_fsevent.stream = FSEventStreamCreate(NULL, &ssp_fsevent_callback, NULL, obs_fsevent.paths, kFSEventStreamEventIdSinceNow, 
        obs_fsevent.latency, kFSEventStreamCreateFlagFileEvents);

    dispatch_queue_t queue = dispatch_queue_create("com.example.fileevents", DISPATCH_QUEUE_SERIAL);
    FSEventStreamSetDispatchQueue(obs_fsevent.stream, queue);
    
    FSEventStreamStart(obs_fsevent.stream);

    if (ssp_obs_dirs_traversal(obs_fsevent.obs) != 0) {
        syslog(LOG_ERR, "SSP FSObserver. Directory traversal error");
        return 1;
    }

    for (size_t i = 0; i < obs_fsevent.obs->dirs_count; i++) {
        syslog(LOG_INFO, "SSP FSObserver. Observing: %s", obs_fsevent.obs->dirs[i]);
    }

    return 0;
}

void ssp_obsps_destruct(void)
{
    FSEventStreamStop(obs_fsevent.stream);
    FSEventStreamInvalidate(obs_fsevent.stream);
    FSEventStreamRelease(obs_fsevent.stream);
    CFRelease(obs_fsevent.paths);

    ssp_obs_destruct(obs_fsevent.obs);
    syslog(LOG_INFO, "SSP FSObserver. FSObserver was destructed");
}
