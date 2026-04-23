//header init
#include <sys/types.h>
#include <time.h>
#include <linux/time.h>
#define MAX 4


//metodo que recebe o endereço do contador e o aumenta
//deve ser usado de forma local
static inline void aumenta(int *count){
    *(count)++;
}

//cronometro
static inline double now() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

//um unico corpo por exprerimentacao
typedef struct{
int* instances[MAX];
int execution_time[MAX];
int inst_cont;
}Corpo;

static inline Corpo new(){
    Corpo a = {{},{},0};
    return a;
}

//static inline int putCorpo(int *pid){}

int start();

