#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <signal.h>

typedef struct inotify_event inotify_event;

int fd, wd;
char *path;

void print_stat(char *path) {
    struct stat st;
    stat(path, &st);
    printf("File: %s, Size: %ld\n", path, st.st_size);
}

void handle_sigint(int sig) {
    print_stat(path);
    inotify_rm_watch(fd, wd);
    close(fd);
    exit(0);
}

int main(int argc, char **argv) {
    signal(SIGINT, handle_sigint);
    char buffer[1024 * (sizeof(inotify_event) + 16)];
    int length, i;
    path = argv[1];
    print_stat(path);
    fd = inotify_init();
    wd = inotify_add_watch(fd, path, IN_ACCESS | IN_CREATE | IN_DELETE | IN_MODIFY | IN_OPEN | IN_ATTRIB);
    while(1) {
        i = 0;
        length = read(fd, buffer, 1024 * (sizeof(inotify_event) + 16));
        while(i < length) {
            inotify_event *event = (inotify_event *) &buffer[i];
            if (event->len) {
                if (event->mask & IN_CREATE) {
                    printf("%s was created.\n", event->name);
                } else if (event->mask & IN_DELETE) {
                    printf("%s was deleted.\n", event->name);
                } else if (event->mask & IN_MODIFY) {
                    printf("%s was modified.\n", event->name);
                } else if (event->mask & IN_ACCESS) {
                    printf("%s was accessed.\n", event->name);
                } else if (event->mask & IN_OPEN) {
                    printf("%s was opened.\n", event->name);
                } else if (event->mask & IN_ATTRIB) {
                    printf("%s metadata changed.\n", event->name);
                }
                print_stat(event->name);
            }
            i += sizeof(inotify_event) + event->len;
        }
    }
}
