#include <stdio.h>
#include <stdlib.h>
#include "strings.h"
#include "scanner.h"
#define FILE_ERROR 5



int main(int argc, char **argv)
{
   int a;

FILE *f;
   
   f=fopen(argv[1],"r");
   TString myToken;
   stringInit(&myToken);
   do {
      a = getNextToken(&myToken, f);
      printf("number: %d  \t----length: %d\t----buffer: %s   \n", a, myToken.length, myToken.myString);

   } while (a != 121);

   stringFree(&myToken);
   fclose(f);

return 0;
}
