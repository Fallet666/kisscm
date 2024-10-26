#include "Assemble.h"
#include "Interpret.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(Assemble, Assembler_load) {
  std::ofstream input("test_input.txt");
  input << "LOAD_CONSTANT 482 22";
  input.close();
  Assemble("test_input.txt", "test_output.bin", "test_log.csv");
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

  std::remove("test_input.txt");
  std::remove("test_output.bin");
  std::remove("test_log.csv");
}

TEST(Assemble, Assembler_Read) {
  std::ofstream input("test_input.txt");
  input << "MEMORY_READ 45 310";
  input.close();
  Assemble("test_input.txt", "test_output.bin", "test_log.csv");
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

  std::remove("test_input.txt");
  std::remove("test_output.bin");
  std::remove("test_log.csv");
}

TEST(Assemble, Assembler_Write) {
  std::ofstream input("test_input.txt");
  input << "MEMORY_WRITE 95 33 543";
  input.close();
  Assemble("test_input.txt", "test_output.bin", "test_log.csv");
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

  std::remove("test_input.txt");
  std::remove("test_output.bin");
  std::remove("test_log.csv");
}

TEST(Assemble, Assembler_Or) {
  std::ofstream input("test_input.txt");
  input << "BITWISE_OR 878 108 83";
  input.close();
  Assemble("test_input.txt", "test_output.bin", "test_log.csv");
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

  std::remove("test_input.txt");
  std::remove("test_output.bin");
  std::remove("test_log.csv");
}

TEST(Inerpret, Interpret_Write) {
  std::ofstream input("test_input.txt");
  input << "LOAD_CONSTANT 52 0" << std::endl;
  input << "MEMORY_WRITE 0 31 0";
  input.close();
  Assemble("test_input.txt", "test_output.bin", "test_log.csv");
  std::remove("test_input.txt");
  std::remove("test_log.csv");
  Interpret("test_output.bin", "result.csv", 0, 2);
  std::remove("test_output.bin");
  std::ifstream result("result.csv");
  std::string line;
  std::getline(result, line);
  std::getline(result, line);
  EXPECT_EQ(line, "0,52");
}