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

int stt_continuous();
static void recognize();

static ps_decoder_t *ps;
static cmd_ln_t *config;
char const *hyp;