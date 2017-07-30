#include "PatternSearch.h"

#include <algorithm>
#include <iostream>

#include "FileCrawler.h"

PatternSearch::PatternSearch()
  : segments_(cMaxSegmentsInMemory) {

}

void PatternSearch::findPattern(const string& pattern,
  const string& path, vector<string> *poutput) {

  auto& output = *poutput;
  output.clear();

  FileCrawler fc(static_cast<int>(pattern.size()), &segments_);
  fc.startReaderWorker(path);

  vector<std::thread> workers;
  for (int i = 0; i < cNThreads; i++) {
    workers.emplace_back(&PatternSearch::findPatternWorker, this, 
      pattern, &output);
  }

  fc.join();
  for (auto& w : workers) {
    w.join();
  }
}

void PatternSearch::formatResult(const string& filename, long long position,
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

void PatternSearch::findPatternWorker(const string& pattern, 
  vector<string> *poutput) {

  auto& output = *poutput;

  TextSegment segment;
  auto signal = segments_.removeItem(&segment);
  while (signal == ProducerConsumerBufferSignal::OK) {

    vector<long long> positions;
    findPatternInText(pattern, segment, &positions);
    
    vector<string> currOutput(positions.size());
    for (size_t i = 0; i < positions.size(); i++) {
      long long position = positions[i];
      formatResult(segment.filename(), segment.offset() + position,
        segment.extractPrefix(position),
        segment.extractSuffix(position + pattern.size()),
        &currOutput[i]);
    }

    outputMutex_.lock();
    output.insert(output.end(), currOutput.begin(), currOutput.end());
    outputMutex_.unlock();

    signal = segments_.removeItem(&segment);
  }
}

void PatternSearch::findPatternInText(const string& pattern, 
  const TextSegment& text, vector<long long> *ppositions) {
  
  auto& positions = *ppositions;
  positions.clear();

  if (pattern.empty())
    return;

  size_t pos = text.find(pattern, 0);
  while (pos != string::npos) {
    positions.push_back(static_cast<long long>(pos));
    pos = text.find(pattern, pos+1);
  }
}
