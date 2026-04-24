#include "processes.h"
#include "../common/config.h"
#include "../ipc/shm.h"
#include "../ipc/semaphore.h"

#include <unistd.h>
#include <sys/wait.h>

long long run_processes(int n_processes, int use_semaphore) {
    int shmid = create_shm();
    shared_data_t* shared = attach_shm(shmid);

    shared->counter = 0;

    sem_t* sem = NULL;
    if (use_semaphore) {
        sem = create_semaphore();
    }

    long long iter = MAX_COUNT / n_processes;

    for (int i = 0; i < n_processes; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            for (long long j = 0; j < iter; j++) {
                if (use_semaphore) {
                    sem_wait(sem);
                    shared->counter++;
                    sem_post(sem);
                } else {
                    shared->counter++;
                }
            }
            _exit(0);
        }
    }

    for (int i = 0; i < n_processes; i++) {
        wait(NULL);
    }

    long long result = shared->counter;

    if (use_semaphore) destroy_semaphore(sem);
    destroy_shm(shmid, shared);

    return result;
}
