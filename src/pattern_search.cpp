#include "pattern_search.h"

#include <algorithm>

void findPatternInText(const string& pattern, const string& text,
  vector<int> *ppositions) {
  
  auto& positions = *ppositions;
  positions.clear();

  if (pattern.empty())
    return;

  for (size_t i = 0; i < text.size(); i++) {
    bool found = true;
    for (size_t j = 0; j < pattern.size(); j++) {
      if (pattern[j] != text[i + j]) {
        found = false;
        break;
      }
    }
    if (found)
      positions.push_back(i);
  }
}

string extractPrefix(const string& text, int patternStartIdx) {
  return text.substr(std::max(0, patternStartIdx - 3), 
    std::min(3, patternStartIdx));
}

string extractSuffix(const string& text, int patternEndIdx) {
  return text.substr(patternEndIdx, 
    std::min(3, static_cast<int>(text.size()) - patternEndIdx));
}