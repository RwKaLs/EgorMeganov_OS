#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <signal.h>
#include <dirent.h>

typedef struct inotify_event inotify_event;

int fd, wd;
char *path;

void print_stat(char *path) {
    struct stat st;
    stat(path, &st);
    printf("File: %s\nSize: %ld bytes\nInode number: %ld\nLast access time: %ld\nLast modification time: %ld\nLast status change time: %ld\nNumber of hard links: %lu\nUser ID of owner: %u\nGroup ID of owner: %u\nBlocksize for filesystem I/O: %ld\nNumber of 512B blocks allocated: %ld\n\n",
           path,
           st.st_size,
           st.st_ino,
           st.st_atime,
           st.st_mtime,
           st.st_ctime,
           st.st_nlink,
           st.st_uid,
           st.st_gid,
           st.st_blksize,
           st.st_blocks);
}

void print_all_stat(char *path) {
    printf("\n----------------------\n\n");
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char file_path[1024];
    if (!(dir = opendir(path))) {
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        snprintf(file_path, sizeof(file_path), "%s/%s", path, entry->d_name);
        if (stat(file_path, &st) == -1) {
            continue;
        }
        printf("File: %s\nSize: %ld bytes\nInode number: %ld\nLast access time: %ld\nLast modification time: %ld\nLast status change time: %ld\nNumber of hard links: %lu\nUser ID of owner: %u\nGroup ID of owner: %u\nBlocksize for filesystem I/O: %ld\nNumber of 512B blocks allocated: %ld\n\n",
               file_path,
               st.st_size,
               st.st_ino,
               st.st_atime,
               st.st_mtime,
               st.st_ctime,
               st.st_nlink,
               st.st_uid,
               st.st_gid,
               st.st_blksize,
               st.st_blocks);

    }
    closedir(dir);
    printf("----------------------\n\n");
}


void handle_sigint(int sig) {
    print_all_stat(path);
    inotify_rm_watch(fd, wd);
    close(fd);
    exit(0);
}

int main(int argc, char **argv) {
    signal(SIGINT, handle_sigint);
    char buffer[1024 * (sizeof(inotify_event) + 16)];
    int length, i;
    path = argv[1];
    print_all_stat(path);
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
