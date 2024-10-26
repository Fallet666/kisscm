#include "Assemble.h"
#include <iostream>

int main(const int argc, char *argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: assembler <input file> <output file> <log file>\n";
    return 1;
  }
  Assemble(argv[1], argv[2], argv[3]);
  return 0;
}
