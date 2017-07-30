#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "PatternSearch.h"
#include "Utils.h"
#include "TextSegment.h"

TEST_CASE("findPattern test") {
  
  string pattern("");
  string text = "abcd efg\n\t_abcd";
  string filename = "tmp-test-file.txt";
  REQUIRE_NOTHROW(writeFile(filename, text));

  vector<string> output;
  PatternSearch searchEngine;

  SECTION("empty pattern / output called non-empty") {
    output.resize(10);
    searchEngine.findPattern(pattern, filename, &output);
    CHECK(output.empty());
  }

  string goodOutput;

  SECTION("one character, multiple occurences") {
    pattern = "a";
    searchEngine.findPattern(pattern, filename, &output);
    CHECK(output.size() == 2);
    PatternSearch::formatResult(filename, 0, "", "bcd", &goodOutput);
    CHECK(output[0] == goodOutput);
    PatternSearch::formatResult(filename, 11, "\\n\\t_", "bcd", &goodOutput);
    CHECK(output[1] == goodOutput);
  }

  SECTION("multiple characters") {
    pattern = "efg\n\t_a";
    searchEngine.findPattern(pattern, filename, &output);
    CHECK(output.size() == 1);
    PatternSearch::formatResult(filename, 5, "cd ", "bcd", &goodOutput);
    CHECK(output[0] == goodOutput);
  }

  SECTION("multiple characters not in text") {
    pattern = "xyzxyz";
    searchEngine.findPattern(pattern, filename, &output);
    CHECK(output.empty());
  }

  SECTION("whitespace in the end of a file") {
    pattern = "efg\n\t_abc";
    searchEngine.findPattern(pattern, filename, &output);
    CHECK(output.size() == 1);
    PatternSearch::formatResult(filename, 5, "cd ", "d", &goodOutput);
    CHECK(output[0] == goodOutput);
  }

  REQUIRE_NOTHROW(deleteFile(filename));
}

TEST_CASE("prefix/suffix test") {

  string pattern("xyz");
  string text;
  text.resize(PatternSearch::cMaxSegmentSize - 3, 'a');

  string filename = "tmp-test-file.txt";
  vector<string> output;
  PatternSearch searchEngine;
  string expectedPrefix, expectedSuffix;
  int expectedOffset;

  SECTION("file beginning") {
    text = pattern + "ccc" + text;
    expectedOffset = 0;
    expectedPrefix = "";
    expectedSuffix = "ccc";
  }

  SECTION("file end") {
    text += ("ccc" + pattern);
    expectedOffset = PatternSearch::cMaxSegmentSize;
    expectedPrefix = "ccc";
    expectedSuffix = "";
  }

  SECTION("postfix borderline") {
    text += (pattern + "bbb");
    expectedOffset = PatternSearch::cMaxSegmentSize - 3;
    expectedPrefix = "aaa";
    expectedSuffix = "bbb";
  }

  SECTION("postfix borderline2") {
    text += ("cc" + pattern + "bbb");
    expectedOffset = PatternSearch::cMaxSegmentSize - 1;
    expectedPrefix = "acc";
    expectedSuffix = "bbb";
  }

  SECTION("prefix borderline") {
    text += ("b" + pattern + "ccc");
    expectedOffset = PatternSearch::cMaxSegmentSize - 2;
    expectedPrefix = "aab";
    expectedSuffix = "ccc";
  }

  REQUIRE_NOTHROW(writeFile(filename, text));

  string expectedOutput;
  searchEngine.findPattern(pattern, filename, &output);
  PatternSearch::formatResult(filename, expectedOffset,
    expectedPrefix, expectedSuffix, &expectedOutput);
  CHECK(output.size() == 1);
  CHECK(output[0] == expectedOutput);

  REQUIRE_NOTHROW(deleteFile(filename));
}