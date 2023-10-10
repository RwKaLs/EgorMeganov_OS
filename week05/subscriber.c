#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_SIZE 1024

int main(int argc, char *argv[]) {
    int id = atoi(argv[1]);
    char path[MAX_SIZE];
    snprintf(path, MAX_SIZE, "/tmp/ex1/s%d", id);
    int fd;
    char input[MAX_SIZE];
    ssize_t count;
    while (1) {
        fd = open(path, O_RDONLY);
        if ((count = read(fd, input, MAX_SIZE)) > 0){
            input[count] = '\0';
            printf("%s\n", input);
        }
        close(fd);
    }
}