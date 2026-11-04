#include <stdio.h>

struct Point {
    float x;
    float y;
};

void displayMessage(int code) {
    switch (code) {
        case 1:
            printf("Status: OK\n");
            break;
        case 2:
            printf("Status: Warning\n");
            break;
        default:
            printf("Status: Unknown\n");
            break;
    }
}

int main() {
    int count = 5;
    double pi = 3.14159;
    char letter = 'A';

    if (count > 0) {
        while (count > 0) {
            count--;
        }
    } else {
        for (int i = 0; i < 2; i++) {
            displayMessage(i);
        }
    }

    return 0;
}