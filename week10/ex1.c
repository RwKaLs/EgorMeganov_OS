#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

char *path;

void print_stat(const char* file) {
    struct stat st;
    char file_path[1024];
    snprintf(file_path, sizeof(file_path), "%s/%s", path, file);
    if (stat(file_path, &st) == 0) {
        printf("File: %s, Size: %ld, Links: %ld\n", file_path, st.st_size, st.st_nlink);
    } else {
        printf("File: %s does not exist.\n", file_path);
    }
}

void find_all_hlinks(const char* source) {
    struct stat source_stat;
    char source_path[1024];
    snprintf(source_path, sizeof(source_path), "%s/%s", path, source);
    stat(source_path, &source_stat);
    DIR *dir = opendir(path);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        struct stat entry_stat;
        char entry_path[1024];
        snprintf(entry_path, sizeof(entry_path), "%s/%s", path, entry->d_name);
        stat(entry_path, &entry_stat);
        if (entry_stat.st_ino == source_stat.st_ino) {
            char *real_path = realpath(entry_path, NULL);
            printf("Inode: %ld, Path: %s\n", entry_stat.st_ino, real_path);

            FILE *file = fopen(real_path, "r");
            char c;
            printf("Content: ");
            while ((c = fgetc(file)) != EOF) {
                printf("%c", c);
            }
            printf("\n");
            fclose(file);

            free(real_path);
        }
    }
    closedir(dir);
}

void unlink_all(const char* source) {
    struct stat source_stat;
    char source_path[1024];
    snprintf(source_path, sizeof(source_path), "%s/%s", path, source);
    stat(source_path, &source_stat);
    DIR *dir = opendir(path);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        struct stat entry_stat;
        char entry_path[1024];
        snprintf(entry_path, sizeof(entry_path), "%s/%s", path, entry->d_name);
        stat(entry_path, &entry_stat);
        if (entry_stat.st_ino == source_stat.st_ino && strcmp(entry->d_name, source) != 0) {
            unlink(entry_path);
        }
    }
    closedir(dir);
}

void create_sym_link(const char* source, const char* link) {
    char source_path[1024], link_path[1024];
    snprintf(source_path, sizeof(source_path), "%s/%s", path, source);
    snprintf(link_path, sizeof(link_path), "%s/%s", path, link);
    symlink(source_path, link_path);
}

int main(int argc, char **argv) {
    path = argv[1];
    char file1[1024], file11[1024], file12[1024], file13[1024];
    snprintf(file1, sizeof(file1), "%s/myfile1.txt", path);
    snprintf(file11, sizeof(file11), "%s/myfile11.txt", path);
    snprintf(file12, sizeof(file12), "%s/myfile12.txt", path);
    snprintf(file13, sizeof(file13), "%s/myfile13.txt", path);
    FILE *file = fopen(file1, "w");
    fprintf(file, "Hello world.");
    fclose(file);
    link(file1, file11);
    link(file1, file12);
    find_all_hlinks("myfile1.txt");
    rename(file1, "/tmp/myfile1.txt");
    file = fopen(file11, "w");
    fprintf(file, "Hello world 2.0.");
    fclose(file);
    create_sym_link("/tmp/myfile1.txt", "myfile13.txt");
    file = fopen("/tmp/myfile1.txt", "w");
    fprintf(file, "Hello world 3.0.");
    fclose(file);
    unlink_all("myfile11.txt");
    print_stat("myfile11.txt");
    return 0;
}
