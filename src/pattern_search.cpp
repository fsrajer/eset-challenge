#include "pattern_search.h"

void findPattern(const string& pattern, const string& text,
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