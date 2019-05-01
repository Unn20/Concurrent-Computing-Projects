#include "entity.h"

static struct bookEnt
{
    long mtype;
    char process[100];
} book;

void writer(int *buf,unsigned char *process_charbuf, unsigned char *book_charbuf, int semid, int msgid, int user_id)
{
	P(semid,6); buf[3]++; V(semid,6);
	
	//read
	char send = 0;
	P(semid,5);
	if (buf[2]!=0){
	int counter, i, readNo;	
	counter = rand() % buf[2];
	i=-1;
	while(counter>=0){
	i = (i+1) % buf[2];
		if (book_charbuf[i] == 1);
			counter--;
	}
	V(semid,5);
	readNo = i;
	P(semid,1);
	
	msgrcv(msgid,&book,100*sizeof(char),readNo+1,IPC_NOWAIT);
	
	book.process[user_id]=0;
	for (int i=0;i<buf[1];i++){
		if (book.process[i]==1){
			if(msgsnd(msgid, &book, 100*sizeof(char), 0) == -1){
                perror("Book input");
                exit(1);
            }
			V(semid,1);
			send = 1;
			break;
		}
	}
	if (send == 0){
		V(semid,0);
		P(semid,5);
		buf[2]--;
		book_charbuf[readNo]=0;
		V(semid,5);
	}

	P(semid,5);
	}
	V(semid,5);

	//write
	int writeNo;
	for (int i=0;i<buf[1];i++)
		if (book_charbuf[i]==0){
			writeNo = i;
			break;
		}
	book.mtype = writeNo + 1;
	P(semid,7);
	for (int i=0;i<buf[0];i++){
		if (process_charbuf[i]==0)
			book.process[i] = 1;
		else
			book.process[i] = 0;
	}
	V(semid,7);
	
	if (msgsnd(msgid, &book, 100*sizeof(char), 0) == -1){
    	perror("Book input [sending msg]");
    	exit(1);
    }
	book_charbuf[writeNo]=1;
	V(semid,1);

	P(semid,5); buf[2]++; V(semid,5);
	usleep((rand() % 5)*30000+(rand()%1000)*1000);
	P(semid,6); buf[3]--; V(semid,6);
}

void reader(int *buf,unsigned char *process_charbuf, unsigned char *book_charbuf, int semid, int msgid, int user_id)
{
	P(semid,6); buf[4]++; V(semid,6);

	//read
	char send = 0;
	P(semid,5);
	if (buf[2]!=0){
	int counter, i, readNo;	
	counter = rand() % buf[2];
	i=-1;
	while(counter>=0){
	i = (i+1) % buf[2];
		if (book_charbuf[i] == 1);
			counter--;
	}
	V(semid,5);
	readNo = i;
	P(semid,1);
	
	msgrcv(msgid,&book,100*sizeof(char),readNo+1,IPC_NOWAIT);

	book.process[user_id]=0;
	for (int i=0;i<buf[1];i++){
		if (book.process[i]==1){
			if(msgsnd(msgid, &book, 100*sizeof(char), 0) == -1){
                perror("Book input");
                exit(1);
            }
			V(semid,1);
			send = 1;
			break;
		}
	}
	if (send == 0){
		V(semid,0);
		P(semid,5);
		buf[2]--;
		book_charbuf[readNo]=0;
		V(semid,5);
	}

	P(semid,5);
	}
	V(semid,5);

	usleep((rand() % 5)*30000+(rand()%1000)*1000);

	P(semid,6); buf[4]--; V(semid,6);
}
