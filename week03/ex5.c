#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <stdarg.h>

void* add(const void* a, const void* b) {
    if (a && b) {
        double sum = *(double*)a + *(double*)b;
        double* p = malloc(sizeof(double));
        *p = sum;
        return p;
    }
    return NULL;
}

void* multiply_int(const void* a, const void* b) {
    int product = *(int*)a * *(int*)b;
    int* p = malloc(sizeof(int));
    *p = product;
    return p;
}

void* multiply_double(const void* a, const void* b) {
    double product = *(double*)a * *(double*)b;
    double* p = malloc(sizeof(double));
    *p = product;
    return p;
}

void* max(const void* a, const void* b) {
    if (a && b) {
        double maximum = (*(double*)a > *(double*)b) ? *(double*)a : *(double*)b;
        double* p = malloc(sizeof(double));
        *p = maximum;
        return p;
    }
    return NULL;
}

void* vaggregate(size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*), ...) {
    va_list args;
    va_start(args, opr);

    void* result;
    for(int i = 0; i < n; i++) {
        void* ptr = va_arg(args, void*);
        result = opr(initial_value, ptr);
        initial_value = result;
    }

    va_end(args);
    return initial_value;
}

int main() {
    double a = 1.1, b = 2.2, c = 3.3, d = 4.4, e = 5.5;
    double initial_value0 = 0.0;
    double initial_value1 = 1.0;
    double initial_value2 = -DBL_MAX;
    void* result0 = vaggregate(sizeof(double), 5, &initial_value0, add, &a, &b, &c, &d, &e);
    printf("Sum of elements: %f\n", *(double*)result0);
    void* result1 = vaggregate(sizeof(double), 5, &initial_value1, multiply_double, &a, &b, &c, &d, &e);
    printf("Multiplication of elements: %f\n", *(double*)result1);
    void* result2 = vaggregate(sizeof(double), 5, &initial_value2, max, &a, &b, &c, &d, &e);
    printf("Max of elements: %f\n", *(double*)result2);
    return 0;
}

