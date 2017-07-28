#pragma once

#include <string>
#include <list>

#include "TextSegment.h"

using std::string;
using std::list;

class FileCrawler {
public:
  FileCrawler(const string& path, int patternLength);
  void getNextSegment(TextSegment *seg);
  bool isFinished() const;

private:
  class Path {
  public:
    Path(const string& path, int offset);
    string path;
    /// Marks where we should start reading a file (meaningless for dirs).
    int offset; 
  };

  void readSegments();
  void readSegments(const Path& file);

  const static int cMaxSegmentsInMemory = 2;
  const int patternLength_;
  list<TextSegment> segmentsToProcess;
  list<Path> pathsToProcess;
};
