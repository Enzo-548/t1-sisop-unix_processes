#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define BILHAO 1000000000

void exit_err(const char* msg){
    perror(msg);
    exit(1);
}

int main(){
    // int n = 0;
    // char *modo;

    // printf("Escolha o modo de execução.\n");
    // printf("Ex: <T1/T2/P1/P2> <N>\n");
    // scanf("%s", modo);
    // scanf("%d", n);

    // if(!strcmp(modo, "P1")){
    //     printf("if");
    //     processos(n);
    // }
    processos(8);
}


void processos(int n){
    printf("iniciei\n");
    //cria a memória compartilhada. private (só esse programa e os forks enxergam).
    int shmid = shmget(IPC_PRIVATE, sizeof(long), IPC_CREAT | 0600);
    if(shmid == -1){
        exit_err("erro ao compartilhar memória");
    }

    //linka a memória ao ponteiro
    long *contador = (long*)shmat(shmid, NULL, 0);
    long max = BILHAO/n;

    pid_t *pids = malloc(n * sizeof(pid_t));

    for(int i = 0; i < n; i++){
        pid_t pid = fork();
        if(pid < 0){
            exit_err("erro no fork");
        }
        else if(pid == 0){//filho
            for(long j = 0; j<max; j++){
                // () pra não mexer no ponteiro
                (*contador)++;
            }
            shmdt(contador);
            exit(0);
        }

        pids[i] = pid;
    }

    //aguardando pelos filhos
    for(int i = 0; i<n; i++){
        int status;
        if(waitpid(pids[i], &status, 0) < 0){
            exit_err("filho não retornou");
        }
    }

    printf("Esperado:           %010ld\n", BILHAO);
    printf("Soma total com P1:  %010ld\n", *contador);
    printf("N = %d\n", n);

    //liberando a memória
    shmdt(contador);
    shmctl(shmid, IPC_RMID, NULL);
    free(pids);
}