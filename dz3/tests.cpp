#include "Translator.h"
#include <gtest/gtest.h>

TEST(keyVaidatorTest, TestTranslate) {
  EXPECT_EQ(Translator::keyValidator("test"), true);
  EXPECT_EQ(Translator::keyValidator("_test_"), true);
  EXPECT_EQ(Translator::keyValidator("Test"), false);
  EXPECT_EQ(Translator::keyValidator("test1"), false);
}

TEST(translatorTest, TestTranslate) {
  std::ofstream file("test.json");
  file << "{\n"
       << "\"log_level\": 3,\n"
       << "\"log_outputs\": [1, 2, 3],\n"
       << "\"comment\": \"comment \\nnew line\"\n"
       << "}";
  file.close();
  Translator("test.json", "test.txt");
  std::ifstream tr_file("test.txt");
  std::string line;
  getline(tr_file, line);
  EXPECT_EQ("var log_level := 3", line);
  getline(tr_file, line);
  EXPECT_EQ("var log_outputs := array( 1, 2, 3 )", line);
  getline(tr_file, line);
  EXPECT_EQ("{{!", line);
  getline(tr_file, line);
  EXPECT_EQ("comment ", line);
  getline(tr_file, line);
  EXPECT_EQ("new line", line);
  getline(tr_file, line);
  EXPECT_EQ("}}", line);
  tr_file.close();
  std::remove("test.txt");
  std::remove("test.json");
}