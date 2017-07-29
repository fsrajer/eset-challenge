#pragma once

#include <string>
#include <vector>
#include <chrono>

#include "TextSegment.h"
#include "ProducerConsumerBuffer.h"

using std::string;
using std::vector;

class PatternSearch {
public:
  PatternSearch();

  void findPattern(const string& pattern, 
    const string& path, vector<string> *output);

private:
  /// This will be filled by file crawler and processed here.
  ProducerConsumerBuffer<TextSegment> segments_;
  const static int cMaxSegmentsInMemory = 100;
  const static int cNThreads = 8;
  std::mutex outputMutex_;

  void findPatternWorker(const string& pattern, vector<string> *output);

  const static int cMaxPrefixLength = 3;
  const static int cMaxSuffixLength = 3;
  
  void findPatternInText(const string& pattern, const string& text,
    vector<int> *positions);
  string extractPrefix(const string& text, int patternStartIdx);
  /// patternEndIdx points to the first element after the pattern
  string extractSuffix(const string& text, int patternEndIdx);
  /// Format one entry of the result.
  void formatResult(const string& filename, int position,
    const string& prefix, const string& suffix, string *result);
};




