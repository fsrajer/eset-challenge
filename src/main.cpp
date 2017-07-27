#include <iostream>
#include <string>
#include <vector>

#include "io.h"
#include "pattern_search.h"

using std::cout;
using std::string;
using std::vector;

const int cPatternMaxLength = 128;

void printUsage() {
  cout
    << "Usage: PatternSearchApp path pattern\n"
    << "Arguments:\n"
    << "path\t: path to a file or folder\n"
    << "pattern\t: pattern to be searched for (max " 
    + std::to_string(cPatternMaxLength) + " characters)\n";
}

int main(int argc, char *argv[]) {
  
  if (argc != 3) {
    printUsage();
    return EXIT_FAILURE;
  }

  string path(argv[1]);
  string pattern(argv[2]);

  if (!isDir(path) && !isFile(path)) {
    cout << "The first argument is not a file or a directory.\n";
    printUsage();
    return EXIT_FAILURE;
  }

  if (pattern.size() > cPatternMaxLength) {
    cout << "Pattern longer than specified maximum length.\n";
    printUsage();
    return EXIT_FAILURE;
  }

  vector<string> output;
  findPatternInFileOrDirectory(pattern, path, &output);
  for (const auto& line : output) {
    cout << line << "\n";
  }
}