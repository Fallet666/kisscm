#include "Interpret.h"

#include <fstream>
#include <iostream>
#include <string>

int main(const int argc, char *argv[]) {
  if (argc != 5) {
    std::cerr << "Usage: " << argv[0]
              << " <binaryFile> <resultFile> <start> <end>" << std::endl;
    return 1;
  }

  const std::string binaryFile = argv[1];
  const std::string resultFile = argv[2];
  const size_t start = std::stoul(argv[3]);
  const size_t end = std::stoul(argv[4]);

  Interpret(binaryFile, resultFile, start, end, true);

  return 0;
}
