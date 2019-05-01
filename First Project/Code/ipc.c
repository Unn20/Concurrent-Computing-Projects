#include "ipc.h"

//semaphore operations
void P(int semid, int semnum)
    {
    sbuf.sem_num = semnum;
    sbuf.sem_op = -1;
    sbuf.sem_flg = 0;
    if (semop(semid, &sbuf, 1) == -1)
    {
        perror("P semaphore operation");
        exit(1);
    }
}

void V(int semid, int semnum)
{
    sbuf.sem_num = semnum;
    sbuf.sem_op = 1;
    sbuf.sem_flg = 0;
    if (semop(semid, &sbuf, 1) == -1)
    {
        perror("V semaphore operation");
        exit(1);
    }
}
