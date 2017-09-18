#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void *child()
{
	char* arg = malloc(sizeof(char));
	printf("Input to child: ");
	char c = getchar();
	*arg = c;
	printf("child now sleep\n");
	sleep(5);
	printf("child now awake\n");
	pthread_exit((void*)arg);
}

int main()
{
	pthread_t threadID;
	pthread_create(&threadID, NULL, child, NULL);
	printf("parent now sleep\n");
	sleep(5);
	printf("parent now awake\n");
	
	void* c = NULL;
	pthread_join(threadID, &c); //som wait()

	printf("%c\n",*(char*)c);

	free(c);

	return 0;
}
