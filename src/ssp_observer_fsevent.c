#include <CoreServices/CoreServices.h>
#include <stdio.h>

void eventCallback(ConstFSEventStreamRef streamRef, void *clientCallBackInfo, size_t numEvents, void *eventPaths, const FSEventStreamEventFlags eventFlags[], const FSEventStreamEventId eventIds[]) {
    char **paths = eventPaths;
    for (size_t i = 0; i < numEvents; i++) {
        if (eventFlags[i] & kFSEventStreamEventFlagItemRemoved) {
            printf("File deleted: %s\n", paths[i]);
        }
        if (eventFlags[i] & kFSEventStreamEventFlagItemCreated) {
            printf("File created: %s\n", paths[i]);
        }
    }
}

int main(int argc, char **argv) {
    CFStringRef path = CFSTR("/Users/nikolajkomarov/dev/slideshow_player/test");

    CFArrayRef pathsToWatch = CFArrayCreate(NULL, (const void **)&path, 1, NULL);
    CFAbsoluteTime latency = 1.0; // Latency in seconds
    FSEventStreamRef stream = FSEventStreamCreate(NULL, &eventCallback, NULL, pathsToWatch, kFSEventStreamEventIdSinceNow, latency, kFSEventStreamCreateFlagFileEvents);

    FSEventStreamScheduleWithRunLoop(stream, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
    FSEventStreamStart(stream);

    CFRunLoopRun();

    FSEventStreamStop(stream);
    FSEventStreamInvalidate(stream);
    FSEventStreamRelease(stream);
    CFRelease(pathsToWatch);

    return 0;
}
