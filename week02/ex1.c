#include <stdio.h>
#include <limits.h>
#include <float.h>

int main() {
    int  max_int = INT_MAX;
    unsigned short max_us = USHRT_MAX;
    long int max_long = LONG_MAX;
    float max_float = FLT_MAX;
    double max_double = DBL_MAX;
    printf("int: %lu %d\n", sizeof(int), max_int);
    printf("unsigned short: %lu %u\n", sizeof(unsigned short), max_us);
    printf("long int: %lu %ld\n", sizeof(long int), max_long);
    printf("float: %lu %f\n", sizeof(float), max_float);
    printf("double: %lu %lf\n", sizeof(double), max_double);
}
