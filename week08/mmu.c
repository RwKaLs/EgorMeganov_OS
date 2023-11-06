#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

typedef struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
} PTE;

PTE *pageTable;

void handler(int signum){
    if (signum == SIGCONT) {
        printf("MMU resumed by SIGCONT signal from pager\n");
    }
}

int main(int argc, char *argv[]) {
    signal(SIGCONT, handler);
    int npages = atoi(argv[1]);
    int nRequests = argc - 3;
    char requests[nRequests][5];
    for (int i = 0; i < nRequests; i++){
        strcpy(requests[i], argv[i + 2]);
    }
    int pager_pid = atoi(argv[argc - 1]);
    int fd = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRWXU);
    ftruncate(fd, npages * sizeof(PTE));
    pageTable = mmap(NULL, npages * sizeof(PTE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    for (int i = 0; i < npages; i++) {
        pageTable[i].valid = false;
        pageTable[i].frame = -1;
        pageTable[i].dirty = false;
        pageTable[i].referenced = 0;
    }
    printf("------------------------------------------\n");
    printf("Initialized page table\n");
    printf("Page table\n");
    for (int i = 0; i < npages; i++){
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n",
               i, pageTable[i].valid, pageTable[i].frame, pageTable[i].dirty, pageTable[i].referenced);
    }
    for (int i = 0; i < nRequests; i++) {
        sleep(1);
        char accessMode = requests[i][0];
        int pageNumber = atoi(&requests[i][1]);
        printf("------------------------------------------\n");
        if (accessMode == 'R') {
            printf("Read request for page %d\n", pageNumber);
            if (pageTable[pageNumber].valid) {
                printf("It is a valid page\n");
            } else {
                printf("It is not a valid page --> page fault\n");
                pageTable[pageNumber].referenced = getpid();
                printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
                kill(pager_pid, SIGUSR1);
                pause();
            }
        } else if (accessMode == 'W') {
            printf("Write request for page %d\n", pageNumber);
            if (pageTable[pageNumber].valid) {
                printf("It is a valid page\n");
                printf("It is a write request then set the dirty field\n");
                pageTable[pageNumber].dirty = true;
            } else {
                printf("It is not a valid page --> page fault\n");
                pageTable[pageNumber].referenced = getpid();
                printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
                kill(pager_pid, SIGUSR1);
                pause();
                pageTable[pageNumber].dirty = true;
            }
        }
        printf("Page table\n");
        for (int j = 0; j < npages; j++){
            printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n",
                   j, pageTable[j].valid, pageTable[j].frame, pageTable[j].dirty, pageTable[j].referenced);
        }
        printf("\n");
    }
    printf("Done all requests.\n");
    close(fd);
    printf("MMU sends SIGUSR1 to the pager.\n");
    kill(pager_pid, SIGUSR1);
    printf("MMU terminates.\n");
    munmap(pageTable, npages * sizeof(PTE));
    return 0;
}
