#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

void readFile(const string& filename, string *text);

void writeFile(const string& filename, const string& text);

void deleteFile(const string& filename);

void listAllFiles(const string& filenameOrDirectory, 
  vector<string> *filenames); 
void listAllFilesInternal(const string& filenameOrDirectory,
  vector<string> *filenames);