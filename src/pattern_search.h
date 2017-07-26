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
