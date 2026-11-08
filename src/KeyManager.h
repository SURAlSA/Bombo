#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include "util.h"

// structs

// classes

// variables

// functions

void replaceKey(const string& filePath, const string& key);

string readKey(const string& filePath);

string decryptKey(const string& filePath);

void encryptKey(const string& filePath);

#endif
