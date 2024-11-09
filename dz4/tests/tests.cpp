#include "Assemble.h"
#include "Interpret.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(Assemble, Assembler_load) {
  std::ofstream input("test_input.asm");
  input << "LOAD_CONSTANT R22, #482";
  input.close();
  Assemble("test_input.asm", "test_output.bin", "test_log.csv");
  std::ifstream output("test_output.bin");
  unsigned char byte;

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x10);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x0F);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x00);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0xB0);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x00);

  std::remove("test_input.asm");
  std::remove("test_output.bin");
  std::remove("test_log.csv");
}

TEST(Assemble, Assembler_Read) {
  std::ofstream input("test_input.asm");
  input << "MEMORY_READ R45, [310]";
  input.close();
  Assemble("test_input.asm", "test_output.bin", "test_log.csv");
  std::ifstream output("test_output.bin");
  unsigned char byte;

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x6f);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0xd9);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x04);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x00);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x00);

  std::remove("test_input.asm");
  std::remove("test_output.bin");
  std::remove("test_log.csv");
}

TEST(Assemble, Assembler_Write) {
  std::ofstream input("test_input.asm");
  input << "MEMORY_WRITE [R33 + 543], R95";
  input.close();
  Assemble("test_input.asm", "test_output.bin", "test_log.csv");
  std::ifstream output("test_output.bin");
  unsigned char byte;

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0xf9);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x86);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x3e);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x04);

  std::remove("test_input.asm");
  std::remove("test_output.bin");
  std::remove("test_log.csv");
}

TEST(Assemble, Assembler_Or) {
  std::ofstream input("test_input.asm");
  input << "OR R108, [R83 + 878]";
  input.close();
  Assemble("test_input.asm", "test_output.bin", "test_log.csv");
  std::ifstream output("test_output.bin");
  unsigned char byte;

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x73);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x1b);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0xd8);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x53);

  std::remove("test_input.asm");
  std::remove("test_output.bin");
  std::remove("test_log.csv");
}

TEST(Inerpret, Interpret_Write) {
  std::ofstream input("test_input.asm");
  input << "LOAD_CONSTANT R0 #52" << std::endl;
  input << "MEMORY_WRITE [R31 + 0], R0";
  input.close();
  Assemble("test_input.asm", "test_output.bin", "test_log.csv");
  std::remove("test_input.asm");
  std::remove("test_log.csv");
  Interpret("test_output.bin", "result.csv", 0, 2);
  std::remove("test_output.bin");
  std::ifstream result("result.csv");
  std::string line;
  std::getline(result, line);
  std::getline(result, line);
  EXPECT_EQ(line, "0,52");
}