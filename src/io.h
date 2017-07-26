#pragma once

#include <string>

using std::string;

void readFile(const string& filename, string *text);

void writeFile(const string& filename, const string& text);

void deleteFile(const string& filename);