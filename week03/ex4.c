#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

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

void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*)) {
    void* ptr = base;
    for(int i = 0; i < n; i++) {
        initial_value = opr(initial_value, ptr);
        ptr += size;
    }
    return initial_value;
}

int main() {
    double arr1[5] = {1.1, 2.2, 3.3, 4.4, 5.5};
    int arr2[5] = {1, 2, 3, 4, 5};
    double initial_value01 = 0.0;
    int initial_value02 = 0;
    double initial_value11 = 1.0;
    int initial_value12 = 1;
    double initial_value21 = -DBL_MAX;
    int initial_value22 = INT_MIN;

    void* result1 = aggregate(arr1, sizeof(arr1[0]), 5, &initial_value01, add);
    printf("Sum of elements in the 1st array: %f\n", *(double*)result1);
    void* result2 = aggregate(arr1, sizeof(arr1[0]), 5, &initial_value11, multiply_double);
    printf("Multiplication of elements in the 1st array: %f\n", *(double*)result2);
    void* result3 = aggregate(arr1, sizeof(arr1[0]), 5, &initial_value21, max);
    printf("Max of elements in the 1st array: %f\n", *(double*)result3);

    printf("\n");

    void* result4 = aggregate(arr2, sizeof(arr2[0]), 5, &initial_value02, add);
    printf("Sum of elements in the 2nd array: %d\n", *(int*)result4);
    void* result5 = aggregate(arr2, sizeof(arr2[0]), 5, &initial_value12, multiply_int);
    printf("Multiplication of elements in the 2nd array: %d\n", *(int*)result5);
    void* result6 = aggregate(arr2, sizeof(arr2[0]), 5, &initial_value22, max);
    printf("Max of elements in the 2nd array: %d\n", *(int*)result6);
    return 0;
}
