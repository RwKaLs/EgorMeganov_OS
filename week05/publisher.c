#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_SIZE 1024

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    int fds[n];
    char path[16];
    for (int i = 0; i < n; i++) {
        snprintf(path, 16, "/tmp/ex1/s%d", i+1);
        mkfifo(path, 0666);
        fds[i] = open(path, O_WRONLY);
    }
    char input[MAX_SIZE];
    while (fgets(input, MAX_SIZE, stdin)) {
        for (int i = 0; i < n; i++) {
            pid_t pid = fork();
            if (pid == 0) {
                write(fds[i], input, strlen(input));
                exit(EXIT_SUCCESS);
            }
        }
    }
    for (int i = 0; i < n; i++) {
        close(fds[i]);
    }
    exit(EXIT_SUCCESS);
}