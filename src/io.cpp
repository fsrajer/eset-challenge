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
