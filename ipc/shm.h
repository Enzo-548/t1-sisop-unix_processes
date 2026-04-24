#ifndef SHM_H
#define SHM_H

typedef struct {
    long long counter;
} shared_data_t;

int create_shm();
shared_data_t* attach_shm(int shmid);
void destroy_shm(int shmid, shared_data_t* ptr);

#endif
