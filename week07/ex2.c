#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>

int main() {
    int fd = open("/dev/random", O_RDONLY);
    if (fd == -1) {
        perror("open /dev/random");
        return 1;
    }
    FILE *file = fopen("text.txt", "w");
    if (file == NULL) {
        perror("fopen text.txt");
        return 1;
    }
    long sz = sysconf(_SC_PAGESIZE);
    long file_size = 500*1024*1024;
    long chunk_size = 1024*sz;
    char c;
    int count = 0;
    long i = 0;
    while (i < file_size) {
        if (read(fd, &c, 1) != 1) {
            perror("read /dev/random");
            return 1;
        }
        if (isprint(c)) {
            fputc(c, file);
            ++i;
            if (++count == 1024) {
                ++i;
                fputc('\n', file);
                count = 0;
            }
        }
    }
    fclose(file);
    close(fd);
    fd = open("text.txt", O_RDWR);
    if (fd == -1) {
        perror("open text.txt");
        return 1;
    }
    int total = 0;
    for (i = 0; i < file_size; i += chunk_size) {
        long current_chunk_size = chunk_size;
        if (i + current_chunk_size > file_size) {
            current_chunk_size = file_size - i;
        }
        char *data = mmap(NULL, current_chunk_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, i);
        if (data == MAP_FAILED) {
            perror("mmap");
            return 1;
        }
        for (int j = 0; j < current_chunk_size; ++j) {
            if (isupper(data[j])) {
                ++total;
                data[j] = tolower(data[j]);
            }
        }
        if (munmap(data, current_chunk_size) == -1) {
            perror("munmap");
            return 1;
        }
    }
    printf("Total uppercase letters: %d\n", total);
    close(fd);
    return 0;
}