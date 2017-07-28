#include "FileCrawler.h"

#include <iostream>
#include <fstream>

#include "io.h"

FileCrawler::FileCrawler(const string& path, int patternLength)
  : patternLength_(patternLength)
{
  pathsToProcess.emplace_back(path, 0);
  readSegments();
}

void FileCrawler::getNextSegment(TextSegment *seg)
{
  *seg = segmentsToProcess.front();
  segmentsToProcess.pop_front();
  
  if (segmentsToProcess.size() < 1) {
    readSegments();
  }
}

bool FileCrawler::isFinished() const
{
  return pathsToProcess.empty() && segmentsToProcess.empty();
}

void FileCrawler::readSegments() {
  while (segmentsToProcess.size() < cMaxSegmentsInMemory && 
    !pathsToProcess.empty()) {

    Path curr = pathsToProcess.front();
    pathsToProcess.pop_front();
    
    if (isDir(curr.path)) {
      vector<string> candidates;
      listAllFilesInDir(curr.path, &candidates);
      for (const auto& candidate : candidates) {
        pathsToProcess.emplace_back(candidate, 0);
      }
    }
    else if (isFile(curr.path)) {
      readSegments(curr);
    }
  }
}

void FileCrawler::readSegments(const Path& file) {

  std::ifstream in(file.path);

  if (!in.is_open()) {
    std::cerr << "Could not open file:\n" << file.path << "\n";
    return;
  }

  in.seekg(0, std::ios::end);
  int fileLength = in.tellg();
  int currOff = file.offset;

  while (segmentsToProcess.size() < cMaxSegmentsInMemory) {
    
    TextSegment segment(file.path, currOff);
    segment.readFromFile(fileLength, &in);
    segmentsToProcess.push_back(segment);

    currOff += segment.text().size();
    if (currOff >= fileLength) {
      break;
    }
    else {
      currOff = currOff - patternLength_ + 1;
    }
  }

  in.close();

  if(currOff < fileLength) {
    Path toBeContinued(file.path, currOff);
    pathsToProcess.push_front(toBeContinued);
  }
}

FileCrawler::Path::Path(const string& path, int offset)
  : path(path), offset(offset) {
}
