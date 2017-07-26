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

void listAllFiles(const string& filenameOrDirectory, 
  vector<string> *filenames) {
  
  filenames->clear();
  listAllFilesInternal(filenameOrDirectory, filenames);
}

bool isFile(const string& filename) {

}

bool isDir(const string& directory) {

}

void listAllFilesInDir(const string& directory, vector<string> *pfilenames) {

  auto& filenames = *pfilenames;

}

void listAllFilesInternal(const string& filenameOrDirectory,
  vector<string> *pfilenames) {

  auto& filenamesOut = *pfilenames;
  if (isFile(filenameOrDirectory)) {
    filenamesOut.push_back(filenameOrDirectory);
  }
  else if (isDir(filenameOrDirectory)) {
    vector<string> candidates;
    listAllFilesInDir(filenameOrDirectory, &candidates);
    for (const auto& candidate : candidates) {
      listAllFilesInternal(candidate, &filenamesOut);
    }
  }
}