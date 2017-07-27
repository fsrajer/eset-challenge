#pragma once

#include <string>
#include <list>

using std::string;
using std::list;

class FileCrawler {
public:
  FileCrawler(const string& path);
  void getNextChunk();
  bool isFinished() const;
};
