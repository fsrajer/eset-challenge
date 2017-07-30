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
  TextSegment(const string& filename, int offset);

  const string& filename() const;
  int offset() const;
  int textSize() const;

  // Find substring
  size_t find(const string& substr, int offset) const;

  void readFromFile();
  /// Using a pre-opened file
  void readFromFile(int fileLength, std::ifstream *in);

  string extractPrefix(int patternStartIdx);
  /// patternEndIdx points to the first element after the pattern
  string extractSuffix(int patternEndIdx);

  const static int cMaxSize = 10000000;
  const static int cMaxPrefixSize = 3;
  const static int cMaxSuffixSize = 3;

private:
  string filename_;
  int offset_;
  int textSize_;
  int preTextSize_;
  int postTextSize_;
  shared_ptr<string> text_;
};