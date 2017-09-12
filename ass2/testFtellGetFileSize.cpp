#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

using namespace std;

int main () {
   FILE *fp;
   int len;

   fp = fopen("testFileSizeFile", "r");
   if( fp == NULL )  {
      perror ("Error opening file");
      return(-1);
   }
   fseek(fp, 0, SEEK_END);

   len = ftell(fp);

   fseek(fp, 0, SEEK_SET);
   //fclose(fp);
   char c;
   while((c = fgetc(fp)) != EOF){
   		printf("%c\n", c);
   }

   printf("Total size of file.txt = %d bytes\n", len);
  
   return(0);
}