#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handle_sigusr1() {
    FILE *file = fopen("text.txt", "r");
    char c;
    while ((c = getc(file)) != EOF) {
        putchar(c);
    }
    fclose(file);
}

void handle_sigusr2() {
    printf("Process terminating...\n");
    exit(0);
}

int main() {
    FILE *file = fopen("/var/run/agent.pid", "w");
    fprintf(file, "%d", getpid());
    fclose(file);

    signal(SIGUSR1, handle_sigusr1);
    signal(SIGUSR2, handle_sigusr2);
    while(1) {
        pause();
    }
}
