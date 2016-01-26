#include "../virMach.h"

int main(int argc, char **argv) {
  if(argc < 2 || argc > 2) {
    std::cerr << "Usage: driver <filename>\n";
    exit(1);
  }

  // Initialize the virtualmachine
  VirtualMachine vm = VirtualMachine();

  // Load the memory from the file provided in command line arguments
  vm.load(argv[argc-1]);

  //vm.display();

  // Execute the program
  vm.execute();

  return 0;
}
