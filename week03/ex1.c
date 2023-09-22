#include <stdio.h>
#include <stdlib.h>

int const_tri(int* p, int n) {
    int i;
    for (i = 3; i <= n; i++) {
        int temp = p[0] + p[1] + p[2];
        p[0] = p[1];
        p[1] = p[2];
        p[2] = temp;
    }
    return p[2];
}

int main() {
    const int x = 1;
    const int* q = &x;
    int* p = (int*)malloc(3 * sizeof(int));
    p[0] = *q;
    p[1] = *q;
    p[2] = 2 * (*q);
    printf("First cell: %p\n", &p[0]);
    printf("Second cell: %p\n", &p[1]);
    printf("Third cell: %p\n", &p[2]);
    printf("Tribonacci number for n=7: %d\n", const_tri(p, 7));
    free(p);
    return 0;
}
