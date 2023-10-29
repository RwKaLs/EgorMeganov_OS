#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

#define SIZE 10 * 1024 * 1024

int main() {
    struct rusage usage;
    for (int i = 0; i < 10; i++) {
        void *p = malloc(SIZE);
        memset(p, 0, SIZE);
        getrusage(RUSAGE_SELF, &usage);
        printf("Memory usage: %ld\n", usage.ru_maxrss);
        sleep(1);
    }
    return 0;
}
