#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "PatternSearch.h"
#include "Utils.h"
#include "TextSegment.h"

TEST_CASE("findPattern test") {
  
  string pattern("");
  string text = "abcd efg\n__abcd";
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

  SECTION("one character") {
    pattern = "a";
    searchEngine.findPattern(pattern, filename, &output);
    CHECK(output.size() == 2);
    PatternSearch::formatResult(filename, 0, "", "bcd", &goodOutput);
    CHECK(output[0] == goodOutput);
    PatternSearch::formatResult(filename, 11, "\\n__", "bcd", &goodOutput);
    CHECK(output[1] == goodOutput);
  }

  SECTION("multiple characters") {
    pattern = "efg\n__a";
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
    pattern = "efg\n__abc";
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
  text.resize(TextSegment::cMaxSize - 3, 'a');
  text += (pattern + pattern);
  string filename = "tmp-test-file.txt";
  REQUIRE_NOTHROW(writeFile(filename, text));

  vector<string> output;
  PatternSearch searchEngine;
  string goodOutput;

  searchEngine.findPattern(pattern, filename, &output);
  CHECK(output.size() == 2);
  PatternSearch::formatResult(filename, TextSegment::cMaxSize,
    "xyz", "", &goodOutput);
  CHECK(output[0] == goodOutput);
  PatternSearch::formatResult(filename, TextSegment::cMaxSize - 3,
    "aaa", "xyz", &goodOutput);
  CHECK(output[1] == goodOutput);

  REQUIRE_NOTHROW(deleteFile(filename));
}