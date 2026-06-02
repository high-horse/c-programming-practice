#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/inotify.h>

const char *evaluate_event(struct inotify_event *event) {
    // Check for directory flag first; can be combined with other bits
    if (event->mask & IN_ISDIR) {
        /* You can handle directory-specific logic here if needed */
        return "inside dir";
    }

    if (event->mask & IN_ACCESS)        return "File accessed";
    if (event->mask & IN_ATTRIB)        return "Metadata changed";
    if (event->mask & IN_CLOSE_WRITE)   return "File closed (written)";
    if (event->mask & IN_CLOSE_NOWRITE) return "File closed (no write)";
    if (event->mask & IN_CREATE)        return "Created";
    if (event->mask & IN_DELETE)        return "Deleted";
    if (event->mask & IN_DELETE_SELF)   return "Watched item deleted";
    if (event->mask & IN_MODIFY)        return "Modified";
    if (event->mask & IN_MOVE_SELF)     return "Watched item moved";
    if (event->mask & IN_MOVED_FROM)    return "Moved out";
    if (event->mask & IN_MOVED_TO)      return "Moved in";
    if (event->mask & IN_OPEN)          return "Opened";
    
    // System events
    if (event->mask & IN_IGNORED)       return "Watch removed";
    if (event->mask & IN_Q_OVERFLOW)    return "Event queue overflowed";
    if (event->mask & IN_UNMOUNT)       return "Filesystem unmounted";

    return "Unknown Event";
}


int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("USAGE: %s <dir-name-or-path>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *dir = argv[1];

    int fd = inotify_init();
    if(fd < 0) {
        perror("Failed to init inotify");
        goto err_cleanup;
    }

    int wd = inotify_add_watch(fd, dir, IN_CLOSE_WRITE | IN_CLOSE_NOWRITE | IN_DELETE | IN_MODIFY);
    if(wd < 0) {
        perror("Failed to add watch");
        goto err_cleanup;
    } 

    bool running = true;
    printf("watching dir '%s'\n", dir);
    size_t len = 0;
    char buffer[4098];

    while (running)
    {
        len = read(fd, buffer, sizeof(buffer));
        if(len < 0) {
            perror("Failed to read from fd");
            goto err_cleanup;
        }

        size_t i = 0;
        while (i < len)
        {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];
            const char *msg = evaluate_event(event);
            
            printf("%s: %s\n", msg, event->name);
            i += sizeof(struct inotify_event ) + event->len;
        }
        
    }
    

    goto ok_cleanup;

    err_cleanup:
        inotify_rm_watch(fd, wd);
        return EXIT_FAILURE;
    

    ok_cleanup:
        inotify_rm_watch(fd, wd);
        return EXIT_SUCCESS;

}