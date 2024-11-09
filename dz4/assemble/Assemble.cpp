#include "Assemble.h"

template <int N>
void Assemble::writeBitsetToBinaryFile(const std::bitset<N> &code) {

  for (int i = 0; i < N / 8; i++) {
    unsigned char byte = (code >> (i * 8)).to_ulong() & 0xFF;
    os.write(reinterpret_cast<const char *>(&byte), sizeof(byte));
  }
}

void Assemble::loadConstant(const unsigned int B, const unsigned int C) {
  std::bitset<40> code;
  constexpr unsigned int A = 0;

  for (int i = 0; i <= 2; i++)
    code[i] = (A >> i) & 1;
  for (int i = 3; i <= 26; i++)
    code[i] = (B >> (i - 3)) & 1;
  for (int i = 27; i <= 33; i++)
    code[i] = (C >> (i - 27)) & 1;

  writeBitsetToBinaryFile<40>(code);
}

void Assemble::memoryRead(const unsigned int B, const unsigned int C) {
  std::bitset<40> code;
  constexpr unsigned int A = 7;

  for (int i = 0; i <= 2; i++)
    code[i] = (A >> i) & 1;
  for (int i = 3; i <= 9; i++)
    code[i] = (B >> (i - 3)) & 1;
  for (int i = 10; i <= 34; i++)
    code[i] = (C >> (i - 10)) & 1;

  writeBitsetToBinaryFile<40>(code);
}

void Assemble::memoryWrite(const unsigned int B, const unsigned int C,
                           const unsigned int D) {
  std::bitset<32> code;
  constexpr unsigned int A = 1;

  for (int i = 0; i <= 2; i++)
    code[i] = (A >> i) & 1;
  for (int i = 3; i <= 9; i++)
    code[i] = (B >> (i - 3)) & 1;
  for (int i = 10; i <= 16; i++)
    code[i] = (C >> (i - 10)) & 1;
  for (int i = 17; i <= 30; i++)
    code[i] = (D >> (i - 17)) & 1;

  writeBitsetToBinaryFile<32>(code);
}

void Assemble::bitwiseOr(const unsigned int B, const unsigned int C,
                         const unsigned int D) {
  std::bitset<32> code;
  constexpr unsigned int A = 3;

  for (int i = 0; i <= 2; i++)
    code[i] = (A >> i) & 1;
  for (int i = 3; i <= 16; i++)
    code[i] = (B >> (i - 3)) & 1;
  for (int i = 17; i <= 23; i++)
    code[i] = (C >> (i - 17)) & 1;
  for (int i = 24; i <= 30; i++)
    code[i] = (D >> (i - 24)) & 1;

  writeBitsetToBinaryFile<32>(code);
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

  std::string input, reg, operand;
  std::string plus, shift;
  unsigned int regAddr, value, addr, offset;

  while (infile >> input) {
    if (input == "LOAD_CONSTANT") {
      try {
        infile >> reg >> operand;
        if (reg[reg.size() - 1] != ',') {
          throw std::invalid_argument("Missing comma");
        }
        if (reg.empty() || reg[0] != 'R') {
          throw std::invalid_argument("Invalid register");
        }
        if (operand.empty() || operand[0] != '#') {
          throw std::invalid_argument("Invalid operand");
        }
        regAddr = std::stoi(reg.substr(1, reg.size() - 1));
        value = std::stoi(operand.substr(1));

        loadConstant(value, regAddr);
        logfile << "LOAD_CONSTANT: A = 0, B (constant) = " << value
                << ", C (register) = " << regAddr << std::endl;
      } catch (std::invalid_argument &e) {
        std::cerr << "Syntax error. Load constant failed. " << inputFile << "\n"
                  << e.what() << std::endl;
        std::cerr << "input: '" << input << "', register: '" << reg
                  << "', operand: '" << operand << "'" << std::endl;
        return;
      } catch (...) {
        std::cerr << "Syntax error. Load constant failed. " << inputFile
                  << std::endl;
        std::cerr << "input: '" << input << "', register: '" << reg
                  << "', operand: '" << operand << "'" << std::endl;
        return;
      }
    } else if (input == "MEMORY_READ") {
      try {
        infile >> reg >> operand;
        if (reg[reg.size() - 1] != ',') {
          throw std::invalid_argument("Missing comma");
        }
        if (reg.empty() || reg[0] != 'R') {
          throw std::invalid_argument("Invalid register");
        }
        if (operand.empty() || operand[0] != '[' ||
            operand[operand.size() - 1] != ']') {
          throw std::invalid_argument("Invalid operand");
        }
        regAddr = std::stoi(reg.substr(1, reg.size() - 1));
        addr = std::stoi(operand.substr(1, operand.size() - 2));

        memoryRead(regAddr, addr);
        logfile << "MEMORY_READ: A = 7, B (register) = " << regAddr
                << ", C (address) = " << addr << std::endl;
      } catch (std::invalid_argument &e) {
        std::cerr << "Syntax error. Memory read failed. " << inputFile << "\n"
                  << e.what() << std::endl;
        std::cerr << "input: '" << input << "', register: '" << reg
                  << "', operand: '" << operand << "'" << std::endl;
        return;
      } catch (...) {
        std::cerr << "Syntax error. Load constant failed. " << inputFile
                  << std::endl;
        std::cerr << "input: '" << input << "', register: '" << reg
                  << "', operand: '" << operand << "'" << std::endl;
        return;
      }

    } else if (input == "MEMORY_WRITE") {
      try {
        infile >> operand >> plus >> shift >> reg;
        if (shift[shift.size() - 1] != ',') {
          throw std::invalid_argument("Missing comma");
        }
        if (plus != "+") {
          throw std::invalid_argument("Missing plus");
        }
        if (reg.empty() || reg[0] != 'R') {
          throw std::invalid_argument("Invalid register");
        }
        if (operand[0] != '[' || shift[shift.size() - 2] != ']' ||
            operand[1] != 'R') {
          throw std::invalid_argument("Invalid operand");
        }

        regAddr = std::stoi(operand.substr(2));
        offset = std::stoi(shift.substr(0, shift.size() - 2));
        value = std::stoi(reg.substr(1));

        memoryWrite(value, regAddr, offset);
        logfile << "MEMORY_WRITE: A = 1, B (value) = " << value
                << ", C (register) = " << regAddr << ", D (offset) = " << offset
                << std::endl;
      } catch (std::invalid_argument &e) {
        std::cerr << "Syntax error. Memory write failed. " << inputFile << "\n"
                  << e.what() << std::endl;
        std::cerr << "input: '" << input << "', operand: '" << operand << " "
                  << plus << " " << shift << "', reg: '" << reg << "'"
                  << std::endl;
        return;
      } catch (...) {
        std::cerr << "Syntax error. Load constant failed. " << inputFile
                  << std::endl;
        std::cerr << "input: '" << input << "', operand: '" << operand << " "
                  << plus << " " << shift << "', reg: '" << reg << "'"
                  << std::endl;
      }

    } else if (input == "OR") {
      try {
        infile >> reg >> operand >> plus >> shift;
        if (reg[reg.size() - 1] != ',') {
          throw std::invalid_argument("Missing comma");
        }
        if (plus != "+") {
          throw std::invalid_argument("Missing plus");
        }
        if (reg.empty() || reg[0] != 'R') {
          throw std::invalid_argument("Invalid register");
        }
        if (operand[0] != '[' || shift[shift.size() - 1] != ']' ||
            operand[1] != 'R') {
          throw std::invalid_argument("Invalid operand");
        }
        regAddr = std::stoi(reg.substr(1, reg.size() - 2));
        addr = std::stoi(operand.substr(2));
        offset = std::stoi(shift.substr(0, shift.size() - 1));

        bitwiseOr(offset, regAddr, addr);
        logfile << "OR: A = 3, B (offset) = " << offset
                << ", C (register) = " << regAddr
                << ", D (memory address register) = " << addr << std::endl;

      } catch (std::invalid_argument &e) {
        std::cerr << "Syntax error. Bitwise or failed. " << inputFile << "\n"
                  << e.what() << std::endl;
        std::cerr << "input: '" << input << "', operand: '" << operand + " "
                  << plus + " " << shift << "', reg: '" << reg << "'"
                  << std::endl;
        return;
      } catch (...) {
        std::cerr << "Syntax error. Load constant failed. " << inputFile
                  << std::endl;
        std::cerr << "input: '" << input << "', operand: '" << operand << " "
                  << plus << " " << shift << "', reg: '" << reg << "'"
                  << std::endl;
      }
    }
  }

  infile.close();
  logfile.close();
}

Assemble::~Assemble() { os.close(); }