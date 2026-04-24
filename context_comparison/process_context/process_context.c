#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stress_test.h>

Corpo body = {{},{},0};

//funcao vai forkear os processos e adicionar no corpo
int fork_with_check(Corpo proc_body){
    pid_t childpid = fork();
    if (childpid == -1){
        printf("erro criando processo no programa atual\n");
        return 1;
    } else if (childpid == 0){
        printf("estou no pid do filho\n");
        return 0;
    } else {
        printf("esta no pid do pai\n");
        return 0;
    }
    printf("###ERRO###\n");
    return 1;
}

int processes_run(){
    printf("\ncomeçando contagem com processos\n");
    fork_with_check(body);   
    int i = 0;
    while (body.instances[i] != NULL)
    {
        
        printf("Id criança de processos: %i", *body.instances[i]);
        i++;
    }
    
    
    return 0;
}

int no_synchronization_mode(){
    return 0;
}

//Com semaforo
int with_synchronization_mode(){
    return 0;
}