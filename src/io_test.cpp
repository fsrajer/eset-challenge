#include <fstream>
#include <cstdio>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "io.h"

TEST_CASE("readFile test") {
  string filename(""), text("");

  SECTION("no filename") {
    REQUIRE_THROWS(readFile(filename, &text));
  }

  SECTION("no filename non-empty text") {
    text = "abc";
    REQUIRE_THROWS(readFile(filename, &text));
  }

  SECTION("bad filename") {
    filename = "abc";
    REQUIRE_THROWS(readFile(filename, &text));
  }

  filename = "tmp-test-file.txt";

  SECTION("empty file") {
    REQUIRE_NOTHROW(writeFile(filename, ""));
    readFile(filename, &text);
    CHECK_NOTHROW(deleteFile(filename));
    REQUIRE(text.empty());
  }

  SECTION("good file") {
    string origText = "Lorem ipsum dolor sit amet,\n consectetur adipiscing";
    REQUIRE_NOTHROW(writeFile(filename, origText));
    readFile(filename, &text);
    CHECK_NOTHROW(deleteFile(filename));
    REQUIRE(text == origText);
  }
}