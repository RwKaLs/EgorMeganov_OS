#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    pthread_t id;
    int i;
    char message[256];
} Thread;

void* print_message(void* arg) {
    Thread* thread = (Thread*) arg;
    printf("Thread id: %lu. %s\n", thread->id, thread->message);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    Thread* threads = malloc(n * sizeof(Thread));
    for (int i = 0; i < n; i++) {
        threads[i].i = i;
        sprintf(threads[i].message, "Hello from thread %d", i);
        printf("Thread %d is created\n", i);
        pthread_create(&threads[i].id, NULL, print_message, &threads[i]);
        pthread_join(threads[i].id, NULL);
    }
    free(threads);
    return 0;
}
