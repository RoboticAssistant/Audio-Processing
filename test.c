#include <pocketsphinx.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE_OF_HEADER 44
char command[500];

void record_audio()
{
    system("./Record_from_DMIC.sh");
    system("arecord -Dhw:sndrpiwsp -r 16000 -c 2 -d 4 -f S16_LE test.wav");  
}

void dual_channel_wav_to_single_channel_raw()
{
 char* wav_filename = "test.wav";
 char* raw_filename = "test.raw"; 

 char* header;
 char* data;

 header = (char*) malloc(SIZE_OF_HEADER); 
 data = (char*) malloc(sizeof(char)*2); 

 FILE *ptr = fopen(wav_filename,"rb");
 if (ptr == NULL) 
 {
	printf("Error opening wav file\n");
	exit(1);
 }

  
 int read = 0;
 
 if(!(read = fread(header, SIZE_OF_HEADER, 1, ptr)))
  printf("File is empty..\n");

 //printf("(1-44): %s \n", header); 
 
 FILE *ptr_raw = fopen(raw_filename,"w");

 if (ptr_raw == NULL) 
 {
   printf("Error opening raw file\n");
	exit(1);
 }

 int counts = 0;
 while(read = fread(data, 2, 1, ptr))
 {
  counts ++;
  if(counts % 2)
   fwrite(data,2,1,ptr_raw);
 }

 fclose(ptr_raw);
 fclose(ptr);

 free(header);	

}

int speech_to_text()
{
    ps_decoder_t *ps;
    cmd_ln_t *config;
    FILE *fh;
    char const *hyp, *uttid;
    int16 buf[512];
    int rv;
    int32 score;

    config = cmd_ln_init(NULL, ps_args(), TRUE,
                 "-hmm", MODELDIR "/en-us/en-us",
                 "-lm", MODELDIR "/en-us/en-us.lm.bin",
                 "-dict", MODELDIR "/en-us/cmudict-en-us.dict",
                 NULL);
    if (config == NULL) {
        fprintf(stderr, "Failed to create config object, see log for details\n");
        return -1;
    }
    
    ps = ps_init(config);
    if (ps == NULL) {
        fprintf(stderr, "Failed to create recognizer, see log for details\n");
        return -1;
    }

    fh = fopen("test.raw", "rb");
    if (fh == NULL) {
        fprintf(stderr, "Unable to open input file goforward.raw\n");
        return -1;
    }

    rv = ps_start_utt(ps);
    
    while (!feof(fh)) {
        size_t nsamp;
        nsamp = fread(buf, 2, 512, fh);
        rv = ps_process_raw(ps, buf, nsamp, FALSE, FALSE);
    }
    
    rv = ps_end_utt(ps);
    hyp = ps_get_hyp(ps, &score);

    printf("Recognized: %s\n", hyp);
    sprintf(command,"pico2wave -w test.wav \"%s\"",hyp);

    fclose(fh);
    ps_free(ps);
    cmd_ln_free_r(config);
}

void single_channel_wav_to_dual_channel_raw()
{
 char* wav_filename = "test.wav";
 char* raw_filename = "test.raw"; 

 char* header;
 char* data;

 header = (char*) malloc(SIZE_OF_HEADER); 
 data = (char*) malloc(sizeof(char)*2); 

 FILE *ptr = fopen(wav_filename,"rb");
 if (ptr == NULL) 
 {
	printf("Error opening wav file\n");
	exit(1);
 }

  
 int read = 0;
 
 if(!(read = fread(header, SIZE_OF_HEADER, 1, ptr)))
  printf("File is empty..\n");

 //printf("(1-44): %s \n", header); 
 
 FILE *ptr_raw = fopen(raw_filename,"w");

 if (ptr_raw == NULL) 
 {
   printf("Error opening raw file\n");
	exit(1);
 }

 //int counts = 0;
 while(read = fread(data, 2, 1, ptr))
 {
   fwrite(data,2,1,ptr_raw);
   fwrite(data,2,1,ptr_raw);
 }

 fclose(ptr_raw);
 fclose(ptr);

 free(header);	

}

void text_to_speech()
{

 //char *text = "Hello";
 //sprintf(command,"pico2wave -w test.wav \"%s\"",text);

 system(command);
}

void play_audio()
{
 system("./Playback_to_Headset.sh");
 puts(command);
}

int main(int argc, char *argv[])
{
    record_audio();
    dual_channel_wav_to_single_channel_raw();
    speech_to_text();

    text_to_speech();
    single_channel_wav_to_dual_channel_raw();
    play_audio();

    return 0;
}