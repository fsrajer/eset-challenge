#pragma once

#include <string>
#include <memory>
#include <fstream>

using std::string;
using std::shared_ptr;

/// Representation hides prefix/suffix handling
/// and stores string in a shared pointer for
/// fast copying.
class TextSegment {
public:
  TextSegment();
  TextSegment(const string& filename, long long offset);

  const string& filename() const;
  long long offset() const;
  long long textSize() const;

  // Find substring
  size_t find(const string& substr, size_t offset) const;

  void readFromFile();
  /// Using a pre-opened file
  void readFromFile(long long fileLength, std::ifstream *in);

  string extractPrefix(long long patternStartIdx);
  /// patternEndIdx points to the first element after the pattern
  string extractSuffix(long long patternEndIdx);

private:
  string filename_;
  long long offset_;
  long long textSize_;
  long long preTextSize_;
  long long postTextSize_;
  shared_ptr<string> text_;
};