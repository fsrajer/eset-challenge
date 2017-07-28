#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "io_utils.h"
#include "pattern_search.h"

using std::cout;
using std::string;
using std::vector;

const int cPatternMaxLength = 128;
const int cNRuns = 100;

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
  
  auto begin = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < cNRuns; i++) {
    output.clear();
    findPatternInFileOrDirectory(pattern, path, &output);
  }
  auto end = std::chrono::high_resolution_clock::now();
  
  cout << "Runtime: "
    << std::chrono::duration_cast<std::chrono::milliseconds>(
      end - begin).count() / double(cNRuns)
    << "ms averaged over " << cNRuns << " runs\n";
  
  for (const auto& line : output) {
    cout << line << "\n";
  }
}