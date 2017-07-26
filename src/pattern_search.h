#pragma once

#include <string>
#include <vector>

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
void findPatternInFile(const string& pattern, const string& filename,
  vector<string> *output);