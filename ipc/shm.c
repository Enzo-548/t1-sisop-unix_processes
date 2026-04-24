#include "shm.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int create_shm() {
    return shmget(IPC_PRIVATE, sizeof(shared_data_t), IPC_CREAT | 0666);
}

shared_data_t* attach_shm(int shmid) {
    return (shared_data_t*) shmat(shmid, NULL, 0);
}

void destroy_shm(int shmid, shared_data_t* ptr) {
    shmdt(ptr);
    shmctl(shmid, IPC_RMID, NULL);
}
