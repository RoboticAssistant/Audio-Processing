#include "tts.h"

void single_channel_wav_to_dual_channel_raw()
{
 char* wav_filename = (char*)"temp/test.wav";
 char* raw_filename = (char*)"temp/test.raw"; 

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

void make_audio(char* text)
{
	char command[200];
	sprintf(command,"pico2wave -w temp/test.wav \"%s\"",text);
	system(command);
	single_channel_wav_to_dual_channel_raw();
}

void play_audio()
{
	system("aplay -Dhw:sndrpiwsp -r 16000 -c 2 -f S16_LE temp/test.raw");
}