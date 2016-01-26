#ifndef MEMORY_SYNACOR_JH_2015
#define MEMORY_SYNACOR_JH_2015
#include <stdint.h>
#include <stdlib.h>
#include <iostream>

#define BOUNDARY 32767

class Memory {
  private:
    // Arrays for storing memory and register values
    uint16_t *mem;
    uint16_t *reg;
  public:
    // Initializers
    Memory();
    Memory(uint16_t *r,size_t size);
    // getters and setters
    uint16_t getItem(uint16_t addr);
    void setItem(uint16_t addr, uint16_t val);

    // display function, for debugging purposes
    friend std::ostream& operator<<(std::ostream &os, Memory &m);
};

#endif
