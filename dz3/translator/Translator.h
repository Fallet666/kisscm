#pragma once
#include <boost/json.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

#ifndef TRANSLATOR_H
#define TRANSLATOR_H

namespace json = boost::json;

class Translator {
public:
  Translator(const std::string &inputFile, const std::string &outputFile);
  static bool keyValidator(const std::string &key);
  static std::string jsonToConfig(const json::value &jv);
  static std::string readFile(const std::string &filename);
  static void writeFile(const std::string &filename,
                        const std::string &content);
};

#endif // TRANSLATOR_H
