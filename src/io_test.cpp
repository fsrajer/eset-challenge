#include <fstream>
#include <cstdio>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "io.h"

void writeFile(const string& filename, const string& text) {
  std::ofstream file(filename);
  REQUIRE(file.is_open());
  file << text;
  file.close();
}

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
    writeFile(filename, "");
    readFile(filename, &text);
    CHECK(std::remove(filename.c_str()) == 0);
    REQUIRE(text.empty());
  }

  SECTION("good file") {
    string origText = "Lorem ipsum dolor sit amet,\n consectetur adipiscing";
    writeFile(filename, origText);
    readFile(filename, &text);
    CHECK(std::remove(filename.c_str()) == 0);
    REQUIRE(text == origText);
  }
}