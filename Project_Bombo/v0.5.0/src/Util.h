#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <string>
#include <cctype>
#include <sstream>

using namespace std;

// functions

void buildFile(const string& fileLocation, const string& data);

void writeProjectFile(string& projectName);

void logEvent(const string& message, const string& projectName);

bool checkFileAlreadyExists(const string& metaDataFileAddress, const string& fileName);

bool checkFunctionAlreadyExists(const string& metaDataFileAddress, const string& functionName);

void addDataToTargetFile(const string& data, const string& targetFileAddress,
const string& lineBreak, const string& targetLocationInFile);
 
#endif