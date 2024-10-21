#include "Translator.h"
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0]
              << " <input_json_file> <output_config_file>\n";
    return 1;
  }
  Translator(argv[1], argv[2]);
  return 0;
}