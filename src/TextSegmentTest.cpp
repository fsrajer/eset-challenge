#include <fstream>
#include <cstdio>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "TextSegment.h"
#include "Utils.h"
#include "PatternSearch.h"

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
  text.resize(PatternSearch::cMaxSegmentSize - 3, 'a');

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
    expectedIndex = PatternSearch::cMaxSegmentSize - 3 - 3;
  }

  REQUIRE_NOTHROW(writeFile(filename, text));
  segment.readFromFile();
  CHECK(segment.find(pattern, findOffset) == expectedIndex);

  REQUIRE_NOTHROW(deleteFile(filename));
}

TEST_CASE("prefix/suffix test") {

  string text;
  text.resize(PatternSearch::cMaxSegmentSize - 3, 'a');

  string filename = "tmp-test-file.txt";
  int segmentOffset = 3;

  string expectedPrefix, expectedSuffix;
  int patternStartIdx, patternEndIdx;

  SECTION("prefix/suffix outside segment") {
    expectedPrefix = "xyz";
    expectedSuffix = "uvt";
    text = expectedPrefix + text + "aaa" + expectedSuffix;
    patternStartIdx = 0;
    patternEndIdx = PatternSearch::cMaxSegmentSize;
  }

  SECTION("prefix/suffix half in segment") {
    expectedPrefix = "xyz";
    expectedSuffix = "uvt";
    text = "a" + expectedPrefix + text + "a" + expectedSuffix;
    patternStartIdx = 1;
    patternEndIdx = PatternSearch::cMaxSegmentSize - 1;
  }

  SECTION("prefix/suffix in segment") {
    expectedPrefix = "xyz";
    expectedSuffix = "uvt";
    text[0] = 'x'; text[1] = 'y'; text[2] = 'z';
    text = "aaa" + text + expectedSuffix + "aaaaaa";
    patternStartIdx = 3;
    patternEndIdx = PatternSearch::cMaxSegmentSize - 3;
  }

  SECTION("borders") {
    segmentOffset = 0;
    expectedPrefix = "";
    expectedSuffix = "";
    patternStartIdx = 0;
    patternEndIdx = text.size();
  }

  REQUIRE_NOTHROW(writeFile(filename, text));
  TextSegment segment(filename, segmentOffset);
  segment.readFromFile();
  
  CHECK(segment.extractPrefix(patternStartIdx) == expectedPrefix);
  CHECK(segment.extractSuffix(patternEndIdx) == expectedSuffix);

  REQUIRE_NOTHROW(deleteFile(filename));
}