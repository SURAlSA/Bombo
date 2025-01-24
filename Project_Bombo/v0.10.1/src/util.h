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

string getCurrentTimeStamp();

string removeFromList(const string& line, const string& item);

int getRandomInt(int min, int max);

#endif
