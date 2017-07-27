#include "pattern_search.h"

#include <algorithm>
#include <iostream>

#include "io.h"

const int cMaxPrefixLength = 3;
const int cMaxSuffixLength = 3;

void findPatternInText(const string& pattern, const string& text,
  vector<int> *ppositions) {
  
  auto& positions = *ppositions;
  positions.clear();

  if (pattern.empty())
    return;

  size_t pos = text.find(pattern, 0);
  while (pos != string::npos) {
    positions.push_back(int(pos));
    pos = text.find(pattern, pos+1);
  }
}

string extractPrefix(const string& text, int patternStartIdx) {
  return text.substr(std::max(0, patternStartIdx - cMaxPrefixLength),
    std::min(cMaxPrefixLength, patternStartIdx));
}

string extractSuffix(const string& text, int patternEndIdx) {
  return text.substr(patternEndIdx, 
    std::min(cMaxSuffixLength, static_cast<int>(text.size()) - patternEndIdx));
}

void formatResult(const string& filename, int position,
  const string& prefix, const string& suffix, string *presult) {
  
  auto& result = *presult;
  result = filename + "(" + std::to_string(position) + "):"
    + prefix + "..." + suffix;

  // Handle newlines and tabs
  size_t pos = result.find_first_of("\t\n", 0);
  while (pos != string::npos) {
    result[pos] = (result[pos] == '\n') ? 'n' : 't';
    result.insert(result.begin() + pos, '\\');
    pos = result.find_first_of("\t\n", pos);
  }
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