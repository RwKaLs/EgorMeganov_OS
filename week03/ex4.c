#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

// We cannot use type conversion int -> double
// because of the different size we'll obtain 0 after conversion
void* add(const void* a, const void* b, size_t size) {
    if (size == sizeof(int)) {
        int sum = *(int*)a + *(int*)b;
        int* p = malloc(sizeof(int));
        *p = sum;
        return p;
    } else {
        double sum = *(double*)a + *(double*)b;
        double* p = malloc(sizeof(double));
        *p = sum;
        return p;
    }
}

void* multiply(const void* a, const void* b, size_t size) {
    if (size == sizeof(int)) {
        int product = *(int*)a * *(int*)b;
        int* p = malloc(sizeof(int));
        *p = product;
        return p;
    } else {
        double product = *(double*)a * *(double*)b;
        double* p = malloc(sizeof(double));
        *p = product;
        return p;
    }
}

void* max(const void* a, const void* b, size_t size) {
    if (size == sizeof(int)) {
        int maximum = (*(int*)a > *(int*)b) ? *(int*)a : *(int*)b;
        int* p = malloc(sizeof(int));
        *p = maximum;
        return p;
    } else {
        double maximum = (*(double*)a > *(double*)b) ? *(double*)a : *(double*)b;
        double* p = malloc(sizeof(double));
        *p = maximum;
        return p;
    }
}

void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*, size_t)) {
    void* ptr = base;
    void* result;
    for(int i = 0; i < n; i++) {
        result = opr(initial_value, ptr, size);
        initial_value = result;
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
    void* result2 = aggregate(arr1, sizeof(arr1[0]), 5, &initial_value11, multiply);
    printf("Multiplication of elements in the 1st array: %f\n", *(double*)result2);
    void* result3 = aggregate(arr1, sizeof(arr1[0]), 5, &initial_value21, max);
    printf("Max of elements in the 1st array: %f\n", *(double*)result3);

    printf("\n");

    void* result4 = aggregate(arr2, sizeof(arr2[0]), 5, &initial_value02, add);
    printf("Sum of elements in the 2nd array: %d\n", *(int*)result4);
    void* result5 = aggregate(arr2, sizeof(arr2[0]), 5, &initial_value12, multiply);
    printf("Multiplication of elements in the 2nd array: %d\n", *(int*)result5);
    void* result6 = aggregate(arr2, sizeof(arr2[0]), 5, &initial_value22, max);
    printf("Max of elements in the 2nd array: %d\n", *(int*)result6);
    return 0;
}
