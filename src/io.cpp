#include "io.h"

#include <fstream>
#include <exception>

void readFile(const string &filename, string *text) {
  *text = "";

  std::ifstream file(filename);

  if (!file.is_open()) {
    throw std::runtime_error("Could not open file.");
  }

  string line;
  while (std::getline(file, line)) {
    *text += line + "\n";
  }
  if (!text->empty()) {
    text->erase(text->size()-1);
  }

  file.close();
}

void writeFile(const string& filename, const string& text) {
  std::ofstream file(filename);
  
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file.");
  }

  file << text;
  file.close();
}

void deleteFile(const string& filename) {
  int exitCode = std::remove(filename.c_str());
  if (exitCode != 0) {
    throw std::runtime_error("Could not delete file.");
  }
}