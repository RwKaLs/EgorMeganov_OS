#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_HEAP_SIZE 10000000

typedef struct {
    unsigned int adrs;
    int size;
} Block;

Block memory[MAX_HEAP_SIZE];

void allocate_first_fit(unsigned int adrs, int size) {
    int i;
    int freeSpace = 0;
    for (i = 0; i < MAX_HEAP_SIZE; i++) {
        if (memory[i].size == 0) {
            freeSpace++;
        } else {
            freeSpace = 0;
            i += memory[i].size-1;
        }
        if (freeSpace == size) {
            memory[i-size+1].adrs = adrs;
            memory[i-size+1].size = size;
            printf("First Fit: Allocated %d bytes at address %u\n", size, adrs);
            return;
        }
    }
    if (i == MAX_HEAP_SIZE) {
        printf("First Fit: Error! No sufficient memory to allocate %d bytes\n", size);
    }
}

void allocate_best_fit(unsigned int adrs, int size) {
    int bestIndex = -1;
    int freeSpace = 0;
    int bestFit = MAX_HEAP_SIZE + 1;
    for (int i = 0; i < MAX_HEAP_SIZE; i++) {
        if (memory[i].size == 0) {
            freeSpace++;
        } else {
            if (freeSpace < bestFit && freeSpace >= size) {
                bestIndex = i - freeSpace;
                bestFit = freeSpace;
            }
            freeSpace = 0;
            i += memory[i].size-1;
        }
    }
    if (bestIndex != -1) {
        if (freeSpace < bestFit) {
            memory[MAX_HEAP_SIZE-freeSpace].adrs = adrs;
            memory[MAX_HEAP_SIZE-freeSpace].size = size;
            printf("Best Fit: Allocated %d bytes at address %u\n", size, adrs);
        } else {
            memory[bestIndex].adrs = adrs;
            memory[bestIndex].size = size;
            printf("Best Fit: Allocated %d bytes at address %u\n", size, adrs);
        }
    } else if (freeSpace > size) {
        memory[MAX_HEAP_SIZE-freeSpace].adrs = adrs;
        memory[MAX_HEAP_SIZE-freeSpace].size = size;
        printf("Best Fit: Allocated %d bytes at address %u\n", size, adrs);
    } else {
        printf("Best Fit: Error! No sufficient memory to allocate %d bytes\n", size);
    }
}

void allocate_worst_fit(unsigned int adrs, int size) {
    int worstIndex = -1;
    int freeSpace = 0;
    int worstFit = -1;
    for (int i = 0; i < MAX_HEAP_SIZE; i++) {
        if (memory[i].size == 0) {
            freeSpace++;
        } else {
            if (freeSpace > worstFit && freeSpace >= size) {
                worstIndex = i - freeSpace;
                worstFit = freeSpace;
            }
            freeSpace = 0;
            i += memory[i].size-1;
        }
    }
    if (worstIndex != -1) {
        if (freeSpace > worstFit) {
            memory[MAX_HEAP_SIZE-freeSpace].adrs = adrs;
            memory[MAX_HEAP_SIZE-freeSpace].size = size;
            printf("Worst Fit: Allocated %d bytes at address %u\n", size, adrs);
        } else {
            memory[worstIndex].adrs = adrs;
            memory[worstIndex].size = size;
            printf("Worst Fit: Allocated %d bytes at address %u\n", size, adrs);
        }
    } else if (freeSpace > size) {
        memory[MAX_HEAP_SIZE-freeSpace].adrs = adrs;
        memory[MAX_HEAP_SIZE-freeSpace].size = size;
        printf("Worst Fit: Allocated %d bytes at address %u\n", size, adrs);
    } else {
        printf("Worst Fit: Error! No sufficient memory to allocate %d bytes\n", size);
    }
}

void clear(unsigned int adrs) {
    int i;
    for (i = 0; i < MAX_HEAP_SIZE; i++) {
        if (memory[i].adrs == adrs) {
            printf("Clear: Freed memory at address %u\n", adrs);
            memory[i].adrs = 0;
            memory[i].size = 0;
            break;
        }
    }
    if (i == MAX_HEAP_SIZE) {
        printf("Clear: Error! No block found with address %u\n", adrs);
    }
}

int main() {
    FILE *file = fopen("queries.txt", "r");
    if (file == NULL) {
        printf("Could not open file queries.txt");
        return 1;
    }
    char query[20];
    unsigned int adrs;
    int size;
    int n = 0;
    while (fscanf(file, "%s", query) != EOF) n++;
    clock_t start, end;
    fseek(file, 0, SEEK_SET);

    // First Fit
    start = clock();
    while (fscanf(file, "%s", query) != EOF) {
        if (strcmp(query, "allocate") == 0) {
            fscanf(file, "%u %d", &adrs, &size);
            allocate_first_fit(adrs, size);
        } else if (strcmp(query, "clear") == 0) {
            fscanf(file, "%u", &adrs);
            clear(adrs);
        } else if (strcmp(query, "end") == 0) {
            break;
        }
    }
    end = clock();
    double firstT = (double) n / ((double) (end - start) / CLOCKS_PER_SEC);
    fseek(file, 0, SEEK_SET);
    memset(memory, 0, sizeof(memory));

    // Best Fit
    start = clock();
    while (fscanf(file, "%s", query) != EOF) {
        if (strcmp(query, "allocate") == 0) {
            fscanf(file, "%u %d", &adrs, &size);
            allocate_best_fit(adrs, size);
        } else if (strcmp(query, "clear") == 0) {
            fscanf(file, "%u", &adrs);
            clear(adrs);
        } else if (strcmp(query, "end") == 0) {
            break;
        }
    }
    end = clock();
    double bestT = (double) n / (((double) (end - start)) / CLOCKS_PER_SEC);
    fseek(file, 0, SEEK_SET);
    memset(memory, 0, sizeof(memory));

    // Worst Fit
    start = clock();
    while (fscanf(file, "%s", query) != EOF) {
        if (strcmp(query, "allocate") == 0) {
            fscanf(file, "%u %d", &adrs, &size);
            allocate_worst_fit(adrs, size);
        } else if (strcmp(query, "clear") == 0) {
            fscanf(file, "%u", &adrs);
            clear(adrs);
        } else if (strcmp(query, "end") == 0) {
            break;
        }
    }
    end = clock();
    double worstT = (double) n / ((double) (end - start) / CLOCKS_PER_SEC);
    printf("First Fit throughput: %f\n", firstT);
    printf("Best Fit throughput: %f\n", bestT);
    printf("Worst Fit throughput: %f\n", worstT);
    fseek(file, 0, SEEK_SET);
    memset(memory, 0, sizeof(memory));
    fclose(file);
    return 0;
}