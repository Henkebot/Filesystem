#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int basket[5];
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

	while(1)
	{
		sleep((rand() % 4) + 1);
		item = produce_item();
		sem_post(&mutexSizeNow);
		pthread_mutex_lock(&lock);	
		insert_item(item);
		pthread_mutex_unlock(&lock);
		sem_wait(&mutexSizeLeft);
		printf("Producer nr %i produced item %i\n", number, item);
								
	}

	
}

void* consumerLoop(void* arg)
{
	int number = *(int*) arg;
	int item;
	while(1)
	{	
		sleep((rand() % 4) + 1);
		sem_wait(&mutexSizeNow);
		pthread_mutex_lock(&lock);
		item = remove_item();
		pthread_mutex_unlock(&lock);
		sem_post(&mutexSizeLeft);
		printf("\t\t\t\tConsumer nr %i consumed item %i\n", number, item);
	}
	
}

int main(int argc, char* argv[])
{
	unsigned producers = 0;
	unsigned consumers = 0;
	
	processArgs(&producers, &consumers, argc, argv);
	
	sem_init(&mutexSizeNow, 0, 0);
	sem_init(&mutexSizeLeft,0, 5);
	
	pthread_t* producerIDs = malloc(producers * sizeof(unsigned));
	pthread_t* consumerIDs = malloc(consumers * sizeof(unsigned));
	//Safe numbers
	int* pNumbers = malloc(	producers * sizeof(int));
	for(unsigned i = 0; i < producers;i++)
		pNumbers[i] = i;
	int* cNumbers = malloc( consumers * sizeof(int));
	for(unsigned i = 0; i < consumers;i++)
		cNumbers[i] = i;

	//Create Producers
	for(unsigned i = 0; i < producers; i++)
	{
		pthread_create(&producerIDs[i], NULL, producerLoop, (void*)& pNumbers[i]);	
	}
	
	for(unsigned i = 0; i < consumers; i++)
	{
		pthread_create(&consumerIDs[i],NULL,consumerLoop, (void*) &cNumbers[i]);
	}


	for(unsigned i = 0; i < producers; i++)
		pthread_join(producerIDs[i],NULL);

	for(unsigned i = 0; i < consumers; i++)
		pthread_join(consumerIDs[i],NULL);
	
	sem_destroy(&mutexSizeLeft);		
	sem_destroy(&mutexSizeNow);

	return 0;
}
