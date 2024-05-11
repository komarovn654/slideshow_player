#include <CoreServices/CoreServices.h>

#include "logman/logman.h"

#include "ssp_observer.h"
#include "ssp_list.h"
#include "ssp_helper.h"

static struct observer {
    FSEventStreamRef stream;
    CFAbsoluteTime latency;
    CFArrayRef paths;
    ssp_list images_list;
} observer;

void ssp_fsevent_callback(ConstFSEventStreamRef streamRef, void *clientCallBackInfo, size_t numEvents, void *eventPaths,
    const FSEventStreamEventFlags eventFlags[],
    const FSEventStreamEventId eventIds[])
{
    int i;
    char **paths = eventPaths;
 
    // printf("Callback called\n");
    for (i=0; i<numEvents; i++) {
        /* flags are unsigned long, IDs are uint64_t */
        // printf("Change %llu in %s, flags 0x%X\n", eventIds[i], paths[i], eventFlags[i]);
        if ((eventFlags[i] & kFSEventStreamEventFlagItemIsFile) == 0) {
            log_error("Something(0x%X) has happened with <%s>", eventFlags[i], paths[i]);
            continue;
        }
        if ((eventFlags[i] & kFSEventStreamEventFlagItemRemoved) != 0) {
            log_info("File <%s> has been deleted", paths[i]);
            continue;
        }
        if ((eventFlags[i] & kFSEventStreamEventFlagItemCreated) != 0) {
            log_info("File <%s> has been created", paths[i]);
            continue;
        }
   }
}

int ssp_obs_process(void)
{
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, observer.latency, true);

    return 0;
}

int ssp_obs_init(const char *dir_path)
{
    CFStringRef path = CFStringCreateWithCString(NULL, dir_path, kCFStringEncodingASCII);
    observer.paths = CFArrayCreate(NULL, (const void **)&path, 1, NULL);
    observer.latency = 0.0; // Latency in seconds
    observer.stream = FSEventStreamCreate(NULL, &ssp_fsevent_callback, NULL, observer.paths, kFSEventStreamEventIdSinceNow, 
        observer.latency, kFSEventStreamCreateFlagFileEvents);

    dispatch_queue_t queue = dispatch_queue_create("com.example.fileevents", DISPATCH_QUEUE_SERIAL);
    FSEventStreamSetDispatchQueue(observer.stream, queue);
    
    FSEventStreamStart(observer.stream);

    if ((observer.images_list = ssp_list_init()) == NULL) {
        return 1;
    }

    if ((ssp_dir_traversal(dir_path, (void (*)(void *, const char *))ssp_list_insert, observer.images_list)) != 0) {
        return 1;
    }

    return 0;
}

void ssp_obs_deinit(void)
{
    FSEventStreamStop(observer.stream);
    FSEventStreamInvalidate(observer.stream);
    FSEventStreamRelease(observer.stream);
    CFRelease(observer.paths);
}

static void ssp_obs_save_image(char **storage, const char *image)
{

}

int ssp_obs_images_list(char **images)
{

    return 0;
}