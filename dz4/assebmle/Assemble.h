#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include <fstream>
#include <string>

class Assemble {
public:
  Assemble(const std::string &inputFile, const std::string &outputFile,
           const std::string &logFile);
  ~Assemble();

private:
  std::ofstream os;

private:
  template <int N> void writeBitsetToBinaryFile(const std::bitset<N> &code);
  void loadConstant(unsigned int B, unsigned int C);
  void memoryRead(unsigned int B, unsigned int C);
  void memoryWrite(unsigned int B, unsigned int C, unsigned int D);
  void bitwiseOr(unsigned int B, unsigned int C, unsigned int D);
};

#endif // ASSEMBLE_H
