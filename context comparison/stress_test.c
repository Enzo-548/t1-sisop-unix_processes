#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
void aumenta(int *cont){
    (*cont)++;
}
int start(int counter){
    int cont = 0;
    pid_t childpid;
    childpid = fork();
        
    if (childpid == -1) { 
    perror("Failed to fork\n");
    return 1;
    } else if(childpid == 0){
        do {
        aumenta(&cont);
        } while(cont<counter);
    } else {
        printf("%i\n", cont);
    }
    
    
    return 0;
}