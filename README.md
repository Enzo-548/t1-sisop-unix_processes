# t1-sisop-unix_processes

make compile


--exemplos
./main T1 <N> <TOTAL_INCREMENTS>   # threads sem sincronização
./main T2 <N> <TOTAL_INCREMENTS>   # threads com pthread_mutex
./main P1 <N> <TOTAL_INCREMENTS>   # processos + shmget/shmat sem sincronização
./main P2 <N> <TOTAL_INCREMENTS>   # processos + shmget/shmat + sem_open (semáforo)

--prontos
./main T1 8 50000000
./main T2 8 50000000
./main P1 8 50000000
./main P2 8 50000000

make run MODE=T2 N=4 TOTAL=10000000