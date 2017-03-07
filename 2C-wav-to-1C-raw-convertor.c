#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE_OF_HEADER 44

int main()
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
 printf("counts = %d\n", counts);

 //fclose(ptr_raw);
 fclose(ptr);

 free(header);

 return 0;
}