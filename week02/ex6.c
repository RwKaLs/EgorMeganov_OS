#include <stdio.h>

void drawFirst(int n) {
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) {
            printf("*");
        }
        printf("\n");
    }
}

void drawSecond(int n) {
    for (int i = 1; i <= n; ++i) {
        if (i <= n/2+1) {
            for (int j = 0; j < i; ++j) {
                printf("*");
            }
        } else {
            for (int j = 1; j <= n-i+1; ++j) {
                printf("*");
            }
        }
        printf("\n");
    }
}

void drawThird(int n) {
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            printf("*");
        }
        printf("\n");
    }
}

int main() {
    drawFirst(7);
    drawSecond(7);
    drawThird(7);
    return 0;
}
