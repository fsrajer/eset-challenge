#include "io_utils.h"

#include <windows.h>
#include <fstream>
#include <exception>

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

void createDir(const string& directory) {
  int exitCode = CreateDirectory(directory.c_str(), NULL);
  if (exitCode == 0) {
    throw std::runtime_error("Could not create directory.");
  }
  if (GetLastError() == ERROR_ALREADY_EXISTS) {
    throw std::runtime_error("Directory already exists.");
  }
}

void deleteDir(const string& directory) {
  int exitCode = RemoveDirectory(directory.c_str());
  if (exitCode == 0) {
    throw std::runtime_error("Could not delete directory.");
  }
}

bool isFile(const string& filename) {
  DWORD attrib = GetFileAttributes(filename.c_str());
  return (attrib != INVALID_FILE_ATTRIBUTES &&
    !(attrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool isDir(const string& directory) {
  DWORD attrib = GetFileAttributes(directory.c_str());
  return (attrib != INVALID_FILE_ATTRIBUTES &&
    (attrib & FILE_ATTRIBUTE_DIRECTORY));
}

void listAllFiles(const string& directory, vector<string> *pfilenames) {

  auto& filenames = *pfilenames;

  WIN32_FIND_DATA data;
  string directorySlash = directory;
  if (!directorySlash.empty() && 
    directorySlash.back() != '/' && 
    directorySlash.back() != '\\') {

    directorySlash  += "/";
  }

  HANDLE hFind = FindFirstFile((directorySlash + "*").c_str(), &data);

  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      string currFilename(data.cFileName);
      if (currFilename != "." && currFilename != "..") {
        filenames.push_back(directorySlash + currFilename);
      }
    } while (FindNextFile(hFind, &data));
    FindClose(hFind);
  }
}