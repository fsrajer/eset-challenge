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

TEST_CASE("isDir/isFile test") {
  
  string filename = "tmp-test-file.txt";
  CHECK(!isFile(filename));
  CHECK(!isDir(filename));

  REQUIRE_NOTHROW(writeFile(filename, ""));
  CHECK(isFile(filename));
  CHECK(!isDir(filename));
  CHECK_NOTHROW(deleteFile(filename));

  string directory = "tmp-test-dir";
  CHECK(!isFile(directory));
  CHECK(!isDir(directory));

  REQUIRE_NOTHROW(createDir(directory));
  CHECK(!isFile(directory));
  CHECK(isDir(directory));
  CHECK_NOTHROW(deleteDir(directory));
}