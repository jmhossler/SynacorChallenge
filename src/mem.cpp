#include <stdint.h>
#include <iostream>
#include <stdlib.h>
#include "mem.h"

// This init really has no purpose. I think I built this because the compiler yelled at me
Memory::Memory() {
  mem = new uint16_t[BOUNDARY + 1];
  reg = NULL;
}

// This init attaches the register to the memory, and initializes the size of the memory
Memory::Memory(uint16_t *r, size_t size) {
  mem = new uint16_t[size];
  reg = r;
}

// Simple, gets the value at addr
uint16_t Memory::getItem(uint16_t addr) {
  return mem[addr];
}

// This sets the value at addr to val, automatically decides if it should be a register
void Memory::setItem(uint16_t addr, uint16_t val) {
  if(addr <= BOUNDARY) {
    mem[addr] = val;
  }
  else if(addr > BOUNDARY && addr <= BOUNDARY + 8) {
    reg[addr - (BOUNDARY + 1)] = val;
  }
  else {
    std::cerr << "Invalid Memory Address\n";
    exit(1);
  }
}

// This was made for debugging, to make sure memory was being loaded correctly
std::ostream& operator<<(std::ostream &os, Memory &m) {
  int i;
  for(i = 0; i <= BOUNDARY; ++i) {
    os << m.mem[i] << "\n";
  }
  return os;
}
