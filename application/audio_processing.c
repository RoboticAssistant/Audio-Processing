#include <pthread.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include "tts.h"

pthread_t t1,t2,t3;
char *transmit, receive[200];

void  *STT(void *arg)
{

/*******Initialize*******/	
	system("./Record_from_Headset.sh");

/*******Run*******/
	//system("pocketsphinx_continuous -inmic yes -adcdev plughw:DEV=0,CARD=1");
	stt_continuous();
}

void  *TTS(void *arg)
{

/*******Initialize*******/
 	system("./Playback_to_Headset.sh");
	
	sprintf(receive,"Hi! I am Tubby");

/*******Run*******/
	int i;	
	while(1)
	{
		if(*receive)
		{
			make_audio(receive);
			play_audio();
			sprintf(receive,"");	
		}
		else
		{
			scanf("%d",&i);
			sprintf(receive,"%d",i);
		}

		sleep(1);
	}
}

void  *PLOT(void *arg)
{
	while(1)
	{
		//printf("Hello3\n");
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