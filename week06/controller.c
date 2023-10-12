#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

void handle_sigint() {
    FILE *file = fopen("/var/run/agent.pid", "r");
    int pid;
    fscanf(file, "%d", &pid);
    fclose(file);
    kill(pid, SIGTERM);
    exit(0);
}

int main() {
    FILE *file = fopen("/var/run/agent.pid", "r");
    if (file == NULL) {
        printf("Error: No agent found.\n");
        return 0;
    }
    int pid;
    fscanf(file, "%d", &pid);
    fclose(file);
    printf("Agent found.\n");
    signal(SIGINT, handle_sigint);

    while(1) {
        printf("Choose a command {“read”, “exit”, “stop”, “continue”} to send to the agent\n");
        char command[10];
        scanf("%s", command);
        if (strcmp(command, "read") == 0) {
            kill(pid, SIGUSR1);
        } else if (strcmp(command, "exit") == 0) {
            kill(pid, SIGUSR2);
            break;
        } else if (strcmp(command, "stop") == 0) {
            kill(pid, SIGSTOP);
        } else if (strcmp(command, "continue") == 0) {
            kill(pid, SIGCONT);
        }
    }
    return 0;
}
