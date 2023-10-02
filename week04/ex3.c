#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    int i;
    for (i = 0; i < n; i++) {
        if (fork() == 0) {
            sleep(5);
            break;
        }
    }
    if (i == n) {
        while (wait(NULL) > 0);
        system("pstree");
    }

    return 0;
}
