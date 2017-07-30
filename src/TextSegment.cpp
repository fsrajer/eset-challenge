#include "TextSegment.h"

#include <algorithm>

TextSegment::TextSegment()
  : filename_(""), offset_(-1), textSize_(0), text_(new string) {
}

TextSegment::TextSegment(const string& filename, long long offset)
  : filename_(filename), offset_(offset), textSize_(0), text_(new string) {
}

const string& TextSegment::filename() const {
  return filename_;
}

long long TextSegment::offset() const {
  return offset_;
}

long long TextSegment::textSize() const {
  return textSize_;
}

size_t TextSegment::find(const string& substr, size_t offset) const {
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
  }
  
  in.seekg(0, std::ios::end);
  long long fileLength = in.tellg();

  readFromFile(fileLength, &in);
  in.close();
}

void TextSegment::readFromFile(long long fileLength, std::ifstream *in) {

  textSize_ = std::min(cMaxSize, fileLength - offset_);

  // Load also a potential prefix and suffix.
  preTextSize_ = std::min(long long(cMaxPrefixSize), offset_);
  postTextSize_ = std::min(long long(cMaxSuffixSize), 
    fileLength - offset_ - textSize_);

  long long readOffset = offset_ - preTextSize_;
  long long loadSize = textSize_ + preTextSize_ + postTextSize_;
  
  in->seekg(readOffset, std::ios::beg);
  text_->resize(loadSize);

  in->read(&(*text_)[0], loadSize);
}

string TextSegment::extractPrefix(long long patternStartIdx) {
  long long patternStartIdxLocal = patternStartIdx + preTextSize_;
  return text_->substr(std::max(0LL, patternStartIdxLocal - cMaxPrefixSize),
    std::min(long long(cMaxPrefixSize), patternStartIdxLocal));
}

string TextSegment::extractSuffix(long long patternEndIdx) {
  long long patternEndIdxLocal = patternEndIdx + preTextSize_;
  return text_->substr(patternEndIdxLocal,
    std::min(long long(cMaxSuffixSize), 
      static_cast<int>(text_->size()) - patternEndIdxLocal));
}