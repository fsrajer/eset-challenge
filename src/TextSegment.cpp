#include "TextSegment.h"

#include <algorithm>

TextSegment::TextSegment() {
}

TextSegment::TextSegment(const string& filename, int offset)
  : filename_(filename), offset_(offset), text_(new string) {
}

const string& TextSegment::filename() const {
  return filename_;
}

const string& TextSegment::text() const {
  return *text_;
}

int TextSegment::offset() const {
  return offset_;
}

void TextSegment::readFromFile() {
  
  std::ifstream in(filename());
  if (!in.is_open()) {
    throw std::runtime_error("Could not open file.");
    return;
  }
  
  in.seekg(0, std::ios::end);
  int fileLength = in.tellg();

  readFromFile(fileLength, &in);
  in.close();
}

void TextSegment::readFromFile(int fileLength, std::ifstream *in) {
  
  in->seekg(offset_, std::ios::beg);

  int size = std::min(cMaxSize, fileLength - offset_);
  text_->resize(size);

  in->read(&(*text_)[0], size);
}