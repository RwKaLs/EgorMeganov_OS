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
    int counterNFU;
    unsigned char counterAging;
} PTE;

typedef struct TLB_entry {
    int page;
    int frame;
} TLB_entry;

TLB_entry* TLB;

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
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d, counterNFU=%d, counterAging=%c\n",
               i, pageTable[i].valid, pageTable[i].frame, pageTable[i].dirty, pageTable[i].referenced, pageTable[i].counterNFU, pageTable[i].counterAging);
    }
    int TLB_size = npages * 0.2;
    TLB = malloc(TLB_size * sizeof(TLB_entry));
    for (int i = 0; i < TLB_size; i++) {
        TLB[i].page = -1;
        TLB[i].frame = -1;
    }
    int tlbHits = 0;
    int hits = 0;
    for (int i = 0; i < nRequests; i++) {
        char accessMode = requests[i][0];
        int pageNumber = atoi(&requests[i][1]);
        pageTable[pageNumber].counterNFU++;
        printf("------------------------------------------\n");
        int TLB_index = -1;
        for (int j = 0; j < TLB_size; j++) {
            if (TLB[j].page == pageNumber) {
                TLB_index = j;
                break;
            }
        }
        if (TLB_index != -1) {
            printf("TLB hit for page %d\n", pageNumber);
            tlbHits++;
        } else {
            printf("TLB miss for page %d\n", pageNumber);
            if (accessMode == 'R') {
                printf("Read request for page %d\n", pageNumber);
                if (pageTable[pageNumber].valid) {
                    printf("It is a valid page\n");
                    hits++;
                    int replace_index = rand() % TLB_size;
                    TLB[replace_index].page = pageNumber;
                    TLB[replace_index].frame = pageTable[pageNumber].frame;
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
                    hits++;
                    int replace_index = rand() % TLB_size;
                    TLB[replace_index].page = pageNumber;
                    TLB[replace_index].frame = pageTable[pageNumber].frame;
                } else {
                    printf("It is not a valid page --> page fault\n");
                    pageTable[pageNumber].referenced = getpid();
                    printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
                    kill(pager_pid, SIGUSR1);
                    pause();
                    pageTable[pageNumber].dirty = true;
                }
            }
        }
        printf("Page table\n");
        for (int j = 0; j < npages; j++){
            printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d, counterNFU=%d, counterAging=%c\n",
                   j, pageTable[j].valid, pageTable[j].frame, pageTable[j].dirty, pageTable[j].referenced, pageTable[j].counterNFU, pageTable[j].counterAging);
        }
        printf("\n");
    }
    printf("Done all requests.\n");
    printf("Hit ratio: %.5f\n", (float)hits / nRequests);
    printf("TLB miss ratio: %.5f\n", 1 - (float) tlbHits / nRequests);
    close(fd);
    printf("MMU sends SIGUSR1 to the pager.\n");
    kill(pager_pid, SIGUSR1);
    printf("MMU terminates.\n");
    munmap(pageTable, npages * sizeof(PTE));
    return 0;
}