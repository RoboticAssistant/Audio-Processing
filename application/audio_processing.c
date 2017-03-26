#include <pthread.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include "tts.h"
#include "stt.h"

pthread_t t1,t2,t3,t4;
char *transmit, receive[200];


extern cmd_ln_t *config;
extern ps_decoder_t *ps;
extern char const *text_detected;

void  *STT(void *arg)
{
	ad_rec_t *ad;
	int16 adbuf[2048];
    	uint8 utt_started, in_speech;
    	int32 k;

/*******Initialize*******/	
	system("./Record_from_DMIC.sh");
    	utt_started = FALSE;
	stt_init();

	if ((ad = ad_open_dev("plughw:DEV=0,CARD=1", (int) cmd_ln_float32_r(config,"-samprate"))) == NULL)
        	E_FATAL("Failed to open audio device\n");

	if (ad_start_rec(ad) < 0)
        	E_FATAL("Failed to start recording\n");

    	if (ps_start_utt(ps) < 0)
        	E_FATAL("Failed to start utterance\n");	

/*******Run*******/
    	E_INFO("Ready....\n");
	while(1)
    	{		
		recognize(ad, adbuf, &utt_started, &in_speech, &k);
     	}

    	ad_close(ad);

    	ps_free(ps);
    	cmd_ln_free_r(config);

}

void  *TTS(void *arg)
{

/*******Initialize*******/
 	system("./Playback_to_Headset.sh");
	
	sprintf(receive,"Hello");

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

void  *PUBSUB(void *arg)
{
	while(1)
	{
		if(text_detected)
		{
			printf("pubsub: %s", text_detected);

			//publish to topics

			

			text_detected = NULL;
		}
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
	pthread_create (&t3, NULL, &PUBSUB, NULL);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	pthread_join(t4,NULL);

	return 0;
}