#include "TextSegment.h"

#include <algorithm>

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

void TextSegment::readFromFile(int fileLength, std::ifstream *in) {
  
  in->seekg(offset_, std::ios::beg);

  int size = std::min(cMaxSize, fileLength - offset_);
  text_->reserve(size);

  in->read(&(*text_)[0], size);
}