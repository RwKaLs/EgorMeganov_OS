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

#define PAGE_SIZE 9

typedef struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
} PTE;

PTE *pageTable;

int nFrames;
int nPages;
int diskAccessCount = 0;

void signalHandler(int signum) {
//    printf("SIGUSR1 signal received.\n");
//
//    FILE *file = fopen("/tmp/mmu.pid", "w");
//    pid_t mmu_pid;
//    fscanf(file, "%d", &mmu_pid);
//    fclose(file);
//
//    int disk_accesses = 0;
//    for (int i = 0; i < npages; i++) {
//        if (page_table[i].referenced != 0) {
//            int frame_to_use = -1;
//            for (int j = 0; j < nframes; j++) {
//                if (strcmp(RAM[j], "_RANDOM!_") == 0) {
//                    frame_to_use = j;
//                    break;
//                }
//            }
//            if (frame_to_use == -1) {
//                frame_to_use = rand() % nframes;
//                if (page_table[frame_to_use].dirty) {
//                    strcpy(disk[frame_to_use], RAM[frame_to_use]);
//                    disk_accesses++;
//                }
//                page_table[frame_to_use].valid = false;
//                page_table[frame_to_use].frame = -1;
//                page_table[frame_to_use].dirty = false;
//                page_table[frame_to_use].referenced = 0;
//            }
//            strcpy(RAM[frame_to_use], disk[i]);
//            page_table[i].valid = true;
//            page_table[i].frame = frame_to_use;
//            page_table[i].dirty = false;
//            page_table[i].referenced = 0;
//            for (int j = 0; j < nframes; j++) {
//                printf("%s\n", RAM[j]);
//            }
//            kill(mmu_pid, SIGCONT);
//            return;
//        }
//    }
//    printf("No non-zero referenced field found. Pager process terminates.\n");
//    printf("Total number of disk accesses: %d\n", disk_accesses);
//    munmap(page_table, npages * sizeof(PTE));
//    close(fd);
//    printf("\nexit--------------\n");
//    exit(0);
}

int main(int argc, char *argv[]) {
    signal(SIGUSR1, signalHandler);
    srand(time(NULL));
    nPages = atoi(argv[1]);
    nFrames = atoi(argv[2]);
    char disk[nPages][PAGE_SIZE];
    char RAM[nFrames][PAGE_SIZE];
    for (int i = 0; i < nPages; i++){
        strcpy(disk[i], "_RANDOM_");
    }
    for (int i = 0; i < nFrames; i++){
        strcpy(RAM[i], "_RANDOM_");
    }
    int pageTableSize = nPages * sizeof(PTE);
    int pageTableFileDescriptor = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (pageTableFileDescriptor == -1) {
        perror("Pagetable open error");
        exit(EXIT_FAILURE);
    }
    ftruncate(pageTableFileDescriptor, pageTableSize);
    pageTable = mmap(NULL, pageTableSize, PROT_READ | PROT_WRITE, MAP_SHARED, pageTableFileDescriptor, 0);
    for (int i = 0; i < nPages; i++) {
        pageTable[i].valid = false;
        pageTable[i].frame = -1;
        pageTable[i].dirty = false;
        pageTable[i].referenced = 0;
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
    for (int i = 0; i < nPages; i++){
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n",
               i, pageTable[i].valid, pageTable[i].frame, pageTable[i].dirty, pageTable[i].referenced);
    }
    printf("------------------------------------------\n");
    printf("Initialized memory\n");
    printf("Memory array\n");
    for (int i = 0; i < nFrames; i++){
        printf("Frame %d ---> %s\n", i, RAM[i]);
    }
    int requestedPage;
    int mmuProcessId;
    while(1){
        pause();
        requestedPage = -1;
        mmuProcessId = -1;
        for (int i = 0; i < nPages; i++) {
            if (pageTable[i].referenced != 0) {
                requestedPage = i;
                mmuProcessId = pageTable[i].referenced;
            }
        }
        if (requestedPage != -1) {
            printf("------------------------------------------\n");
            printf("A disk access request from MMU Process (pid=%d)\n", mmuProcessId);
            printf("Page %d is referenced\n", requestedPage);
            int freeFrameIndex = -1;
            for (int i = 0; i < nFrames; i++){
                if (strcmp(RAM[i], "_RANDOM_") == 0){
                    freeFrameIndex = i;
                    break;
                }
            }
            if (freeFrameIndex < 0) {
                printf("We do not have free frames in memory\n");
                int victimPage = rand() % nPages;
                freeFrameIndex = pageTable[victimPage].frame;
                printf("Choose a random victim page %d\n", victimPage);
                printf("Replace/Evict it with page %d to be allocated to frame %d\n", requestedPage, freeFrameIndex);
                if (pageTable[victimPage].dirty) {
                    strcpy(disk[victimPage], RAM[freeFrameIndex]);
                    diskAccessCount++;
                }
                pageTable[victimPage].valid = false;
                pageTable[victimPage].frame = -1;
                pageTable[victimPage].dirty = false;
                pageTable[victimPage].referenced = 0;
            } else {
                printf("We can allocate it to free frame %d\n", freeFrameIndex);
            }
            printf("Copy data from the disk (page=%d) to memory (frame=%d)\n", requestedPage, freeFrameIndex);
            strcpy(RAM[freeFrameIndex], disk[requestedPage]);
            diskAccessCount++;
            printf("Memory array\n");
            for (int i = 0; i < nFrames; i++){
                printf("Frame %d ---> %s\n", i, RAM[i]);
            }
            pageTable[requestedPage].valid = true;
            pageTable[requestedPage].frame = freeFrameIndex;
            pageTable[requestedPage].referenced = 0;
            printf("Disk accesses is %d so far\n", diskAccessCount);
            printf("Resume MMU process\n");
            kill(mmuProcessId, SIGCONT);
        } else {
            printf("------------------------------------------\n");
            printf("%d disk accesses in total\n", diskAccessCount);
            munmap(pageTable, pageTableSize);
            remove("/tmp/ex2/pagetable");
            rmdir("/tmp/ex2");
            printf("Pager is terminated\n");
            exit(EXIT_SUCCESS);
        }
    }
}
