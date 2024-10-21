#include <boost/json.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

namespace json = boost::json;

bool keyValidator(const std::string &key) {
  return !std::all_of(key.begin(), key.end(), [](const char &c) {
    return std::islower(c) || c == '_';
  });
}

std::string jsonToConfig(const json::value &jv) {
  std::stringstream ss;

  if (jv.is_object()) {
    auto obj = jv.as_object();
    for (const auto &[key, value] : obj) {
      if (keyValidator(key)) {
        std::cout << "invalid key: " << key << "\n";
        continue;
      }
      if (key == "comment") {
        if (std::string temp = value.as_string().c_str();
            temp.find('\n') != std::string::npos) {
          ss << "{{!\n";
          for (const char &i : temp) {
            if (i == '\n')
              ss << "\n";
            else
              ss << i;
          }
          ss << "\n}}";
        } else {
          ss << "// " << temp << "\n";
        }
        continue;
      }
      if (value.is_int64()) {
        ss << "var " << key << " := " << value.as_int64() << "\n";
      } else if (value.is_array()) {
        ss << "var " << key << " := array( ";
        for (const auto &val : value.as_array()) {
          if (val.is_int64()) {
            ss << val.as_int64() << ", ";
          }
        }
        ss.seekp(-2, std::stringstream::cur);
        ss << " )\n";
      }
    }
  }
  return ss.str();
}
std::string readFile(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file: " + filename);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

void writeFile(const std::string &filename, const std::string &content) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file: " + filename);
  }

  file << content;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0]
              << " <input_json_file> <output_config_file>\n";
    return 1;
  }

  std::string inputFile = argv[1];
  std::string outputFile = argv[2];

  try {
    std::string jsonContent = readFile(inputFile);
    json::value jv = json::parse(jsonContent);

    std::string configContent = jsonToConfig(jv);

    writeFile(outputFile, configContent);

    std::cout << "Transformation completed successfully.\n";
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}