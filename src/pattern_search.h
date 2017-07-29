#pragma once

#include <string>
#include <vector>
#include <chrono>

#include "TextSegment.h"

using std::string;
using std::vector;

void findPatternInText(const string& pattern, const string& text, 
  vector<int> *positions);

string extractPrefix(const string& text, int patternStartIdx);

/// patternEndIdx points to the first element after the pattern
string extractSuffix(const string& text, int patternEndIdx);

/// Format one entry of the result.
void formatResult(const string& filename, int position,
  const string& prefix, const string& suffix, string *result);

/// Output in the final format
void findPatternInSegment(const string& pattern, const TextSegment& segment,
  vector<string> *output);

/// output in the final format
/// \return duration of the search only (no file crawling) 
std::chrono::nanoseconds findPatternInFileOrDirectory(const string& pattern,
  const string& filenameOrDirectory, vector<string> *output);