#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <semaphore.h>

sem_t* create_semaphore();
void destroy_semaphore(sem_t* sem);

#endif
