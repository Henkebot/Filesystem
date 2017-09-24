#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TRUE 1
#define BASKET_SIZE 5

int basket[BASKET_SIZE];
int nrOfContent = 0;

sem_t mutexSizeLeft;
sem_t mutexSizeNow;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void insert_item(int content)
{
	basket[nrOfContent++] = content;
}

int remove_item()
{
	if(nrOfContent == 0)
		return -1;

	int number = basket[0];

	for(int i = 0; i < nrOfContent - 1; i++)
		basket[i] = basket[i+1];

	basket[nrOfContent--] = 0;

	return number;

}

int produce_item()
{
	return (rand() % 9000) + 1000;
}


void processArgs(unsigned* producers, unsigned* consumers, int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Usage: PRODUCERS CONSUMERS\n");
		exit(0);
	}
	else 
	{
		*producers = atoi(argv[1]);
		*consumers = atoi(argv[2]);
	}

	
}

void* producerLoop(void* arg)
{
	int number = *(int*) arg;
	int item;

	while(TRUE)
	{
		sleep((rand() % 4) + 1);

		item = produce_item();

		sem_wait(&mutexSizeLeft);
			pthread_mutex_lock(&lock);	
				insert_item(item);
				printf("Producer nr %i produced item %i\n", number, item);
			pthread_mutex_unlock(&lock);
		sem_post(&mutexSizeNow);

								
	}
}

void* consumerLoop(void* arg)
{
	
	int number = *(int*) arg;
	int item;

	while(TRUE)
	{	
		sleep((rand() % 4) + 1);

		sem_wait(&mutexSizeNow);
			pthread_mutex_lock(&lock);
				item = remove_item();
				printf("\t\t\t\tConsumer nr %i consumed item %i\n", number, item);
			pthread_mutex_unlock(&lock);
		sem_post(&mutexSizeLeft);

	}
	
}

int main(int argc, char* argv[])
{
	
	unsigned producers = 0;
	unsigned consumers = 0;
	
	processArgs(&producers, &consumers, argc, argv);
	
	sem_init(&mutexSizeNow, 0, 0);
	sem_init(&mutexSizeLeft,0, BASKET_SIZE);
	
	// Thread Arrays	
	pthread_t* producerIDs = malloc(producers * sizeof(unsigned));
	pthread_t* consumerIDs = malloc(consumers * sizeof(unsigned));
	
	//Pre compute indexes
	int* pNumbers = malloc(	producers * sizeof(int));
	for(unsigned i = 0; i < producers;i++)
		pNumbers[i] = i;

	int* cNumbers = malloc( consumers * sizeof(int));
	for(unsigned i = 0; i < consumers;i++)
		cNumbers[i] = i;

	//Create Producers
	for(unsigned i = 0; i < producers; i++)
	{
		pthread_create(&producerIDs[i], NULL, producerLoop, (void*) &pNumbers[i]);	
	}
	//Create Consumers	
	for(unsigned i = 0; i < consumers; i++)
	{
		pthread_create(&consumerIDs[i],NULL,consumerLoop, (void*) &cNumbers[i]);
	}

	// Incase the program somehow completes
	for(unsigned i = 0; i < producers; i++)
		pthread_join(producerIDs[i],NULL);

	for(unsigned i = 0; i < consumers; i++)
		pthread_join(consumerIDs[i],NULL);
	
	sem_destroy(&mutexSizeLeft);		
	sem_destroy(&mutexSizeNow);

	return 0;
}
