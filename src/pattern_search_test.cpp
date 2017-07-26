#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "pattern_search.h"

TEST_CASE("findPattern test") {
  string pattern(""), text("");
  vector<int> pos;

  SECTION("everything empty") {
    findPattern(pattern, text, &pos);
    REQUIRE(pos.empty());
  }

  SECTION("position called non-empty") {
    pos.resize(10);
    findPattern(pattern, text, &pos);
    REQUIRE(pos.empty());
  }

  text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do";

  SECTION("empty pattern") {
    findPattern(pattern, text, &pos);
    REQUIRE(pos.empty());
  }

  SECTION("one character") {
    pattern = ",";
    findPattern(pattern, text, &pos);
    REQUIRE(pos.size() == 2);
  }

  SECTION("multiple characters") {
    pattern = "ipsum dolor sit";
    findPattern(pattern, text, &pos);
    REQUIRE(pos.size() == 1);
    REQUIRE(pos[0] == 6);
  }

  SECTION("multiple characters not in text") {
    pattern = "ipsum dolor sitxxxxxxxxxxx";
    findPattern(pattern, text, &pos);
    REQUIRE(pos.empty());
  }
}