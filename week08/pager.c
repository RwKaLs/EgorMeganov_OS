#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <time.h>

#define PSIZE 9

typedef struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
    int counter;
} PTE;

PTE *pageTable;

int nFrames;
int nPages;
int diskAccessCounter = 0;

int myRandom() {

}

int nfu() {
    for(int i = 0; i < nPages; i++) {
        if(pageTable[i].referenced) {
            pageTable[i].referenced = false;
        }
    }
    int min = pageTable[0].counter;
    int index = 0;
    for(int i = 1; i < nPages; i++) {
        if(pageTable[i].counter < min) {
            min = pageTable[i].counter;
            index = i;
        }
    }
    return index;
}

int aging() {

}

void signalHandler(int signum) {

}

void initialize(char disk[nPages][PSIZE], char RAM[nPages][PSIZE]) {
    for (int i = 0; i < nPages; ++i) {
        pageTable[i].valid = false;
        pageTable[i].frame = -1;
        pageTable[i].dirty = false;
        pageTable[i].referenced = 0;
        pageTable[i].counter = 0;
    }
    printf("------------------------------------------\n");
    printf("Initialized disk\n");
    printf("Disk array\n");
    for (int i = 0; i < nPages; i++){
        printf("Page %d ---> %s\n", i, disk[i]);
    }
    printf("------------------------------------------\n");
    printf("Initialized page table\n");
    printf("Page table\n");
    for (int i = 0; i < nPages; ++i){
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d, counter=%d\n",
               i, pageTable[i].valid, pageTable[i].frame, pageTable[i].dirty, pageTable[i].referenced, pageTable[i].counter);
    }
    printf("------------------------------------------\n");
    printf("Initialized memory\n");
    printf("RAM array\n");
    for (int i = 0; i < nFrames; ++i){
        printf("Frame %d ---> %s\n", i, RAM[i]);
    }
}

void genString(char *s, const int len) {
    static const char alph[] ="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < len; ++i) {
        s[i] = alph[rand() % (sizeof(alph) - 1)];
    }

    s[len] = '\0';
}

int main(int argc, char *argv[]) {
    signal(SIGUSR1, signalHandler);
    srand(time(NULL));
    nPages = atoi(argv[1]);
    nFrames = atoi(argv[2]);
    char disk[nPages][PSIZE];
    char RAM[nFrames][PSIZE];
    for (int i = 0; i < nPages; ++i) {
        genString(disk[i], PSIZE - 1);
    }
    int ptSize = nPages * sizeof(PTE);
    int pageTableFileDescriptor = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRWXU);
    ftruncate(pageTableFileDescriptor, ptSize);
    pageTable = mmap(NULL, ptSize, PROT_READ | PROT_WRITE, MAP_SHARED, pageTableFileDescriptor, 0);
    initialize(disk, RAM);
    int requestedPage = -1;
    int mmuProcessId = -1;
    int ind, victim;
    while(1){
        pause();
        for (int i = 0; i < nPages; ++i) {
            if (pageTable[i].referenced != 0) {
                requestedPage = i;
                mmuProcessId = pageTable[i].referenced;
                break;
            }
        }
        if (requestedPage != -1) {
            printf("------------------------------------------\n");
            printf("A disk access request from MMU Process (pid=%d)\n", mmuProcessId);
            printf("Page %d is referenced\n", requestedPage);
            ind = -1;
            for (int i = 0; i < nFrames; ++i){
                if (strcmp(RAM[i], "") == 0){
                    ind = i;
                    break;
                }
            }
            if (ind < 0) {
                printf("We do not have free frames in memory\n");
                victim = rand() % nPages;
                //victim = nfu();
                ind = pageTable[victim].frame;
                while (ind == -1) {
                    victim = rand() % nPages;
                    ind = pageTable[victim].frame;
                }
                printf("Choose a random victim page %d\n", victim);
                printf("Replace/Evict it with page %d to be allocated to frame %d\n", requestedPage, ind);
                if (pageTable[victim].dirty) {
                    strcpy(disk[victim], RAM[ind]);
                    diskAccessCounter++;
                }
                pageTable[victim].valid = false;
                pageTable[victim].frame = -1;
                pageTable[victim].dirty = false;
                pageTable[victim].referenced = 0;
            } else {
                printf("We can allocate it to free frame %d\n", ind);
            }
            printf("Copy data from the disk (page=%d) to memory (frame=%d)\n", requestedPage, ind);
            strcpy(RAM[ind], disk[requestedPage]);
            diskAccessCounter++;
            printf("RAM array\n");
            for (int i = 0; i < nFrames; ++i){
                printf("Frame %d ---> %s\n", i, RAM[i]);
            }
            pageTable[requestedPage].valid = true;
            pageTable[requestedPage].frame = ind;
            pageTable[requestedPage].referenced = 0;
            printf("Disk accesses is %d so far\n", diskAccessCounter);
            printf("Resume MMU process\n");
            kill(mmuProcessId, SIGCONT);
        } else {
            printf("------------------------------------------\n");
            printf("%d disk accesses in total\n", diskAccessCounter);
            munmap(pageTable, ptSize);
            remove("/tmp/ex2/pagetable");
            rmdir("/tmp/ex2");
            printf("Pager is terminated\n");
            exit(EXIT_SUCCESS);
        }
        requestedPage = -1;
        mmuProcessId = -1;
    }
}
