#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define TRUE 1
#define SLOTS 5


//Initiaze Globals
sem_t mutexOccupied;
sem_t mutexFree;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int slots[SLOTS];
int nrOfItems = 0;

void processArgs(unsigned* producers, unsigned* consumers, int argc, char* argv[]);

void rest(int duration);
void* consumer(void* arg);
void* producer(void* arg);
int produceItem();
void insertItem(int item);
int getItem();


int main(int argc, char* argv[])
{
    unsigned producers = 0;
    unsigned consumers = 0;
    processArgs(&producers, &consumers, argc, argv);
    srand((unsigned)time(0));
    sem_init(&mutexOccupied, 0, 0);
    sem_init(&mutexFree, 0, SLOTS);

    pthread_t* producerIDs = malloc(producers * sizeof(pthread_t));
    pthread_t* consumerIDs = malloc(consumers * sizeof(pthread_t));


    for (unsigned i = 0; i < producers; i++)
    {
        unsigned* p = malloc(sizeof(unsigned));
        *p = i;

        pthread_create(&producerIDs[i], NULL, producer, (void*)p);
    }
    for (unsigned i = 0; i < consumers; i++)
    {
        unsigned* p = malloc(sizeof(unsigned));
        *p = i;
        pthread_create(&consumerIDs[i], NULL, consumer, (void*)p);
    }

    rest(50);

    return 0;
}

void processArgs(unsigned* producers, unsigned* consumers, int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Usage: PRODUCERS CONSUMERS");
        exit(0);
    }
    *producers = atoi(argv[1]);
    *consumers = atoi(argv[2]);
}

void rest(int duration)
{
    time_t now = time(0);
    while(time(0) - now < duration);
}

void* consumer(void* arg)
{
    int item;
    while(TRUE)
    {
        rest(rand() % 5 + 1);
        sem_wait(&mutexOccupied);
            pthread_mutex_lock(&lock);
                item = getItem();
                printf("\t\t\t\tConsumer nr %i consumed item %i\n", *(int*)arg, item);
            pthread_mutex_unlock(&lock);
        sem_post(&mutexFree);
    }
}

void* producer(void* arg)
{
    int item;

    while(TRUE)
    {
        rest(rand() % 5 + 1);

        item = produceItem();
        sem_wait(&mutexFree);
            pthread_mutex_lock(&lock);
                insertItem(item);
                printf("Producer nr %i produced item %i\n", *(int*)arg, item);
            pthread_mutex_unlock(&lock);
        sem_post(&mutexOccupied);
    }
}

int produceItem()
{
    return rand() % 9000 + 1000;
}

void insertItem(int item)
{
    slots[nrOfItems++] = item;
}

int getItem()
{
    int item = slots[0];
    for (int i = 0; i < SLOTS - 1; i++)
    {
        slots[i] = slots[i+1];
    }
    slots[nrOfItems--] = 0;
    return item;
}
