#include <stdint.h>
#include <stdlib.h>
#include <ncurses.h>
#include <iostream>
#include <fstream>
#include "virMach.h"
#include "mem.h"

#define MAX_VAL 0x8000

// Init without ncurses, memory is initialized, register is made, stack is created.
VirtualMachine::VirtualMachine() {
  memAddr = 0;
  reg = new uint16_t[8];
  mem = Memory(reg,(uint16_t)MAX_VAL);
  stack = Stack();
  inWin = false;
}

// Init with ncurses, the same as the other, except it attaches the ncurses window
VirtualMachine::VirtualMachine(WINDOW *w) {
  memAddr = 0;
  reg = new uint16_t[8];
  mem = Memory(reg, (uint16_t)MAX_VAL);
  stack = Stack();
  win = w;
  inWin = true;
}

// This loads the binary file into memory, takes into account little endian
void VirtualMachine::load(char *filename) {
  FILE *fp = fopen(filename,"rb");

  uint8_t lsb, msb;
  uint16_t num;
  // testVal determines if the msb and lsb are both initialized to a full 16bit value
  bool testVal = false;

  while(!feof(fp)) {
    // Read in one character (1 byte)
    fscanf(fp,"%c",&lsb);
    if(testVal) {
      num = (lsb << 8) + msb;
      mem.setItem(memAddr++,num);
      testVal = false;
    }
    else {
      msb = lsb;
      testVal = true;
    }
  }
  fclose(fp);
  memAddr = 0;
}

void VirtualMachine::step() {
  uint16_t a,b,c;
  // I wanted to make function pointers, but with each instruction having a different number
  //    of arguments, I have no idea how to make that work. Had the idea to make all functions
  //    take three arguments and use function pointers, but I had already implemented this
  //    and decided to keep this, since this challenge was just for fun
  switch(mem.getItem(memAddr++)) {
    case 0:
      this->halt();
      break;
    case 1:
      a = mem.getItem(memAddr++); b = mem.getItem(memAddr++);
      this->set(a,b);
      break;
    case 2:
      this->push(mem.getItem(memAddr++));
      break;
    case 3:
      this->pop(mem.getItem(memAddr++));
      break;
    case 4:
      a = mem.getItem(memAddr++); b = mem.getItem(memAddr++); c = mem.getItem(memAddr++);
      this->eq(a,b,c);
      break;
    case 5:
      a = mem.getItem(memAddr++); b = mem.getItem(memAddr++); c = mem.getItem(memAddr++);
      this->gt(a,b,c);
      break;
    case 6:
      this->jmp(mem.getItem(memAddr++));
      break;
    case 7:
      a = mem.getItem(memAddr++); b = mem.getItem(memAddr++);
      this->jt(a,b);
      break;
    case 8:
      a = mem.getItem(memAddr++); b = mem.getItem(memAddr++);
      this->jf(a,b);
      break;
    case 9:
      a = mem.getItem(memAddr++); b = mem.getItem(memAddr++); c = mem.getItem(memAddr++);
      this->add(a,b,c);
      break;
    case 10:
      a = mem.getItem(memAddr++); b = mem.getItem(memAddr++); c = mem.getItem(memAddr++);
      this->mult(a,b,c);
      break;
    case 11:
      a = mem.getItem(memAddr++); b = mem.getItem(memAddr++); c = mem.getItem(memAddr++);
      this->mod(a,b,c);
      break;
    case 12:
      a = mem.getItem(memAddr++); b = mem.getItem(memAddr++); c = mem.getItem(memAddr++);
      this->AND(a,b,c);
      break;
    case 13:
      a = mem.getItem(memAddr++); b = mem.getItem(memAddr++); c = mem.getItem(memAddr++);
      this->OR(a,b,c);
      break;
    case 14:
      a = mem.getItem(memAddr++); b = mem.getItem(memAddr++);
      this->NOT(a,b);
      break;
    case 15:
      a = mem.getItem(memAddr++); b = mem.getItem(memAddr++);
      this->rmem(a,b);
      break;
    case 16:
      a = mem.getItem(memAddr++); b = mem.getItem(memAddr++);
      this->wmem(a,b);
      break;
    case 17:
      this->call(mem.getItem(memAddr++));
      break;
    case 18:
      this->ret();
      break;
    case 19:
      this->out(mem.getItem(memAddr++));
      break;
    case 20:
      this->in(mem.getItem(memAddr++));
      break;
    case 21:
      this->noop();
      break;
  }
}

// display really is just displayMemory, but I figured less typing was easier for a simple
//  debug function
void VirtualMachine::display() {
  std::cout << mem;
}

void VirtualMachine::execute() {
  // This keeps stepping through the instructions, catches any throws (stack throw, halt)
  while(1) {
    try {
      this->step();
    }
    catch(int e) {
      std::cerr << "Exception " << e << " was thrown!\n";
      exit(1);
    }
  }
}

void VirtualMachine::halt() {
  throw 0;
}

void VirtualMachine::noop() {
  return;
}

void VirtualMachine::out(uint16_t a) {
  if(!inWin) {
    std::cout << (char)this->value(a);
  }
  else {
    wprintw(win,"%c",(char)this->value(a));
    wrefresh(win);
  }
}

void VirtualMachine::push(uint16_t a) {
  stack.push(this->value(a));
}

void VirtualMachine::pop(uint16_t a) {
  mem.setItem(a,stack.pop());
}

void VirtualMachine::eq(uint16_t a, uint16_t b, uint16_t c) {
  if(this->value(b) == this->value(c)) {
    mem.setItem(a,1);
  }
  else {
    mem.setItem(a,0);
  }
}

void VirtualMachine::gt(uint16_t a, uint16_t b, uint16_t c) {
  if(this->value(b) > this->value(c)) {
    mem.setItem(a,1);
  }
  else {
    mem.setItem(a,0);
  }
}

void VirtualMachine::jmp(uint16_t a) {
  a = this->value(a);
  memAddr = a;
}

void VirtualMachine::jt(uint16_t a, uint16_t b) {
  a = this->value(a);
  if(a != 0) {
    memAddr = b;
  }
}

void VirtualMachine::jf(uint16_t a, uint16_t b) {
  a = this->value(a);
  if(a == 0) {
    memAddr = b;
  }
}

void VirtualMachine::add(uint16_t a, uint16_t b, uint16_t c) {
  mem.setItem(a,(this->value(b) + this->value(c)) & 0x7FFF);
}

void VirtualMachine::mult(uint16_t a, uint16_t b, uint16_t c) {
  mem.setItem(a,(this->value(b) * this->value(c)) & 0x7FFF);
}

void VirtualMachine::mod(uint16_t a, uint16_t b, uint16_t c) {
  uint16_t quotient = this->value(b) / this->value(c);
  mem.setItem(a,this->value(b) - (quotient * this->value(c)));
}

void VirtualMachine::AND(uint16_t a, uint16_t b, uint16_t c) {
  mem.setItem(a,(this->value(b) & this->value(c)));
}

void VirtualMachine::OR(uint16_t a, uint16_t b, uint16_t c) {
  mem.setItem(a, (this->value(b) | this->value(c)));
}

void VirtualMachine::NOT(uint16_t a, uint16_t b) {
  b = this->value(b);
  mem.setItem(a, ~b & 0x7FFF);
}

void VirtualMachine::set(uint16_t a, uint16_t b, uint16_t c) {
  reg[a-MAX_VAL] = this->value(b);
}

void VirtualMachine::rmem(uint16_t a, uint16_t b) {
  b = this->value(b);
  mem.setItem(a,mem.getItem(b));
}

void VirtualMachine::wmem(uint16_t a, uint16_t b) {
  a = this->value(a);
  b = this->value(b);
  mem.setItem(a,b);
}

void VirtualMachine::call(uint16_t a) {
  stack.push(memAddr);
  memAddr = this->value(a);
}

void VirtualMachine::ret() {
  memAddr = this->value(stack.pop());
}

void VirtualMachine::in(uint16_t a) {
  unsigned char val;
  if(!inWin) {
    scanf("%c",&val);
  }
  else {
    val = wgetch(win);
  }
  mem.setItem(a,(uint16_t)val);

}

uint16_t VirtualMachine::value(uint16_t a) {
  if(a < MAX_VAL) {
    return a;
  }
  else if(a < MAX_VAL + 8) {
    return reg[a-MAX_VAL];
  }
  else {
    std::cerr << a << " is an Invalid number\n";
    throw 22;
  }
}
