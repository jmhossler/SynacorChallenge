#ifndef VM_SYNACOR_JMH_2015
#define VM_SYNACOR_JMH_2015

#include <stdint.h>
#include <ncurses.h>
#include "stack.h"
#include "mem.h"

class VirtualMachine {
  private:
    WINDOW *win; // FOR DEBUGGER USE ONLY
    // inWin determines if an ncurses windows is attached to the virtual machine
    bool inWin;
    // memAddr keeps track of the position in memory
    uint16_t memAddr;
    uint16_t *reg;
    Stack stack;
    Stack call_stack;
    Memory mem;
    uint16_t value(uint16_t a);
  public:
    // Step will execute the next instruction
    void step();
    // Made the functions for instructions public so they could be used by debugger
    void halt();
    void set(uint16_t a, uint16_t b, uint16_t c = 0);
    void push(uint16_t a);
    void pop(uint16_t a);
    void eq(uint16_t a, uint16_t b, uint16_t c);
    void gt(uint16_t a, uint16_t b, uint16_t c);
    void jmp(uint16_t a);
    void jt(uint16_t a, uint16_t b);
    void jf(uint16_t a, uint16_t b);
    void add(uint16_t a, uint16_t b, uint16_t c);
    void mult(uint16_t a, uint16_t b, uint16_t c);
    void mod(uint16_t a, uint16_t b, uint16_t c);
    void AND(uint16_t a, uint16_t b, uint16_t c);
    void OR(uint16_t a, uint16_t b, uint16_t c);
    void NOT(uint16_t a, uint16_t b);
    void rmem(uint16_t a, uint16_t b);
    void wmem(uint16_t a, uint16_t b);
    void call(uint16_t a);
    void ret();
    void out(uint16_t a);
    void in(uint16_t a);
    void noop();
    // Accessors so I can view the memory, stack, registers, and the memAddr
    Memory &getMem() { return mem; }
    Stack getStack() { return stack; }
    uint16_t getAddr() { return memAddr; }
    uint16_t *getReg() { return reg; }
    // Initializers
    VirtualMachine();
    VirtualMachine(WINDOW *);
    // this loads the filename into memory
    void load(char *fileName);
    // displays the memory, this isn't necessary, used for early debugging
    void display();
    // This executes all the commands, does not wait for anyone else
    void execute();
};

#endif
