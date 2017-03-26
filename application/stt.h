#include <pocketsphinx.h>
#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdlib.h>

int stt_init();
int stt_continuous(ad_rec_t *ad,int16 *adbuf,uint8 *utt_started,uint8 *in_speech,int32 *k);
char const *recognize(ad_rec_t *ad,int16 *adbuf,uint8 *utt_started,uint8 *in_speech,int32 *k);
