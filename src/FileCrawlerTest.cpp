#include <fstream>
#include <cstdio>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "FileCrawler.h"
#include "Utils.h"
#include "ProducerConsumerBuffer.h"
#include "TextSegment.h"

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
    CHECK(dummy.text().empty());
  }

  SECTION("non-existent dir") {
    string text;
    text.resize(TextSegment::cMaxSize * 2, 'a');
    text[TextSegment::cMaxSize / 2] = 'b';
    text[TextSegment::cMaxSize] = 'x';

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
    CHECK(segment.text() == text.substr(
      TextSegment::cMaxSize - pattern.size() + 1,
      TextSegment::cMaxSize));

    CHECK(segments.removeItem(&segment) ==
      ProducerConsumerBufferSignal::OK);
    CHECK(segment.text() == text.substr(0, TextSegment::cMaxSize));

    CHECK(segments.removeItem(&segment) ==
      ProducerConsumerBufferSignal::SHUTDOWN);

    CHECK_NOTHROW(deleteFile(filename));
    CHECK_NOTHROW(deleteDir(directory));
  }
}