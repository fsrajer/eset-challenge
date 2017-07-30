#include <fstream>
#include <cstdio>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "TextSegment.h"
#include "Utils.h"

TEST_CASE("empty constructor test") {

  TextSegment seg;

  CHECK(seg.filename().empty());
  CHECK_NOTHROW(seg.text());
  CHECK(seg.text().empty());  
}

TEST_CASE("readFromFile test") {

  string filename = "tmp-test-file.txt";
  REQUIRE_NOTHROW(writeFile(filename, "abcdef"));
  int stringLength = 6;
  int offset = 2;

  TextSegment seg(filename, offset);

  CHECK(seg.filename() == filename);
  CHECK(seg.text().empty());
  CHECK(seg.offset() == offset);

  SECTION("no args") {
    seg.readFromFile();
    CHECK(seg.text() == "cdef");
  }

  SECTION("pre-openned file") {
    std::ifstream in(filename);
    seg.readFromFile(stringLength, &in);
    CHECK(seg.text() == "cdef");
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

  CHECK(seg.text() == text);

  CHECK_NOTHROW(deleteFile(filename));
}