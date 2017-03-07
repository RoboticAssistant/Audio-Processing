#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE_OF_HEADER 44

int main()
{
 char* wav_filename = "test.wav";
 char* raw_filename = "test.raw"; 
 char* pcm_filename = "test.csv"; 

 char* header;
 char* data;
 int   pcm_temp_data;
 char* pcm_data;

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

 FILE *ptr_pcm = fopen(pcm_filename,"w");

 if (ptr_pcm == NULL) 
 {
   printf("Error opening raw file\n");
	exit(1);
 }

 int counts = 0;
 while(read = fread(data, 2, 1, ptr))
 {
  counts ++;

  pcm_temp_data = ((int )(data[1]<<8) + data[0]); 

  if(counts % 2)
  {
   fwrite(data,2,1,ptr_raw);
   fprintf(ptr_pcm,"%d\n",pcm_temp_data);
   printf("%d\n",pcm_temp_data);  
  }
 }
 //printf("counts = %d    pcm_data = %d\n", counts,pcm_data);

 fclose(ptr_pcm);
 fclose(ptr_raw);
 fclose(ptr);

 free(header);

 return 0;
}