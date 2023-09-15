#include <stdio.h>

int tribonacci(int n) {
    int first = 0, second = 1, third = 1;
    if (n == 0) {
        return 0;
    } else if (n == 1 || n == 2) {
        return 1;
    }
    int current;
    for (int i = 3; i <= n; ++i) {
        current = first + second + third;
        first = second;
        second = third;
        third = current;
    }
    return current;
}

int main() {
    printf("%d\n", tribonacci(4));
    printf("%d", tribonacci(36));
    return 0;
}
