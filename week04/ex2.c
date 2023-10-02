#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void calculate_dot_product(int start, int end, const int *u, const int *v) {
    FILE *file = fopen("temp.txt", "a+");
    int sum = 0;
    for (int i = start; i < end; i++) {
        sum += u[i] * v[i];
    }
    fprintf(file, "%d\n", sum);
    fclose(file);
}

int main() {
    int u[120], v[120];
    for (int i = 0; i < 120; i++) {
        u[i] = rand() % 100;
        v[i] = rand() % 100;
    }
    int n;
    printf("Number of processes: ");
    scanf("%d", &n);
    int l = 120 / n;
    pid_t pid;
    for (int i = 0; i < n; i++) {
        pid = fork();
        if (pid == 0) {
            calculate_dot_product(i * l, (i + 1) * l, u, v);
            exit(0);
        }
    }
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }
    FILE *file = fopen("temp.txt", "r");
    int sum = 0, temp;
    while (fscanf(file, "%d", &temp) != EOF) {
        sum += temp;
    }
    fclose(file);
    remove("temp.txt");
    printf("Dot product is: %d\n", sum);
    return 0;
}
