#include <stdio.h>
#include <math.h>

typedef struct Point {
    double x, y;
} Point;

double distance(Point a, Point b) {
    return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

double area(Point A, Point B, Point C) {
    return 0.5 * (A.x * B.y - B.x * A.y + B.x * C.y - C.x * B.y + C.x * A.y - A.x * C.y);
}

int main() {
    Point A, B, C;
    A.x = 2.5, A.y = 6;
    B.x = 1, B.y = 2.2;
    C.x = 10, C.y = 6;
    printf("%f\n", area(A, B, C));
    printf("%f\n", distance(A, B));
    return 0;
}
