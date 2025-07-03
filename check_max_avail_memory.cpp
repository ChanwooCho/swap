#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STEP_MB 100

int main() {
    size_t step = STEP_MB * 1024 * 1024;
    size_t total = 0;

    while (1) {
        void *ptr = malloc(step);
        if (!ptr) {
            printf("malloc failed at total = %lu MB\n", total / (1024 * 1024));
            break;
        }
        memset(ptr, 0, step);  // 실제로 할당하게 하려면 필요
        total += step;
        printf("Allocated %lu MB\n", total / (1024 * 1024));
    }
    return 0;
}
