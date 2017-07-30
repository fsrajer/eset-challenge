//-----------------------------------------------------------------------------
/**
* \file       PatternSearch.h
* \author     Filip Srajer
* \date       July, 2017
* \brief      Class for search of a pattern in a file or directory.
*
*/
//-----------------------------------------------------------------------------
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

  /// Format one entry of the result.
  static void formatResult(const string& filename, long long position,
    const string& prefix, const string& suffix, string *result);

  // Signed representation is more convenient for subtraction.
  const static long long cMaxSegmentSize = 10000000;
  const static long long cMaxPrefixSize = 3;
  const static long long cMaxSuffixSize = 3;
  const static int cMaxSegmentsInMemory = 100;
  const static int cNThreads = 8;

private:
  /// This will be filled by file crawler and processed here.
  ProducerConsumerBuffer<TextSegment> segments_;
  std::mutex outputMutex_;


  void findPatternWorker(const string& pattern, vector<string> *output);
    
  void findPatternInText(const string& pattern, const TextSegment& text,
    vector<long long> *positions);
};




