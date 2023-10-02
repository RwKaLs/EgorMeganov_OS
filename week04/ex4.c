#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(void) {
    char buf[1024];
    pid_t pid;
    int status;
    printf("$ ");
    while (scanf("%s", buf) != EOF) {
        if ((pid = fork()) < 0) {
            printf("fork error");
        } else if (pid == 0) {
            char *argv[] = { "/bin/sh", "-c", buf, NULL };
            if (execve("/bin/sh", argv, NULL) == -1) {
                printf("couldn't execute: %s", buf);
                exit(127);
            }
        }
        if ((pid = waitpid(pid, &status, 0)) < 0) {
            printf("waitpid error");
        }
        printf("$ ");
    }
    return 0;
}
