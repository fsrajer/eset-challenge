#include "pattern_search.h"

#include <algorithm>
#include <iostream>

#include "io.h"

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

void formatResult(const string& filename, int position,
  const string& prefix, const string& suffix, string *result) {
  *result = filename + "(" + std::to_string(position) + "):"
    + prefix + "..." + suffix;
}

void findPatternInFile(const string& pattern, const string& filename,
  vector<string> *poutput) {

  auto& output = *poutput;
  output.clear();

  try {
    string text;
    readFile(filename, &text);

    vector<int> positions;
    findPatternInText(pattern, text, &positions);
    output.resize(positions.size());

    for (size_t i = 0; i < positions.size(); i++) {
      int position = positions[i];
      formatResult(filename, position, extractPrefix(text, position),
        extractSuffix(text, position + pattern.size()), &output[i]);
    }
  }
  catch (std::runtime_error err) {
    std::cerr << err.what() << "\n  " << filename << "\n";
  }
}

void findPatternInFileOrDirectory(const string& pattern, 
  const string& filenameOrDirectory, vector<string> *poutput) {

  auto& output = *poutput;
  output.clear();

  vector<string> allFilenames;
  listAllFiles(filenameOrDirectory, &allFilenames);

  for (const auto& fn : allFilenames) {
    vector<string> curr;
    findPatternInFile(pattern, fn, &curr);
    output.insert(output.end(), curr.begin(), curr.end());
  }
}