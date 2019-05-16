#include <stdio.h>
#include <stdlib.h>

#define NBITS 1024
#define NWORDS 1024/32
typedef unsigned int Word;
typedef Word Bits[NWORDS];
void Bitinsert(Bits a,int i);


void Bitinsert(Bits a,int i);

  int whichWord=i/32;
  int whichBit=i%32;
  a[whichWord]= a[whichWord] | (whichBit<<1);


int main(void) {
  printf("Hello World\n");
  return 0;
}
