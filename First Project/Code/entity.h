#ifndef ENTITY_H
#define ENTITY_H

#include "ipc.h"
#include <time.h>

void writer(int *buf,unsigned char *process_charbuf, unsigned char *book_charbuf, int semid, int msgid, int user_id);

void reader(int *buf,unsigned char *process_charbuf, unsigned char *book_charbuf, int semid, int msgid, int user_id);

#endif

