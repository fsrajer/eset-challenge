//-----------------------------------------------------------------------------
/**
* \file       Utils.h
* \author     Filip Srajer
* \date       July, 2017
* \brief      Utility i/o functions implemented using windows.h functions.
*
*/
//-----------------------------------------------------------------------------
#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

void writeFile(const string& filename, const string& text);
void deleteFile(const string& filename);

void createDir(const string& directory);
void deleteDir(const string& directory);

bool isFile(const string& filename);
bool isDir(const string& directory);

/// List all files and directories
void listAllFiles(const string& directory, vector<string> *filenames);