#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_t t1,t2,t3;

void  *STT(void *arg)
{
	while(1)
	{
		printf("Hello1\n");
		sleep(1);
	}
}

void  *TTS(void *arg)
{
	while(1)
	{
		printf("Hello2\n");
		sleep(1);
	}
}

void  *PLOT(void *arg)
{
	while(1)
	{
		printf("Hello3\n");
		sleep(1);
	}
}

int main()
{
	pthread_create (&t1, NULL, &STT, NULL);
	pthread_create (&t2, NULL, &TTS, NULL);
	pthread_create (&t3, NULL, &PLOT, NULL);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);

	return 0;
}