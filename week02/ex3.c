#include <stdio.h>

void convert(long long number, int sBase, int tBase) {
    if (sBase < 2 || sBase > 10 || tBase < 2 || tBase > 10) {
        printf("cannot convert!\n");
        return;
    }
    long long dec = 0;
    long long n = 1;
    while (number > 0) {
        int digit = (int) number % 10;
        if (digit >= sBase) {
            printf("cannot convert!\n");
            return;
        }
        dec += digit * n;
        n *= sBase;
        number /= 10;
    }
    char res[100];
    int index = 0;
    while (dec > 0) {
        int rem = (int) dec % tBase;
        res[index++] = (char) (rem + '0');
        dec /= tBase;
    }
    if (index == 0) {
        res[index++] = '0';
    }
    res[index] = '\0';
    for (int i = index - 1; i >= 0; --i) {
        printf("%c", res[i]);
    }
    printf("\n");
}

int main() {
    char input[200];
    long long number;
    int sBase, tBase;
    printf("Number: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input!\n");
        return 1;
    }
    printf("\n");
    if (sscanf(input, "%lld", &number) != 1) {
        printf("Invalid input!\n");
        return 1;
    }
    printf("Source base (2-10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input!\n");
        return 1;
    }
    printf("\n");
    if (sscanf(input, "%d", &sBase) != 1) {
        printf("Invalid input!\n");
        return 1;
    }
    printf("Target base (2-10): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input!\n");
        return 1;
    }
    printf("\n");
    if (sscanf(input, "%d", &tBase) != 1) {
        printf("Invalid input!\n");
        return 1;
    }
    convert(number, sBase, tBase);
    return 0;
}
