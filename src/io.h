#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

void readFile(const string& filename, string *text);
void writeFile(const string& filename, const string& text);
void deleteFile(const string& filename);

void createDir(const string& directory);
void deleteDir(const string& directory);

void listAllFiles(const string& filenameOrDirectory, 
  vector<string> *filenames); 
void listAllFilesInternal(const string& filenameOrDirectory,
  vector<string> *filenames);

bool isFile(const string& filename);
bool isDir(const string& directory);
void listAllFilesInDir(const string& directory, vector<string> *pfilenames);