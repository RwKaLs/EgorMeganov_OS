#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

struct inode {
    char name[16]; // file name
    int size; // file size (in number of blocks)
    int blockPointers[8]; // direct block pointers
    int used; // 0 => inode is free; 1 => in use
};

struct superblock {
    char freeBlockList[128];
    struct inode inodes[16];
};

struct disk {
    struct superblock superBlock;
    char blocks[127][1024];
};

struct disk mydisk;

void create(char name[16], int size) {
    for (int i = 0; i < 16; i++) {
        if (mydisk.superBlock.inodes[i].used == 0) {
            int freeBlocks = 0;
            for (int j = 0; j < 128; j++) {
                if (mydisk.superBlock.freeBlockList[j] == 0) {
                    freeBlocks++;
                }
            }
            if (freeBlocks < size) {
                printf("Not enough space on disk\n");
                return;
            }
            mydisk.superBlock.inodes[i].used = 1;
            strncpy(mydisk.superBlock.inodes[i].name, name, 16);
            mydisk.superBlock.inodes[i].size = size;
            int blockCount = 0;
            for (int j = 0; j < 128 && blockCount < size; j++) {
                if (mydisk.superBlock.freeBlockList[j] == 0) {
                    mydisk.superBlock.freeBlockList[j] = 1;
                    mydisk.superBlock.inodes[i].blockPointers[blockCount] = j;
                    blockCount++;
                }
            }
            return;
        }
    }
    printf("No more free inodes\n");
}

void delete(char name[16]) {
    for (int i = 0; i < 16; i++) {
        if (mydisk.superBlock.inodes[i].used == 1 && strncmp(mydisk.superBlock.inodes[i].name, name, 16) == 0) {
            for (int j = 0; j < mydisk.superBlock.inodes[i].size; j++) {
                mydisk.superBlock.freeBlockList[mydisk.superBlock.inodes[i].blockPointers[j]] = 0;
            }
            mydisk.superBlock.inodes[i].used = 0;
            return;
        }
    }
    printf("File not found\n");
}

void myRead(char name[16], int blockNum, char buf[1024]) {
    for (int i = 0; i < 16; i++) {
        if (mydisk.superBlock.inodes[i].used == 1 && strncmp(mydisk.superBlock.inodes[i].name, name, 16) == 0) {
            if (blockNum < 0 || blockNum >= mydisk.superBlock.inodes[i].size) {
                printf("Invalid block number\n");
                return;
            }
            memcpy(buf, mydisk.blocks[mydisk.superBlock.inodes[i].blockPointers[blockNum]], 1024);
            return;
        }
    }
    printf("File not found\n");
}

void myWrite(char name[16], int blockNum, char buf[1024]) {
    for (int i = 0; i < 16; i++) {
        if (mydisk.superBlock.inodes[i].used == 1 && strncmp(mydisk.superBlock.inodes[i].name, name, 16) == 0) {
            if (blockNum < 0 || blockNum >= mydisk.superBlock.inodes[i].size) {
                printf("Invalid block number\n");
                return;
            }
            memcpy(mydisk.blocks[mydisk.superBlock.inodes[i].blockPointers[blockNum]], buf, 1024);
            return;
        }
    }
    printf("File not found\n");
}

void ls(void) {
    for (int i = 0; i < 16; i++) {
        if (mydisk.superBlock.inodes[i].used == 1) {
            printf("%s %d\n", mydisk.superBlock.inodes[i].name, mydisk.superBlock.inodes[i].size);
        }
    }
}

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDWR);
    read(fd, &mydisk, sizeof(mydisk));
    close(fd);

    FILE *file = fopen(argv[2], "r");
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char cmd[2], name[16], buf[1024];
        int size, blockNum;
        sscanf(line, "%s %s %d %d %s", cmd, name, &size, &blockNum, buf);
        switch (cmd[0]) {
            case 'C':
                create(name, size);
                break;
            case 'D':
                delete(name);
                break;
            case 'R':
                myRead(name, blockNum, buf);
                break;
            case 'W':
                myWrite(name, blockNum, buf);
                break;
            case 'L':
                ls();
                break;
        }
    }
    fclose(file);

    fd = open(argv[1], O_RDWR);
    write(fd, &mydisk, sizeof(mydisk));
    close(fd);
    return EXIT_SUCCESS;
}
