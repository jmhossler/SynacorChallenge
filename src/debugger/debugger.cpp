#include <ncurses.h>
#include <panel.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <string>
#include <unistd.h>
#include "../virMach.h"
#include "../mem.h"
#include "../stack.h"

#define INSTRCNM 710000

const static std::string comm[] = { "HALT", "SET", "PUSH", "POP", "EQ", "GT", "JMP", "JT",
            "JF", "ADD", "MULT", "MOD", "AND", "OR", "NOT", "RMEM", "WMEM", "CALL", "RET",
            "OUT", "IN", "NOOP" };

const static int numArgs[] = {0,2,1,1,3,3,1,2,2,3,3,3,3,3,2,2,2,1,0,1,1,0};

void addBorders(WINDOW **);
void displayMem(WINDOW *win, uint16_t memAddr, Memory &m, uint16_t *reg, int num);
int args(uint16_t val);
std::string parseInstruction(Memory &m, uint16_t memAddr, uint16_t *reg);
std::string value(uint16_t *reg, Memory &m, uint16_t val);
std::string iToA(int val);
void displayReg(WINDOW *win, uint16_t *reg);

int main(int argc, char **argv) {
  if(argc != 2) {
    std::cerr << "Usage:\tdebugger <filename>\n";
    exit(1);
  }

  struct winsize ts;
  ioctl(0,TIOCGWINSZ,&ts);
  WINDOW *my_wins[3];
  PANEL * my_panels[3];
  int i;

  initscr();
  cbreak();
  noecho();

  my_wins[0] = newwin(3 * ts.ws_row / 4,3 * ts.ws_col / 4,0,0);
  my_wins[1] = newwin(ts.ws_row / 4,ts.ws_col, 3 * ts.ws_row/4, 0);
  my_wins[2] = newwin(3 * ts.ws_row / 4,ts.ws_col / 4, 0, 3 * ts.ws_col/4);

  scrollok(my_wins[0],TRUE);
  scrollok(my_wins[2],TRUE);

  wtimeout(my_wins[2],0);

  VirtualMachine vm = VirtualMachine(my_wins[0]);

  vm.load(argv[argc-1]);

  for( i = 0; i < 3; ++i ) {
    box(my_wins[i], 0, 0);
    wborder(my_wins[i],'|','|','-','-','+','+','+','+');
  }
  wborder(my_wins[0],' ',' ',' ',' ',' ',' ',' ',' ');


  my_panels[0] = new_panel(my_wins[0]);
  my_panels[1] = new_panel(my_wins[1]);
  my_panels[2] = new_panel(my_wins[2]);

  update_panels();
  doupdate();

  bool swSem = false;
  int ch = 0;
  i = 0;
  while(1) {
    ch = wgetch(my_wins[2]);
    if(vm.getAddr() == 0x178b) {
      swSem = false;
      sleep(10);
      displayReg(my_wins[1],vm.getReg());
      displayMem(my_wins[2],vm.getAddr(),vm.getMem(),vm.getReg(),(3*ts.ws_row / 4)-2);
    }
    if(ch == 27) {
      swSem = !swSem;
    }
    if(vm.getAddr() == 0x1571) {
      displayMem(my_wins[2],vm.getAddr(),vm.getMem(),vm.getReg(),(3*ts.ws_row / 4)-2);
      vm.set(0x8007,0x6486);
      vm.wmem(0x1571,21);
      vm.wmem(0x1572,21);
      vm.set(0x8000,6);
    }
    if(swSem) {
        displayMem(my_wins[2],vm.getAddr(),vm.getMem(),vm.getReg(),(3 * ts.ws_row / 4) - 2);
    }
    try {
      vm.step();
    }
    catch(int e) {
      endwin();
      return 1;
    }

    addBorders(my_wins);
    update_panels();
    doupdate();
    ++i;
    //usleep(50000);
  }
  wgetch(my_wins[2]);
  endwin();

  return 0;
}

void displayReg(WINDOW *win, uint16_t *reg) {
  int i;
  wclear(win);
  wprintw(win,"\n");
  for(i = 0; i < 8; ++i) {
    wprintw(win,"\tREG%d: %05d",i,reg[i]);
    if((i+1) % 4 == 0) {
      wprintw(win,"\n");
    }
  }
}

void displayMem(WINDOW *win, uint16_t memAddr, Memory &m, uint16_t *reg, int num) {
  int i;
  wclear(win);
  wprintw(win,"\n");
  for(i = 0; i < num; ++i) {
    wprintw(win," 0x%04x %s\n",memAddr,parseInstruction(m,memAddr,reg).c_str());
    if(m.getItem(memAddr) > 21) {
      memAddr += 1;
      continue;
    }
    memAddr += args(m.getItem(memAddr)) + 1;
  }
}

std::string iToA(int val) {
  char str[80];
  sprintf(str,"0x%x",val);
  std::string ret = std::string(str);
  return ret;
}

std::string parseInstruction(Memory &m, uint16_t memAddr, uint16_t *reg) {
  uint16_t inst = m.getItem(memAddr);
  if(inst > 21) { return value(reg,m,m.getItem(memAddr)); }
  std::string out = comm[inst];
  //std::string out = iToA((int)inst);
  int arg = args(inst);
  int i;
  for(i = 1; i <= arg; ++i) {
    out += " ";
    //out += iToA((int)value(reg,m,m.getItem(memAddr+i)));
    out += value(reg,m,m.getItem(memAddr+i));
  }
  return out;
}

std::string value(uint16_t *reg, Memory &m, uint16_t val) {
  if(val < 0x8000) {
    return iToA((int)val);
  }
  else if(val < 0x8000 + 8) {
    switch(val-0x8000) {
      case 0:
        return "R0";
      case 1:
        return "R1";
      case 2:
        return "R2";
      case 3:
        return "R3";
      case 4:
        return "R4";
      case 5:
        return "R5";
      case 6:
        return "R6";
      case 7:
        return "R7";
      default:
        return "NULL";
    }
  }
  else {
    return "NULL";
  }
}

int args(uint16_t val) {
  return numArgs[val];
}

void addBorders(WINDOW **wins) {
  int i;
  for( i = 1; i < 3; ++i ) {
    wborder(wins[i],'|','|','-','-','+','+','+','+');
  }
}
