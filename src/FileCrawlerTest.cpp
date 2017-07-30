#include <fstream>
#include <cstdio>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "FileCrawler.h"
#include "Utils.h"
#include "ProducerConsumerBuffer.h"
#include "TextSegment.h"
#include "PatternSearch.h"

TEST_CASE("read test") {

  string directory = "tmp-test-dir";
  ProducerConsumerBuffer<TextSegment> segments(100);
  string pattern = "abc";

  SECTION("non-existent dir") {

    FileCrawler fc(pattern.size(), &segments);
    fc.startReaderWorker(directory);

    fc.join();

    TextSegment dummy;
    CHECK(segments.removeItem(&dummy) == 
      ProducerConsumerBufferSignal::SHUTDOWN);
    CHECK(dummy.textSize() == 0);
  }

  SECTION("non-existent dir") {
    string text;
    text.resize(PatternSearch::cMaxSegmentSize * 2, 'a');
    text[PatternSearch::cMaxSegmentSize / 2] = 'b';
    text[PatternSearch::cMaxSegmentSize] = 'x';

    CHECK_NOTHROW(createDir(directory));
    string filename = directory + "/tmp-test-file1.txt";
    CHECK_NOTHROW(writeFile(filename, text));

    FileCrawler fc(pattern.size(), &segments);
    fc.startReaderWorker(directory);

    fc.join();

    TextSegment segment;
    CHECK(segments.removeItem(&segment) ==
      ProducerConsumerBufferSignal::OK);
    CHECK(segments.removeItem(&segment) ==
      ProducerConsumerBufferSignal::OK);
    CHECK(segment.find(text.substr(
      PatternSearch::cMaxSegmentSize - pattern.size() + 1,
      PatternSearch::cMaxSegmentSize), 0) == 0);

    CHECK(segments.removeItem(&segment) ==
      ProducerConsumerBufferSignal::OK);
    CHECK(segment.find(
      text.substr(0, PatternSearch::cMaxSegmentSize), 0) == 0);

    CHECK(segments.removeItem(&segment) ==
      ProducerConsumerBufferSignal::SHUTDOWN);

    CHECK_NOTHROW(deleteFile(filename));
    CHECK_NOTHROW(deleteDir(directory));
  }
}