#include <stdio.h>

#include "threads/threads.h"
#include "processes/processes.h"

void start_timer();
double end_timer();

void run_all() {
    FILE* f = fopen("results.csv", "w");

    fprintf(f, "mode,workers,sync,time,final_counter\n");

    int Ns[] = {2, 4, 8};

    for (int i = 0; i < 3; i++) {
        int N = Ns[i];

        for (int sync = 0; sync <= 1; sync++) {
            start_timer();
            long long result = run_threads(N, sync);
            double time = end_timer();

            fprintf(f, "threads,%d,%d,%.6f,%lld\n", N, sync, time, result);
            fflush(f);
        }

        for (int sync = 0; sync <= 1; sync++) {
            start_timer();
            long long result = run_processes(N, sync);
            double time = end_timer();

            fprintf(f, "processes,%d,%d,%.6f,%lld\n", N, sync, time, result);
            fflush(f);
        }
    }

    fclose(f);
    printf("Resultados salvos em results.csv\n");
}

int main() {
    run_all();
    return 0;
}
