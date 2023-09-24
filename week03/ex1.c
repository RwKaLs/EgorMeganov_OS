#include <stdio.h>
#include <stdlib.h>

int const_tri(int* p, int n) {
    if (n < 0) {
        return -1;
    } else if (n == 0) {
        return 0;
    } else if (n < 3) {
        return 1;
    }
    int i;
    int temp;
    for (i = 3; i < n; i++) {
        temp = p[0] + p[1] + p[2];
        p[0] = p[1];
        p[1] = p[2];
        p[2] = temp;
    }
    return p[2];
}

// counting numbers starts from 0 ([0] = 0, [1] = 1, [2] = 1, [3] = 2, [4] = 4)
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
    printf("Tribonacci number [5]: %d\n", const_tri(p, 5));
    free(p);
    return 0;
}
