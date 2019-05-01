/*Maciej Leszczyk 136759
Informatyka I2, 3 semestr
Programowanie Wspolbiezne
Czytelnicy i pisarze*/

#include "entity.h"

//PROBABILITY TO BECOME A READER IN PERCENT <0,100>
#define READER_PERCENT_PROBABILITY 90

void draw(int *buf, unsigned char *charbuf1, int semid)
{
	int writers_count=0;
    printf("\033[H\033[J");
    printf("Ilosc procesow = %d\n", buf[0]);
    printf("Maksymalna ilosc ksiazek = %d\n", buf[1]);
	printf("Liczba dotychczasowych zakleszczen = %d\n", buf[8]);
    P(semid,5); printf("Ilosc ksiazek w bibliotece = %d\n", buf[2]); V(semid,5);
    P(semid,6); printf("Ilosc pisarzy  w bibliotece= %d\n", buf[3]);
    printf("Ilosc czytelnikow w bibliotece= %d\n", buf[4]); V(semid,6);
    printf("Procesy-czytelnicy :\n");
    printf("{");
	P(semid,7);
    for (int i=0; i<buf[0]; i++){
        if (charbuf1[i] == 0)
            printf("%d, ", i);
    }   printf("}\n");
    printf("Procesy-pisarze :\n");
    printf("{");
    for (int i=0; i<buf[0]; i++){
        if (charbuf1[i] == 1){
            printf("%d, ", i);
			writers_count++;
		}
    }   printf("}\n");
	V(semid,7);
/*
	printf("Wartosc semaforu 0: %d\n", semctl(semid, 0, GETVAL, 0));
	printf("Wartosc semaforu 1: %d\n", semctl(semid, 1, GETVAL, 0));
	printf("Wartosc semaforu 2: %d\n", semctl(semid, 2, GETVAL, 0));
	printf("Wartosc semaforu 3: %d\n", semctl(semid, 3, GETVAL, 0));
	printf("Wartosc semaforu 4: %d\n", semctl(semid, 4, GETVAL, 0));
	printf("Wartosc semaforu 5: %d\n", semctl(semid, 5, GETVAL, 0));
	printf("Wartosc semaforu 6: %d\n", semctl(semid, 6, GETVAL, 0));
	printf("Wartosc semaforu 7: %d\n", semctl(semid, 7, GETVAL, 0));
*/	
    usleep(100000);
	if ((writers_count == buf[0])&&((semctl(semid, 0, GETVAL, 0)==0))){
		printf("WYKRYTO ZAKLESZCZENIE!\n");
		buf[8]++;
		buf[7]=1;
		semctl(semid, 0, SETVAL, buf[0]);
		usleep(1000000);
		buf[7]=0;
	}
}



int main(int argc, char *argv[])
{

    //default parameters for task
    int N=5, K=10;
    if (argc>=2){
        N=atoi(argv[1]);
        if ((N>100) || (N<1)){
        printf("Wrong process input count! 0>N>100");
        exit(1);
        }
    }
    if (argc>=3){
        K=atoi(argv[2]);
        if ((K>160) || (K<1)){
        printf("Wrong maxbooks input count! 0>K>160");
        exit(1);
        }
    }
    int shmid,semid,msgid;
    int *buf;
    unsigned char *process_charbuf, *book_charbuf;

    shmid = shmget(IPC_PRIVATE, 9*sizeof(int), IPC_CREAT|0600);
    if (shmid == -1){
            perror("IPC int segment create");
            exit(1);
        }
   
    buf = (int*)shmat(shmid, NULL, 0);
    if (buf == NULL){
        perror("IPC int segment join");
        exit(1);
        }

    shmid = shmget(IPC_PRIVATE, N*sizeof(char), IPC_CREAT|0600);
    if (shmid == -1){
            perror("IPC char0 segment create");
            exit(1);
        }
    
    process_charbuf = (unsigned char*)shmat(shmid, NULL, 0);
    if (process_charbuf == NULL){
        perror("IPC char0 segment join");
        exit(1);
        }

    shmid = shmget(IPC_PRIVATE, K*sizeof(char), IPC_CREAT|0600);
    if (shmid == -1){
            perror("IPC char2 segment create");
            exit(1);
        }
    
    book_charbuf = (unsigned char*)shmat(shmid, NULL, 0);
    if (book_charbuf == NULL){
        perror("IPC char2 segment join");
        exit(1);
        }

    semid = semget(IPC_PRIVATE, 8, IPC_CREAT|0600); //6 binary semaphores
    if (semid == -1){
        perror("Semaphore tab");
        exit(1);
        }
    if (semctl(semid, 0, SETVAL, (int)K) == -1){	//empty library
        perror("Semaphore 0 initialization");
        exit(1);
        }
    if (semctl(semid, 1, SETVAL, (int)0) == -1){	//full library
        perror("Semaphore 1 initialization");
        exit(1);
        }
    if (semctl(semid, 2, SETVAL, (int)1) == -1){	//mutex
        perror("Semaphore 2 initialization");
        exit(1);
        }
    if (semctl(semid, 3, SETVAL, (int)1) == -1){	//srmutex
        perror("Semaphore 3 initialization");
        exit(1);
        }
    if (semctl(semid, 4, SETVAL, (int)1) == -1){	//wmutex
        perror("Semaphore 4 initialization");
        exit(1);
        }
	if (semctl(semid, 5, SETVAL, (int)1) == -1){	//buf[2]
        perror("Semaphore 5 initialization");
        exit(1);
        }
	if (semctl(semid, 6, SETVAL, (int)1) == -1){	//buf[3] and buf[4]
        perror("Semaphore 6 initialization");
        exit(1);
        }
	if (semctl(semid, 7, SETVAL, (int)1) == -1){	//process_charbuf
        perror("Semaphore 7 initialization");
        exit(1);
        }

    msgid = msgget(IPC_PRIVATE, IPC_CREAT|0600);
    if (msgid == -1){
        perror("Msg IPC create");
        exit(1);
        }
    buf[0] = N;
    buf[1] = K;
    buf[2] = 0;	//Book_count
    buf[3] = 0;	//Readers_count
    buf[4] = 0;	//Writers_count
    buf[5] = 0;	//User_ID
	buf[6] = 0;	//nreaders
	buf[7] = 0; //zakleszczenie	
	buf[8] = 0; //zakleszczenie

    for (int i=0;i<N;i++)
    {
        if (fork() == 0)
        {
            P(semid,5);
            int user_ID = buf[5]++;
            V(semid,5);
            srand(time(NULL)+user_ID);
            char function;    //0-reader, 1-writer
            while(1)
            {
                usleep((rand() % 5)*100000+(rand()%1000)*1000);
				if ( rand() % 100 < READER_PERCENT_PROBABILITY ){
					if (function==1){
						function=0;
						P(semid,7);
						process_charbuf[user_ID]=function;
						V(semid,7);
					}
				}
				else{
					if (function==0){
						function=1;
						P(semid,7);
						process_charbuf[user_ID]=function;
						V(semid,7);
					}
				}
				usleep((rand() % 4)*100000+(rand()%1000)*1000);
				
				if (function){							//WRITER
					P(semid,0); //if full books
					if (buf[7]==0){
					P(semid,4);	//wmutex
					P(semid,3);	//srmutex
					
					writer(buf,process_charbuf,book_charbuf,semid, msgid, user_ID);
					V(semid,3);
					V(semid,4);
					}
				}
				else{									//READER
					P(semid,5);
					if(buf[2]!=0){ V(semid,5);	//check if library is not empty
					P(semid,2);	//mutex
					if ((buf[6]) == 0){
						buf[6]++;
						P(semid,4);
					} else buf[6]++;
					V(semid,2);
					reader(buf,process_charbuf,book_charbuf,semid, msgid, user_ID);
					P(semid,2);
					buf[6]--;
					if (buf[6] == 0)
						V(semid,4);
					V(semid,2);
					P(semid,5);
					}V(semid,5);

				}
            }
            exit(0);
        }
    }

    while(1)
    {
        draw(buf, process_charbuf, semid);
    }

    return 0;
}
