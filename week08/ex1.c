#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

char *shared;

void handler() {
    munmap(shared, 14);
    remove("/tmp/ex1.pid");
    exit(EXIT_SUCCESS);
}

int main() {
    signal(SIGTERM, handler);
    FILE *file = fopen("/tmp/ex1.pid", "w");
    fprintf(file, "%d", getpid());
    fclose(file);
    char password[14] = "pass:";
    int urandom = open("/dev/urandom", O_RDONLY);
    char c;
    for (int i = 5; i < 13; i++) {
        read(urandom, &c, 1);
        while (!isprint(c)) {
            read(urandom, &c, 1);
        }
        password[i] = c;
    }
    close(urandom);
    password[13] = '\0';
    shared = mmap(NULL, 14, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    strcpy(shared, password);
    while (1) {}
}
