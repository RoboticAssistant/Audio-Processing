#include "stt.h"

cmd_ln_t *config;
ps_decoder_t *ps;
char const *text_detected;

/* Sleep for specified msec */
static void sleep_msec(int32 ms)
{
    struct timeval tmo;

    tmo.tv_sec = 0;
    tmo.tv_usec = ms * 1000;

    select(0, NULL, NULL, NULL, &tmo);
}

int stt_init()
{
    config = cmd_ln_init(NULL, ps_args(), TRUE,
                 "-hmm", MODELDIR "/en-us/en-us",
                 "-lm", MODELDIR "/en-us/en-us.lm.bin",
                 "-dict", MODELDIR "/en-us/cmudict-en-us.dict",
                 NULL);
    if (config == NULL) {
        fprintf(stderr, "Failed to create config object, see log for details\n");
        return -1;
    }

    ps_default_search_args(config);

    ps = ps_init(config);
    if (ps == NULL) {
        fprintf(stderr, "Failed to create recognizer, see log for details\n");
        return -1;
    }	

}

char const *recognize(ad_rec_t *ad,int16 *adbuf,uint8 *utt_started,uint8 *in_speech,int32 *k)
{
	char const *hyp;

        if ((*k = ad_read(ad, adbuf, 2048)) < 0)
            E_FATAL("Failed to read audio\n");

        ps_process_raw(ps, adbuf, *k, FALSE, FALSE);
        *in_speech = ps_get_in_speech(ps);

        if (*in_speech && !*utt_started) {
            *utt_started = TRUE;
            E_INFO("Listening...\n");
        }

        if (!*in_speech && *utt_started) {
            /* speech -> silence transition, time to start new utterance  */

            ps_end_utt(ps);
            hyp = ps_get_hyp(ps, NULL );
            if (hyp != NULL) {
                //printf("%s\n", hyp);
		text_detected = hyp;
                fflush(stdout);
            }

            if (ps_start_utt(ps) < 0)
                E_FATAL("Failed to start utterance\n");
            *utt_started = FALSE;
            E_INFO("Ready....\n");
        }
        sleep_msec(100);

}