#pragma once

#include <string>
#include <vector>
#include <chrono>

#include "TextSegment.h"

using std::string;
using std::vector;

class PatternSearch {
public:

  /// output in the final format
  /// \return duration of the search only (no file crawling) 
  std::chrono::nanoseconds findPattern(const string& pattern, 
    const string& path, vector<string> *output);

private:
  void findPatternWorker(const string& pattern, const TextSegment& segment,
    vector<string> *output);

  void findPatternInText(const string& pattern, const string& text,
    vector<int> *positions);
  string extractPrefix(const string& text, int patternStartIdx);
  /// patternEndIdx points to the first element after the pattern
  string extractSuffix(const string& text, int patternEndIdx);
  /// Format one entry of the result.
  void formatResult(const string& filename, int position,
    const string& prefix, const string& suffix, string *result);
};




