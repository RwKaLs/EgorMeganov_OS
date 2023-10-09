#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

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

typedef struct prime_request {
    int a, b;
} prime_request;

void *prime_counter(void *arg) {
    prime_request req = *(prime_request *)arg;
    int *count = malloc(sizeof(int));
    *count = primes_count(req.a, req.b);
    return ((void *)count);
}

int main(int argc, char **argv) {
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    pthread_t threads[m];
    prime_request requests[m];
    int counts[m];
    for (int i = 0; i < m; ++i) {
        requests[i].a = i * n / m;
        requests[i].b = (i + 1) * n / m;
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
