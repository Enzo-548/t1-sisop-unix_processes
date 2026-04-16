#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main(void)
{
pid_t childpid;
childpid = fork();
if (childpid == -1) { /* error */
perror("Failed to fork\n");
return 1;
}
if (childpid == 0) { /* child code */
printf("I am child %d\n", getpid());
} else { /* parent code */
printf("I am parent %d\n", getpid());
}
return 0;
}

//body de processos ?
/**
* struct process_body(){
int* pid_t[];
int execution_time[];
int cont;
}
*/

// a quantidade de processos nn eh fixa
void process_run(int qtd){
    //caso1
    //process_body body1;
    //caso2
    //process_body body2;
    //caso 3
    //process_body body3;
}


int processes_run(){
    //instancia o processo
    //forqueia uma quantidade de vezes
    //comeca o contador
}