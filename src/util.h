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
#include <functional>
#include <unordered_map>
#include <termios.h>
#include <unistd.h>

using namespace std;

// functions

bool deleteWarning();

void enableRawMode(termios &oldTermios);

void disableRawMode(termios &oldTermios);

void processArrowKeyInput(char direction, string &command, vector<string> &commandHistory, int &historyIndex);

void handleInput(string &command, vector<string> &commandHistory, int &historyIndex);

string getCurrentTimeStamp();

string removeFromList(const string& line, const string& item);

int getRandomInt(int min, int max);

void throwObjectDoesntExistError(string object); // replaces old doesnt exist errors

// void throwFunctionDoesntExistError();

// void throwFileDoesntExistError();

void throwObjectAlreadyExistsError(string object); // replaces old already exists errors

// void throwFileAlreadyExistsError();

// void throwClassAlreadyExistsError();

// void throwFunctionAlreadyExistsError();

// void throwVariableAlreadyExistsError();

// void throwStructAlreadyExistsError();

void throwCantOpenObjectError(string object); // replaces old cant open errors

void throwCantFindMetaDataError();

void throwCantLoadProjectError(const string& projectName);

void throwVerificationUnsuccessfulError();

// void throwCantOpenMetaDataError();

void throwCantWriteToMetaDataError();

void throwUsersAlreadyEnabledError();

// void throwUserAlreadyExistsError();

void throwInvalidKeyError();

void throwBackupsNotEnabledError();

void throwBackupsAlreadyEnabledError();

// void throwCantOpenProjectFileError();

void throwSourceDirectoryDoesNotExistOrIsNotADirectoryError();

void throwDestinationDirectoryDoesNotExistError();

void throwMakeFileNotFoundError(const string& makeFilePath);

void throwFailedToRunMakeCleanCommandError();

// void throwUnableToOpenLogFileError();

void throwCantWriteToHeaderFileError();

// void throwCantOpenTargetFileError();

void throwCantCreateProjectFileError();

// void throwCantOpenFileError();

void throwCantFindBackupDestinationInFileError();

void throwFailedToDeleteFileError();

void throwFailedToRunMakeCommand();

void throwFailedToOpenHeaderFile(const string& headerFile);

void throwFailedToOpenMetaDataFile(const string& metadataFile);

void throwCantOpenFileAtFilePathForWritingError(const string& filePath);

void throwCantOpenFileAtFilePathError(const string& filePath);

void throwInvalidUserNameError();

void throwMustLoadProjectBeforeUsingThisFunction();

#endif
