#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define SIZE 0x8000

uint16_t r7;

uint16_t **arr;

uint16_t ack2(uint16_t,uint16_t);
void resetArr();
void destroyArr();

int main() {
  FILE *fp = fopen("ans.txt","w");
  int i;
  uint16_t x;

  resetArr();

  for(i = 0x7150; i > 0; --i) {
    r7=i;
    printf("Starting %04x\n",i);
    x = ack2(4,1);
    if(x == 6) {
      fprintf(fp,"Ahaha! %04x\n",i);
      printf("Found it! %04x\n",i);
      break;
    }
    destroyArr();
    resetArr();
  }

  fclose(fp);

  return 0;
}

void destroyArr() {
  int i;
  for(i = 0; i < SIZE; ++i) {
    free(arr[i]);
  }
  free(arr);
  return;
}

void resetArr() {
  int i,j;
  arr = malloc(sizeof(uint16_t *) * SIZE);
  for(i = 0; i < SIZE; ++i) {
    arr[i] = calloc(SIZE,sizeof(uint16_t));
    /*
    for(j = 0; j < SIZE; ++j) {
      arr[i][j] = 0x8000;
    }
    */
  }
}

uint16_t ack2(uint16_t x, uint16_t y) {
  //if(arr[x][y] != 0x8000) { return arr[x][y]; }
  if(arr[x][y] != 0) { return arr[x][y]; }
  if(x == 0) {
    arr[x][y] = (y+1)&0x7fff;
    return (y+1)&0x7fff;
  }
  else if(y == 0) {
    arr[x][y] = ack2(x-1,r7);
    return ack2(x-1,r7);
  }
  else {
    arr[x][y] = ack2(x-1,ack2(x,y-1));
    return ack2(x-1,ack2(x,y-1));
  }
}
