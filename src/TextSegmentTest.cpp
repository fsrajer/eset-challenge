#include <fstream>
#include <cstdio>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "TextSegment.h"
#include "Utils.h"

TEST_CASE("empty constructor test") {

  TextSegment seg;

  CHECK(seg.filename().empty());
  CHECK_NOTHROW(seg.textSize() == 0);
}

TEST_CASE("readFromFile test") {

  string filename = "tmp-test-file.txt";
  REQUIRE_NOTHROW(writeFile(filename, "abcdef"));
  int stringLength = 6;
  int offset = 2;

  TextSegment seg(filename, offset);

  CHECK(seg.filename() == filename);
  CHECK(seg.textSize() == 0);
  CHECK(seg.offset() == offset);

  SECTION("no args") {
    seg.readFromFile();
    CHECK(seg.find("cdef",0) == 0);
  }

  SECTION("pre-openned file") {
    std::ifstream in(filename);
    seg.readFromFile(stringLength, &in);
    CHECK(seg.find("cdef", 0) == 0);
  }

  CHECK_NOTHROW(deleteFile(filename));
}

TEST_CASE("readFromFile2 test") {

  string filename = "tmp-test-file.txt";
  string text = "a\nb\n\tc";
  REQUIRE_NOTHROW(writeFile(filename, text));
  int offset = 0;

  TextSegment seg(filename, offset);
  seg.readFromFile();

  CHECK(seg.find(text, 0) == 0);

  CHECK_NOTHROW(deleteFile(filename));
}

TEST_CASE("find test") {

  string pattern("xyz");
  string text;
  text.resize(TextSegment::cMaxSize - 3, 'a');

  string filename = "tmp-test-file.txt";
  int segmentOffset = 3;
  TextSegment segment(filename, segmentOffset);

  int findOffset;
  size_t expectedIndex;

  SECTION("pattern outside segment") {
    text = pattern + text;
    findOffset = 0;
    expectedIndex = string::npos;
  }

  SECTION("pattern outside segment2") {
    text = text + "aaaa" + pattern;
    findOffset = 0;
    expectedIndex = string::npos;
  }

  SECTION("find-offset after pattern") {
    text = "aaa" + pattern + text;
    findOffset = 1;
    expectedIndex = string::npos;
  }

  SECTION("pattern in segment") {
    text = text + pattern;
    findOffset = 0;
    expectedIndex = TextSegment::cMaxSize - 3 - 3;
  }

  REQUIRE_NOTHROW(writeFile(filename, text));
  segment.readFromFile();
  CHECK(segment.find(pattern, findOffset) == expectedIndex);

  REQUIRE_NOTHROW(deleteFile(filename));
}