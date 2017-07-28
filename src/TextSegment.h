#pragma once

#include <string>
#include <memory>
#include <fstream>

using std::string;
using std::shared_ptr;

class TextSegment {
public:
  TextSegment();
  TextSegment(const string& filename, int offset);

  const string& filename() const;
  int offset() const;
  const string& text() const;

  void readFromFile();
  void readFromFile(int fileLength, std::ifstream *in);

  const static int cMaxSize = 10000000;

private:
  string filename_;
  int offset_;
  shared_ptr<string> text_;
};