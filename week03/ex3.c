#include <stdio.h>
#include <string.h>

#define MAX_NAME 63
#define MAX_PATH 2048
#define MAX_DATA 1024
#define MAX_FILES 256
#define MAX_DIRS 256

typedef struct File {
    int id;
    char name[MAX_NAME];
    int size;
    char data[MAX_DATA];
    struct Directory* directory;
} File;

typedef struct Directory {
    char name[MAX_NAME];
    struct File files[MAX_FILES];
    struct Directory* directories[MAX_DIRS];
    unsigned char nf;
    unsigned char nd;
    char path[MAX_PATH];
} Directory;

void overwrite_to_file(struct File* file, const char* str) {
    strncpy(file->data, str, MAX_DATA);
    file->size = strlen(file->data);
}

void append_to_file(struct File* file, const char* str) {
    strncat(file->data, str, MAX_DATA - file->size);
    file->size = strlen(file->data);
}

void printp_file(struct File* file) {
    printf("%s/%s\n", file->directory->path, file->name);
}

void add_file(struct File* file, struct Directory* dir) {
    if (dir->nf < MAX_FILES) {
        dir->files[dir->nf] = *file;
        file->directory = dir;
        dir->nf++;
    }
}

void add_dir(Directory* dir1, Directory* dir2) {
    if (dir1 && dir2) {
        dir2->directories[dir2->nd] = dir1;
        dir2->nd++;
        char temp_path[MAX_PATH];
        if (strcmp(dir2->path, "/")) {
            strcpy(temp_path, dir2->path);
            strcat(temp_path, "/");
            strcat(temp_path, dir1->name);
            strcpy(dir1->path, temp_path);
        } else {
            strcpy(temp_path, "/");
            strcat(temp_path, dir1->name);
            strcpy(dir1->path, temp_path);
        }
    }
}

// Prints the name of the File file
void show_file(File* file) {
    printf("%s ", file -> name);
}

// Displays the content of the Directory dir
void show_dir(Directory* dir) {
    printf("\nDIRECTORY\n");
    printf(" path: %s\n", dir->path);
    printf(" files:\n");
    printf(" [ ");
    for (int i = 0; i < dir->nf; i++) {
        show_file(&(dir->files[i]));
    }
    printf("]\n");
    printf(" directories:\n");
    printf(" { ");
    for (int i = 0; i < dir->nd; i++) {
        show_dir(dir->directories[i]);
    }
    printf("}\n");
}

int main() {
    Directory root;
    strcpy(root.name, "/");
    strcpy(root.path, "/");
    root.nf = 0;
    root.nd = 0;
    Directory home, bin;
    strcpy(home.name, "home");
    strcpy(bin.name, "bin");
    home.nf = 0;
    home.nd = 0;
    bin.nf = 0;
    bin.nd = 0;
    add_dir(&home, &root);
    add_dir(&bin, &root);
    File bash, ex3_1, ex3_2;
    bash.id = 1;
    strcpy(bash.name, "bash");
    ex3_1.id = 2;
    strcpy(ex3_1.name, "ex3_1.c");
    ex3_2.id = 3;
    strcpy(ex3_2.name, "ex3_2.c");
    overwrite_to_file(&bash, "Bourne Again Shell!!");
    overwrite_to_file(&ex3_1, "int printf(const char * format, ...);");
    overwrite_to_file(&ex3_2, "//This is a comment in C language");
    append_to_file(&ex3_1, "int main(){printf(\"Hello World!\")}");
    add_file(&bash, &bin);
    add_file(&ex3_1, &home);
    add_file(&ex3_2, &home);
    printp_file(&bash);
    printp_file(&ex3_1);
    printp_file(&ex3_2);
    show_dir(&root);
    return 0;
}
