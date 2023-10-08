#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_SIZE 1024

int main() {
    int fd[2];
    pid_t pid;
    pipe(fd);
    char input[MAX_SIZE];
    pid = fork();
    if (pid == 0) { // child
        close(fd[1]);
        ssize_t count;
        while ((count = read(fd[0], input, MAX_SIZE - 1)) > 0) {
            input[count] = '\0';
            printf("%s\n", input);
        }
        close(fd[0]);
        _exit(EXIT_SUCCESS);
    } else { // parent
        close(fd[0]);
        while (fgets(input, MAX_SIZE, stdin)) {
	    write(fd[1], input, strlen(input));
        }
        close(fd[1]);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}
