#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>

	/*---INICJALIZACJA MUTEXOW I ZMIENNYCH WARUNKOWYCH---*/
pthread_mutex_t mutex_mikolaj=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_renifer=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_skrzat=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_mikolaj=PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_renifer=PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_skrzat=PTHREAD_COND_INITIALIZER;

int liczba_reniferow = 0, liczba_skrzatow = 0;

	/*---WATEK MIKOLAJA---*/
void *Mikolaj()
{
	char spiacy = 0;

	while(1)
	{
		if (spiacy)
			printf("Mikolaj budzi sie... \n");
		spiacy = 1;

		/*---SEKCJA KRYTYCZNA RENIFEROW---*/
		pthread_mutex_lock(&mutex_renifer);
		if (liczba_reniferow == 9)
		{
			spiacy = 0;
			printf("Mikolaj wyrusza aby rozdac prezenty!\n");
			sleep(3);
			printf("MIKOLAJ ROZDAL PREZENTY! Renifery zostaja odwolane.\n\n");
			liczba_reniferow = 0;
			pthread_cond_broadcast(&cond_renifer);
		}
		pthread_mutex_unlock(&mutex_renifer);
		//...

		/*---SEKCJA KRYTYCZNA SKRZATOW---*/
		pthread_mutex_lock(&mutex_skrzat);
		if (liczba_skrzatow >= 3)
		{
			spiacy = 0;
			printf("Mikolaj bedzie udzielal konsultacji skrzatom.\n");
			sleep(2);
			printf("ZADOWOLONE SKRZATY opuszczaja chate Mikolaja.\n\n");
			liczba_skrzatow = 0;
			pthread_cond_broadcast(&cond_skrzat);
		}
		pthread_mutex_unlock(&mutex_skrzat);
		//...

		if (spiacy){
			printf("Mikolaj pograza sie we snie...\n");
			pthread_cond_wait(&cond_mikolaj, &mutex_mikolaj);
		}
	}
	return NULL;
}
//...

/*---WATEK RENIFERA---*/
void *Renifer(void *numer)
{
	srand(time(NULL) + (int)numer);
	while(1)
	{
		sleep((rand() % 12)+2);
		printf("\t\tRenifer nr #%d jest gotowy do podrozy\n", (int)numer);
		pthread_mutex_lock(&mutex_renifer);
		liczba_reniferow++;
		if (liczba_reniferow == 9){
			printf("\tDziewiaty renifer chce zbudzic Mikolaja\n");
			pthread_cond_signal(&cond_mikolaj);
		}
		pthread_cond_wait(&cond_renifer, &mutex_renifer);
		pthread_mutex_unlock(&mutex_renifer);
	}
	return NULL;
}
//...

/*---WATEK SKRZATA---*/
void *Skrzat(void *numer)
{
	srand(time(NULL) + (int)numer);
	while(1)
	{
		sleep((rand() % 12)+3);
		printf("\t\tSkrzat numer #%d domaga sie konsultacji.\n", (int)numer);
		pthread_mutex_lock(&mutex_skrzat);
		liczba_skrzatow++;
		if (liczba_skrzatow >=3){
			printf("\tSkrzaty chca obudzic Mikolaja!\n");
			pthread_cond_signal(&cond_mikolaj);
		}
		pthread_cond_wait(&cond_skrzat, &mutex_skrzat);
		pthread_mutex_unlock(&mutex_skrzat);
	}
	return NULL;
}
//...

int main()
{
	printf("***POCZATEK PROGRAMU***\n");
	pthread_t skrzat_id[10], renifer_id[9], mikolaj;

	int i;
	if (pthread_create(&mikolaj, NULL, Mikolaj, NULL)){
		perror("Blad przy tworzeniu Mikolaja");
		exit(-1);
	}
	for (i=0;i<9;i++)
	{
		if (pthread_create(&renifer_id[i], NULL, Renifer, (void*)(i+1))){
			perror("Blad przy tworzeniu renifera");
			exit(-1);
		}
	}
	for (i=0;i<10;i++)
	{
		if (pthread_create(&skrzat_id[i], NULL, Skrzat, (void*)(i+1))){
			perror("Blad przy tworzeniu skrzata");
			exit(-1);
		}
	}

	//program nigdy sie nie skonczy
	(void)pthread_join(mikolaj, NULL);
	printf("***KONIEC PROGRAMU***\n");
	return EXIT_SUCCESS;
}
