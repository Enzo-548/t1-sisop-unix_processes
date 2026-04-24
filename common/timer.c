#include <time.h>

static struct timespec start, end;

void start_timer() {
    clock_gettime(CLOCK_MONOTONIC, &start);
}

double end_timer() {
    clock_gettime(CLOCK_MONOTONIC, &end);

    return (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec) / 1e9;
}
