#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_MSG_SIZE 1024

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    char buf[MAX_MSG_SIZE];
    char pipe_name[16];

    for (int i = 0; i < n; ++i) {
        sprintf(pipe_name, "/tmp/ex1/s%d", i + 1);
        mkfifo(pipe_name, 0666);
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return 1;
        }
        if (pid == 0) {
            int fd = open(pipe_name, O_WRONLY);
            while(fgets(buf, MAX_MSG_SIZE, stdin)){
                write(fd, buf, strlen(buf));
            }
            close(fd);
            exit(EXIT_SUCCESS);
        }
    }
    while(1) {
        sleep(1);
    }
}
