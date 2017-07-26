#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "pattern_search.h"
#include "io.h"

TEST_CASE("findPatternInText test") {
  string pattern(""), text("");
  vector<int> pos;

  SECTION("everything empty") {
    findPatternInText(pattern, text, &pos);
    REQUIRE(pos.empty());
  }

  SECTION("position called non-empty") {
    pos.resize(10);
    findPatternInText(pattern, text, &pos);
    REQUIRE(pos.empty());
  }

  text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do";

  SECTION("empty pattern") {
    findPatternInText(pattern, text, &pos);
    REQUIRE(pos.empty());
  }

  SECTION("one character") {
    pattern = ",";
    findPatternInText(pattern, text, &pos);
    REQUIRE(pos.size() == 2);
  }

  SECTION("multiple characters") {
    pattern = "ipsum dolor sit";
    findPatternInText(pattern, text, &pos);
    REQUIRE(pos.size() == 1);
    REQUIRE(pos[0] == 6);
  }

  SECTION("multiple characters not in text") {
    pattern = "ipsum dolor sitxxxxxxxxxxx";
    findPatternInText(pattern, text, &pos);
    REQUIRE(pos.empty());
  }
}

TEST_CASE("prefix/suffix test") {
  string text("abcdefg\n"), prefix, suffix;
  int pos = 0;

  prefix = extractPrefix(text, pos);
  REQUIRE(prefix.empty());
  
  suffix = extractSuffix(text, pos);
  REQUIRE(suffix == "abc");

  pos = 1;
  prefix = extractPrefix(text, pos);
  REQUIRE(prefix == "a");

  pos = int(text.size());
  prefix = extractPrefix(text, pos);
  REQUIRE(prefix == "fg\n");

  suffix = extractSuffix(text, pos);
  REQUIRE(suffix.empty());

  pos -= 2;
  suffix = extractSuffix(text, pos);
  REQUIRE(suffix == "g\n");
}

TEST_CASE("findPatternInFile test") {
  string pattern(""), text(""), filename("");
  vector<string> output;

  SECTION("everything empty") {
    findPatternInFile(pattern, filename, &output);
    REQUIRE(output.empty());
  }

  SECTION("output called non-empty") {
    output.resize(10);
    findPatternInFile(pattern, filename, &output);
    REQUIRE(output.empty());
  }

  text = "abcd efg\n__abc";
  filename = "tmp-test-file.txt";
  REQUIRE_NOTHROW(writeFile(filename, text));

  SECTION("empty pattern") {
    findPatternInFile(pattern, filename, &output);
    CHECK(output.empty());
  }
  
  string goodOutput;

  SECTION("one character") {
    pattern = "a";
    findPatternInFile(pattern, filename, &output);
    CHECK(output.size() == 2);
    formatResult(filename, 0, "", "bcd", &goodOutput);
    CHECK(output[0] == goodOutput);
    formatResult(filename, 11, "\n__", "bc", &goodOutput);
    CHECK(output[1] == goodOutput);
  }

  REQUIRE_NOTHROW(deleteFile(filename));
}