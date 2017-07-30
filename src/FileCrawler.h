//-----------------------------------------------------------------------------
/**
* \file       FileCrawler.h
* \author     Filip Srajer
* \date       July, 2017
* \brief      Creates a worker thread which reads in text segments in a buffer.
*
*/
//-----------------------------------------------------------------------------
#pragma once

#include <string>
#include <list>
#include <memory>

#include "TextSegment.h"
#include "ProducerConsumerBuffer.h"

using std::string;
using std::list;

class FileCrawler {
public:
  FileCrawler(int patternLength, 
    ProducerConsumerBuffer<TextSegment> *segments);

  /// This method has a corresponding join() method.
  void startReaderWorker(const string& path);
  void join();

private:
  void readSegmentsWorker();
  void readSegmentsFromFile(const string& filename);

  const int patternLength_;
  list<string> pathsToProcess_;
  ProducerConsumerBuffer<TextSegment> *segments_;
  std::shared_ptr<std::thread> worker_;
};
