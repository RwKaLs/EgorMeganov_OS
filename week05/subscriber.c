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

    if ((fd = open(path, O_RDONLY)) < 0) {
        perror("Failed to open pipe");
        return 1;
    }

    while (1) {
        if ((count = read(fd, input, MAX_SIZE)) > 0){
            input[count] = '\0';
            printf("%s\n", input);
        }
    }
}