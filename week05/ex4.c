#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct prime_request {
    int a, b;
} prime_request;

bool is_prime(int n) {
    if (n <= 1) {
        return false;
    }
    int i = 2;
    for (; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int primes_count(int a, int b) {
    int ret = 0;
    for (int i = a; i < b; i++) {
        if (is_prime(i) != 0) {
            ret++;
        }
    }
    return ret;
}

int k = 0;
int c = 0;

void *prime_counter(void *arg) {
    prime_request req = *(prime_request *)arg;
    int count = primes_count(req.a, req.b);
    pthread_mutex_lock(&global_lock);
    c += count;
    k = req.b;
    pthread_mutex_unlock(&global_lock);
    int *count_ptr = malloc(sizeof(int));
    *count_ptr = count;
    return (void *)count_ptr;
}

int main(int argc, char **argv) {
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    pthread_t threads[m];
    prime_request requests[m];
    int counts[m];
    for (int i = 0; i < m; ++i) {
        requests[i].a = k;
        requests[i].b = k + n / m;
        k += n / m;
        pthread_create(&threads[i], NULL, prime_counter, &requests[i]);
    }
    for (int i = 0; i < m; ++i) {
        void *ret;
        pthread_join(threads[i], &ret);
        counts[i] = *(int *)ret;
        free(ret);
    }
    int total = 0;
    for (int i = 0; i < m; ++i) {
        total += counts[i];
    }
    printf("Total primes: %d\n", total);
    return 0;
}
