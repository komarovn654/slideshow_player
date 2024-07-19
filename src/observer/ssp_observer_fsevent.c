#include <CoreServices/CoreServices.h>
#include <stdio.h>
#include <sys/stat.h>

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
            ssp_syslog(LOG_WARNING, "<FS observer>: <Callback>: Something(0x%X) has happened with <%s>", eventFlags[i], paths[i]);
            continue;
        }

        struct stat file_stat;
        if (stat(paths[i], &file_stat) != 0) {
            ssp_obs_storage_remove(obs_fsevent.obs, paths[i]);
            ssp_syslog(LOG_INFO, "<FS observer>: <Callback>: The file <%s> was deleted", paths[i]);
            continue;
        }

        if (obs_fsevent.obs->filter(paths[i]) == false) {
            ssp_syslog(LOG_WARNING, "<FS observer>: <Callback>: File <%s> was filtred", paths[i]);
            continue;
        }

        ssp_obs_storage_insert(obs_fsevent.obs, paths[i]);
        ssp_syslog(LOG_INFO, "<FS observer>: <Callback>: File <%s> was created", paths[i]);
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
        ssp_syslog(LOG_CRIT, "<FS observer>: <Initialization>: Common observer initialization error");
        return 1;
    }

    if ((ssp_obs_dirs_create(obs_fsevent.obs)) != 0) {
        ssp_syslog(LOG_CRIT, "<FS observer>: <Initialization>: Observer directories creation error");
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
        ssp_syslog(LOG_CRIT, "<FS observer>: <Initialization>: Directory traversal error");
        return 1;
    }

    for (size_t i = 0; i < obs_fsevent.obs->dirs_count; i++) {
        ssp_syslog(LOG_INFO, "<FS observer>: <Initialization>: Observing: %s", obs_fsevent.obs->dirs[i]);
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
    ssp_syslog(LOG_INFO, "<FS observer>: <Destruct>: FS Observer was destructed");
}
