#include "threads.h"
#include "../common/config.h"

#include <pthread.h>
#include <stdio.h>

static long long counter = 0;
static pthread_mutex_t lock;

typedef struct
{
    long long iterations;
    int use_mutex;
} thread_arg_t;

void *worker(void *arg)
{
    thread_arg_t *data = (thread_arg_t *)arg;

    for (long long i = 0; i < data->iterations; i++)
    {
        if (data->use_mutex)
        {
            pthread_mutex_lock(&lock);
            counter++;
            pthread_mutex_unlock(&lock);
        }
        else
        {
            counter++;
        }
    }
    return NULL;
}

long long run_threads(int n_threads, int use_mutex)
{
    printf("Running with %d threads and %s mutex...\n", n_threads, use_mutex ? "with" : "without");
    pthread_t threads[n_threads];
    thread_arg_t args[n_threads];

    counter = 0;
    pthread_mutex_init(&lock, NULL);

    long long iter = MAX_COUNT / n_threads;

    for (int i = 0; i < n_threads; i++)
    {
        args[i].iterations = iter;
        args[i].use_mutex = use_mutex;
        pthread_create(&threads[i], NULL, worker, &args[i]);
    }

    for (int i = 0; i < n_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    return counter;
}
