#include "TextSegment.h"

#include <algorithm>

TextSegment::TextSegment()
  : filename_(""), offset_(-1), textSize_(0), text_(new string) {
}

TextSegment::TextSegment(const string& filename, int offset)
  : filename_(filename), offset_(offset), textSize_(0), text_(new string) {
}

const string& TextSegment::filename() const {
  return filename_;
}

int TextSegment::offset() const {
  return offset_;
}

int TextSegment::textSize() const {
  return textSize_;
}

size_t TextSegment::find(const string& substr, int offset) const {
  size_t pos = text_->find(substr, offset + preTextSize_);
  if (pos != string::npos) {
    pos -= preTextSize_;
    if ((pos + substr.size()) > textSize_) {
      pos = string::npos;
    }
  }
  return pos;
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

  textSize_ = std::min(cMaxSize, fileLength - offset_);

  // Load also a potential prefix and suffix.
  preTextSize_ = std::min(cMaxPrefixSize, offset_);
  postTextSize_ = std::min(cMaxSuffixSize, fileLength - offset_ - textSize_);

  int readOffset = offset_ - preTextSize_;
  int loadSize = textSize_ + preTextSize_ + postTextSize_;
  
  in->seekg(readOffset, std::ios::beg);
  text_->resize(loadSize);

  in->read(&(*text_)[0], loadSize);
}

string TextSegment::extractPrefix(int patternStartIdx) {
  int patternStartIdxLocal = patternStartIdx + preTextSize_;
  return text_->substr(std::max(0, patternStartIdxLocal - cMaxPrefixSize),
    std::min(cMaxPrefixSize, patternStartIdxLocal));
}

string TextSegment::extractSuffix(int patternEndIdx) {
  int patternEndIdxLocal = patternEndIdx + preTextSize_;
  return text_->substr(patternEndIdxLocal,
    std::min(cMaxSuffixSize, static_cast<int>(text_->size()) - patternEndIdxLocal));
}