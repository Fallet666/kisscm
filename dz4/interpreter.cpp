#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Память и регистры виртуальной машины
std::vector<uint32_t> memory(1024, 0);
std::vector<uint32_t> registers(32, 0);

void debugOutput() {
  // Вывод регистров
  std::cout << "Registers: ";
  for (size_t i = 0; i < registers.size(); ++i) {
    std::cout << "R" << i << ":" << registers[i] << " ";
  }
  std::cout << "\nMemory: ";
  // Вывод первых 16 ячеек памяти
  for (size_t i = 0; i < 16; ++i) {
    std::cout << "M" << i << ":" << memory[i] << " ";
  }
  std::cout << "\n\n";
}

template <int N> std::bitset<N> readBitsetFromBinaryFile(std::istream &is) {
  std::bitset<N> code;
  for (int i = 0; i < N / 8; i++) {
    unsigned char byte;
    if (is.read(reinterpret_cast<char *>(&byte), sizeof(byte))) {
      code |= std::bitset<N>(byte) << (i * 8);
    }
  }
  return code;
}

// Функция для декодирования loadConstant из считанного битсета
void loadConstantFromCode(const std::bitset<40> code) {
  const unsigned int A = (code >> 0).to_ulong() & 0b111;           // 3 бита
  const unsigned int B = (code >> 3).to_ulong() & ((1 << 24) - 1); // 24 бита
  const unsigned int C = (code >> 27).to_ulong() & ((1 << 7) - 1); // 7 бит
  std::cout << "LOAD_CONSTANT: A=" << A << ", B=" << B << ", C=" << C << "\n";
  registers[C] = B;
}

// Функция для декодирования memoryRead из считанного битсета
void memoryReadFromCode(const std::bitset<40> code) {

  const unsigned int A = (code >> 0).to_ulong() & 0b111;          // 3 бита
  const unsigned int B = (code >> 3).to_ulong() & ((1 << 7) - 1); // 7 бит
  const unsigned int C = (code >> 10).to_ulong() & ((1 << 25) - 1); // 25 бит
  if (C < memory.size()) {
    registers[B] = memory[C];
  }
  std::cout << "MEMORY_READ: A=" << A << ", B=" << B << ", C=" << C << "\n";
}

// Функция для декодирования memoryWrite из считанного битсета
void memoryWriteFromCode(const std::bitset<32> code) {
  const unsigned int A = (code >> 0).to_ulong() & 0b111;           // 3 бита
  const unsigned int B = (code >> 3).to_ulong() & ((1 << 7) - 1);  // 7 бит
  const unsigned int C = (code >> 10).to_ulong() & ((1 << 7) - 1); // 7 бит
  const unsigned int D = (code >> 17).to_ulong() & ((1 << 14) - 1); // 14 бит
  if (C < registers.size() && (registers[C] + D) < memory.size()) {
    memory[registers[C] + D] = registers[B];
  }
  std::cout << "MEMORY_WRITE: A=" << A << ", B=" << B << ", C=" << C
            << ", D=" << D << "\n";
}

// Функция для декодирования bitwiseOr из считанного битсета
void bitwiseOrFromCode(const std::bitset<32> code) {
  const unsigned int A = (code >> 0).to_ulong() & 0b111;           // 3 бита
  const unsigned int B = (code >> 3).to_ulong() & ((1 << 14) - 1); // 14 бит
  const unsigned int C = (code >> 17).to_ulong() & ((1 << 7) - 1); // 7 бит
  const unsigned int D = (code >> 24).to_ulong() & ((1 << 7) - 1); // 7 бит
  if (C < registers.size() && D < registers.size() &&
      (registers[D] + B) < memory.size()) {
    registers[C] |= memory[registers[D] + B];
  }
  std::cout << "BITWISE_OR: A=" << A << ", B=" << B << ", C=" << C
            << ", D=" << D << "\n";
}

// Изменим функцию executeBinaryFile для проверки типа команды и чтения
// соответствующего количества байт
void executeBinaryFile(const std::string &binaryFile) {
  std::ifstream infile(binaryFile, std::ios::binary);
  if (!infile) {
    std::cerr << "Error opening binary file." << std::endl;
    return;
  }

  while (infile.peek() != EOF) {
    // Читаем первые 3 бита для определения размера команды
    std::bitset<8> commandType = readBitsetFromBinaryFile<8>(infile);
    const unsigned int A = (commandType >> 0).to_ulong() & 0b111;

    infile.seekg(-1, std::ios::cur); // Возвращаемся назад на 1 байт для полного
                                     // считывания команды

    if (A == 0 || A == 7) {
      // 40 бит для loadConstant и memoryRead
      const std::bitset<40> code = readBitsetFromBinaryFile<40>(infile);
      switch (A) {
      case 0:
        loadConstantFromCode(code);
        break;
      case 7:
        memoryReadFromCode(code);
        break;
      default:;
      }
    } else {
      const std::bitset<32> code = readBitsetFromBinaryFile<32>(infile);
      switch (A) {
      case 1:
        // Обработка команды записи в память (memoryWrite)
        memoryWriteFromCode(code);
        break;
      case 3:
        // Обработка команды побитового "или" (bitwiseOr)
        bitwiseOrFromCode(code);
        break;
      default:
        std::cerr << "Unknown command type!" << std::endl;
        break;
      }
    }

    debugOutput(); // Отладочный вывод после каждой команды
  }
}

void saveMemoryToCSV(const std::string &resultFile, size_t start, size_t end) {
  std::ofstream out(resultFile);
  out << "ID, VALUE" << std::endl;
  for (size_t i = start; i <= end && i < memory.size(); ++i) {
        out << i << "," << memory[i] << "\n";
    }
}

int main(const int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <binaryFile> <resultFile> <start> <end>" << std::endl;
        return 1;
    }

    const std::string binaryFile = argv[1];
    const std::string resultFile = argv[2];
    const size_t start = std::stoul(argv[3]);
    const size_t end = std::stoul(argv[4]);

    executeBinaryFile(binaryFile);
    saveMemoryToCSV(resultFile, start, end);

    return 0;
}
