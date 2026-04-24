#include "semaphore.h"
#include <fcntl.h>

sem_t* create_semaphore() {
    return sem_open("/counter_sem", O_CREAT, 0644, 1);
}

void destroy_semaphore(sem_t* sem) {
    sem_close(sem);
    sem_unlink("/counter_sem");
}
