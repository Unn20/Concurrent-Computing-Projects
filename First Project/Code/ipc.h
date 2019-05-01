#ifndef IPC_H
#define IPC_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>

struct sembuf sbuf;
void P(int semid, int semnum);
void V(int semid, int semnum);

#endif
