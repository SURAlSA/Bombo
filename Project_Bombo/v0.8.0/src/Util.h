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
#include <cstdio>
#include <memory>
#include <array>
#include <random>

using namespace std;

// functions

int getRandomInt(int min, int max);

void buildFile(const string& fileLocation, const string& data);

void writeProjectFile(string& projectName);

void logEvent(const string& message, const string& projectName);

bool checkFileAlreadyExists(const string& metaDataFileAddress, const string& fileName);

bool checkFunctionAlreadyExists(const string& metaDataFileAddress, const string& functionName);

bool checkUserAlreadyExists(const string& metaDataFileAddress, const string& userName);

void addDataToTargetFile(const string& data, const string& targetFileAddress,
const string& lineBreak, const string& targetLocationInFile);

void encryptKey(const string& filePath);

string decryptKey(const string& filePath);

string readKey(const string& filePath);

void replaceKey(const string& filePath, const string& Key);

string getDestinationDirectory(const string& metaDataFilePath);

string getCurrentTimestamp();

void encryptSourceCodeFiles(); // TODO

void decryptSourceCodeFiles(); // TODO

 
#endif