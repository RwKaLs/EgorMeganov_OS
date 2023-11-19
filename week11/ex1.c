#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    DIR *d;
    struct dirent *dir;

    if (argc > 1 && strcmp(argv[1], "--loftdisk") == 0) {
        chroot("/loftdisk");
    }

    d = opendir("/");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }

    return EXIT_SUCCESS;
}

