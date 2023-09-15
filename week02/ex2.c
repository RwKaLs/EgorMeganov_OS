#include <stdio.h>

int main() {
    char s[256];
    char temp;
    int len = 0;
    while (1) {
        temp = getchar();
        if (temp == '\n' || temp == '.') {
            break;
        }
        s[len++] = temp;
    }
    printf("\"");
    for (int i = len - 1; i >= 0; i--) {
        printf("%c", s[i]);
    }
    printf("\"\n");
    return 0;
}
