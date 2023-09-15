#include <stdio.h>
#include <string.h>
#include <ctype.h>

int count(const char *s, char symbol) {
    int count = 0;
    int len = strlen(s);
    for (int i = 0; i < len; ++i) {
        if (tolower(s[i]) == tolower(symbol)) {
            ++count;
        }
    }
    return count;
}

void countAll(const char *s) {
    int len = strlen(s);
    for (int i = 0; i < len; ++i) {
        char currentChar = s[i];
        if (isalpha(currentChar)) {
            int charCount = count(s, currentChar);
            printf("%c:%d, ", tolower(currentChar), charCount);
            while (i < len && tolower(s[i]) == tolower(currentChar)) {
                ++i;
            }
            --i;
        }
    }
    printf("\n");
}

int main() {
    char input[256];
    scanf("%s", input);
    countAll(input);
    return 0;
}
