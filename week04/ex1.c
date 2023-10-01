#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

int main() {
    pid_t pid1, pid2;
    clock_t start;
    pid1 = fork();
    start = clock();
    if (pid1 == 0) {
        printf("Child 1\n{\nPID = %d\nParent PID = %d\nExec time = %f\n}\n", getpid(), getppid(),
               (((double) (clock() - start)) / CLOCKS_PER_SEC) * 1000);
        return 0;
    }
    pid2 = fork();
    start = clock();
    if (pid2 == 0) {
        printf("Child 2\n{\nPID = %d\nParent PID = %d\nExec time = %f\n}\n", getpid(), getppid(),
               (((double) (clock() - start)) / CLOCKS_PER_SEC) * 1000);
        return 0;
    }
    printf("Parent\n{\nPID = %d\nParent PID = %d\nExec time = %f\n}\n", getpid(), getppid(),
           (((double) (clock() - start)) / CLOCKS_PER_SEC) * 1000);
    return 0;
}
