#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE_OF_HEADER 44

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

void get_text()
{
 char* text = "Hello World!";
 char command[200];

 sprintf(command,"pico2wave -w test.wav \"%s\"",text);
 puts(command);
 system(command);
}

void play_audio()
{
 system("./Playback_to_Headset.sh");
}

int main()
{
 get_text();
 single_channel_wav_to_dual_channel_raw();
 play_audio();
 return 0;
}
