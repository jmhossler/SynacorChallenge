#include <stdint.h>
#include <stdio.h>
#include <fstream>
#include <iostream>

#define SIZE 0x8000

uint16_t r0,r1,r7;

uint16_t arr[SIZE][SIZE];

uint16_t ack2(uint16_t,uint16_t);
void resetArr();

int main() {
  std::fstream fp;

  std::cout << "Initializing Cache\n";
  for(int i = 0; i < SIZE; ++i) {
    for(int j = 0; j < SIZE; ++j) {
      arr[i][j] = 0x8000;
    }
  }
  std::cout << "Finished!\n";

  for(int i = 1; i < 0x8000; ++i) {
    r0=4;
    r1=1;
    r7=i;
    printf("Starting %d\n",i);
    uint16_t x = ack2(r0,r1);
    printf("Finished %d\n",i);
    if(x == 6) {
      printf("Ahaha! %04x\n",i);
    }
    resetArr();
    if(r0==6) {
      fp.open("findAnswer.txt",std::fstream::out | std::fstream::app);
      fp << "R0: " << r0 << "\n";
      fp << "R1: " << r1 << "\n";
      fp << "R7: " << r7 << "\n";
      fp << "\n";
      fp.close();
    }
  }

  return 0;
}

void resetArr() {
  for(int i = 0; i < SIZE; ++i) {
    for(int j = 0; j < SIZE; ++j) {
      arr[i][j] = 0x8000;
    }
  }
}

uint16_t ack2(uint16_t x, uint16_t y) {
  if(arr[x][y] != 0x8000) { return arr[x][y]; }
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
