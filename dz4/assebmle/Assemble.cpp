#include "Assemble.h"

template <int N>
void Assemble::writeBitsetToBinaryFile(const std::bitset<N> &code) {
  // Запись в порядке от младшего байта к старшему
  for (int i = 0; i < N / 8; i++) {
    unsigned char byte = (code >> (i * 8)).to_ulong() & 0xFF;
    os.write(reinterpret_cast<const char *>(&byte), sizeof(byte));
  }
}

void Assemble::loadConstant(const unsigned int B, const unsigned int C) {
  std::bitset<40> code;
  constexpr unsigned int A = 0;

  // Устанавливаем биты для A, B и C
  for (int i = 0; i <= 2; i++)
    code[i] = (A >> i) & 1;
  for (int i = 3; i <= 26; i++)
    code[i] = (B >> (i - 3)) & 1;
  for (int i = 27; i <= 33; i++)
    code[i] = (C >> (i - 27)) & 1;

  writeBitsetToBinaryFile<40>(code); // 5 байт
}

void Assemble::memoryRead(const unsigned int B, const unsigned int C) {
  std::bitset<40> code;
  constexpr unsigned int A = 7;

  // Устанавливаем биты для A, B и C
  for (int i = 0; i <= 2; i++)
    code[i] = (A >> i) & 1;
  for (int i = 3; i <= 9; i++)
    code[i] = (B >> (i - 3)) & 1;
  for (int i = 10; i <= 34; i++)
    code[i] = (C >> (i - 10)) & 1;

  writeBitsetToBinaryFile<40>(code); // 5 байт
}

void Assemble::memoryWrite(const unsigned int B, const unsigned int C,
                           const unsigned int D) {
  std::bitset<32> code;
  constexpr unsigned int A = 1;

  // Устанавливаем биты для A, B, C и D
  for (int i = 0; i <= 2; i++)
    code[i] = (A >> i) & 1;
  for (int i = 3; i <= 9; i++)
    code[i] = (B >> (i - 3)) & 1;
  for (int i = 10; i <= 16; i++)
    code[i] = (C >> (i - 10)) & 1;
  for (int i = 17; i <= 30; i++)
    code[i] = (D >> (i - 17)) & 1;

  writeBitsetToBinaryFile<32>(code); // 4 байта
}

void Assemble::bitwiseOr(const unsigned int B, const unsigned int C,
                         const unsigned int D) {
  std::bitset<32> code;
  constexpr unsigned int A = 3;

  // Устанавливаем биты для A, B, C и D
  for (int i = 0; i <= 2; i++)
    code[i] = (A >> i) & 1;
  for (int i = 3; i <= 16; i++)
    code[i] = (B >> (i - 3)) & 1;
  for (int i = 17; i <= 23; i++)
    code[i] = (C >> (i - 17)) & 1;
  for (int i = 24; i <= 30; i++)
    code[i] = (D >> (i - 24)) & 1;

  writeBitsetToBinaryFile<32>(code); // 4 байта
}

Assemble::Assemble(const std::string &inputFile, const std::string &outputFile,
                   const std::string &logFile) {
  std::ifstream infile(inputFile);
  os = std::ofstream(outputFile, std::ios::binary);
  std::ofstream logfile(logFile);
  if (!infile || !os || !logfile) {
    logfile << "Error opening files." << std::endl;
    return;
  }
  unsigned int B, C, D;
  std::string input;
  while (infile >> input) {
    if (input == "LOAD_CONSTANT") {
      infile >> B >> C;
      loadConstant(B, C);
      logfile << "A = " << 0 << ", B = " << B << ", C = " << C << std::endl;

    } else if (input == "MEMORY_READ") {
      infile >> B >> C;
      memoryRead(B, C);
      logfile << "A = " << 7 << ", B = " << B << ", C = " << C << std::endl;

    } else if (input == "MEMORY_WRITE") {
      infile >> B >> C >> D;
      memoryWrite(B, C, D);
      logfile << "A = " << 1 << ", B = " << B << ", C = " << C << ", D = " << D
              << std::endl;

    } else if (input == "BITWISE_OR") {
      infile >> B >> C >> D;
      bitwiseOr(B, C, D);
      logfile << "A = " << 3 << ", B = " << B << ", C = " << C << ", D = " << D
              << std::endl;
    }
  }
  infile.close();
  logfile.close();
}

Assemble::~Assemble() {
  os.close();
}