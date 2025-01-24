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

// in all functions here the root folder is the folder that Bombo is located in

// Util Bible
// metaDataFileAddress = projectName + "/.PROJECT" 

int getRandomInt(int min, int max);

void buildFile(const string& fileLocation, const string& data);

void writeProjectFile(string& projectName);

void logEvent(const string& message, const string& projectName);

bool checkFileAlreadyExists(const string& metaDataFileAddress, const string& fileName);

bool checkFunctionAlreadyExists(const string& metaDataFileAddress, const string& functionName);

bool checkUserAlreadyExists(const string& metaDataFileAddress, const string& userName);

bool checkClassAlreadyExists(const string& metaDataFileAddress, const string& className);

bool checkVariableAlreadyExists(const string& metaDataFileAddress, const string& variableName);

bool checkStructAlreadyExists(const string& metaDataFileAddress, const string& structName);

void addDataToTargetFile(const string& data, const string& targetFileAddress,
const string& lineBreak, const string& targetLocationInFile);

void encryptKey(const string& filePath);

string decryptKey(const string& filePath);

string readKey(const string& filePath);

void replaceKey(const string& filePath, const string& Key);

string getDestinationDirectory(const string& metaDataFileAddress);

string getCurrentTimestamp();

void encryptSourceCodeFiles(const string& projectName);

void decryptFile(const string& filePath);

void decryptSourceCodeFiles(const string& projectName);

string readFile(const string& filePath);

void writeFile(const string& filePath, const string& content);

void encryptFile(const string& filePath); 

string getMetadataValue(const string& metadata, const string& key);

vector<string> getMetadataValues(const string& metadata, const string& key);
 
string trim(const string& str);


#endif