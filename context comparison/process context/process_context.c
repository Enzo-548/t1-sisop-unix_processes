#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stress_test.h>



//funcao vai forkear os processos e adicionar no corpo
int met1(pid_t childpid, Corpo proc_body){
    childpid = fork();
    if (childpid == -1){
        printerr("erro criando processo no programa atual");
        return 1;
    } else if (childpid == 0){
        printf("fork de processo foi um sucesso");
        putCorpo((int) childpid);
        return 0;
    } else {
        printf("esta no pid do pai");
        return 0;
    }
}

int processes_run(Corpo proc_body){
    printf("começando contagem com processos");
    pid_t childpid;
    
    childpid = fork();   
    
    if (childpid == -1) {

    perror("Failed to fork\n");
    return 1;
    
    } else if(childpid == 0){
        //Aumenta
    } else {
        //Printa resultados
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