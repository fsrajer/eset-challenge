#include "pattern_search.h"

#include <algorithm>
#include <iostream>

#include "FileCrawler.h"

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

void findPatternInSegment(const string& pattern, const TextSegment& segment,
  vector<string> *poutput) {

  auto& output = *poutput;
  output.clear();

  vector<int> positions;
  findPatternInText(pattern, segment.text(), &positions);
  output.resize(positions.size());

  for (size_t i = 0; i < positions.size(); i++) {
    int position = positions[i];
    formatResult(segment.filename(), segment.offset() + position, 
      extractPrefix(segment.text(), position),
      extractSuffix(segment.text(), position + pattern.size()), &output[i]);
  }
}

void findPatternInFileOrDirectory(const string& pattern, 
  const string& filenameOrDirectory, vector<string> *poutput) {

  auto& output = *poutput;
  output.clear();

  FileCrawler fc(filenameOrDirectory, pattern.size());

  while (!fc.isFinished()) {
    TextSegment segment;
    fc.getNextSegment(&segment);
    vector<string> curr;
    findPatternInSegment(pattern, segment, &curr);
    output.insert(output.end(), curr.begin(), curr.end());
  }
}