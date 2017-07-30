#include "FileCrawler.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <thread>

#include "Utils.h"

FileCrawler::FileCrawler(int patternLength,
  ProducerConsumerBuffer<TextSegment> *segments)
  : patternLength_(patternLength), segments_(segments), worker_(nullptr) {
}

void FileCrawler::startReaderWorker(const string& path) {
  pathsToProcess_.emplace_back(path, 0);
  worker_ = std::make_shared<std::thread>(
    &FileCrawler::readSegmentsWorker, this);
}

void FileCrawler::join() {
  if(worker_)
    worker_->join();
}

void FileCrawler::readSegmentsWorker() {
  while (!pathsToProcess_.empty()) {

    string path = pathsToProcess_.front();
    pathsToProcess_.pop_front();

    if (isDir(path)) {
      vector<string> candidates;
      listAllFiles(path, &candidates);
      for (const auto& candidate : candidates) {
        pathsToProcess_.emplace_back(candidate, 0);
      }
    }
    else if (isFile(path)) {
      readSegmentsFromFile(path);
    }
  }

  // Important! Let everyone know that we are done.
  segments_->shutdown();
}

void FileCrawler::readSegmentsFromFile(const string& filename) {

  std::ifstream in(filename);

  if (!in.is_open()) {
    std::cerr << "Could not open file:\n" << filename << "\n";
    return;
  }

  in.seekg(0, std::ios::end);
  long long fileLength = in.tellg();
  
  long long currOff = 0;
  while (currOff < fileLength) {

    // We need to take into account overlap
    // (otherwise, we wouldn't find all segments).
    currOff = std::max(0LL, currOff - patternLength_ + 1);

    TextSegment segment(filename, currOff);
    segment.readFromFile(fileLength, &in);
    segments_->insertItem(segment);

    currOff += segment.textSize();
  }

  in.close();
}