#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "util.h"
#include "KeyManager.h"
#include "Variables.h"

// structs

// classes

class Checker
{
	private:
	protected:
	public:
		bool checkFileAlreadyExists(const string& metaDataFileAddress, const string& fileName);
		bool checkFunctionAlreadyExists(const string& metaDataFileAddress, const string& functionName);
		bool checkUserAlreadyExists(const string& metaDataFileAddress, const string& userName);
		bool checkClassAlreadyExists(const string& metaDataFileAddress, const string& className);
		bool checkVariableAlreadyExists(const string& metaDataFileAddress, const string& variableName);
		bool checkStructAlreadyExists(const string& metaDataFileAddress, const string& structName);
		string getDestinationDirectory(const string& metaDataFileAddress);
		string getMetaDataValue(const string& metaDataFileAddress, const string& key);
		vector<string> getMetaDataValues(const string& metaDataFileAddress, const string& key);
};

// variables

// functions

string trim(const string& str);

void encryptFile(const string& filePath);

void writeFile(const string& filePath, const string& content);

string readFile(const string& filePath);

void decryptSourceCodeFiles(const string& projectName);

void decryptFile(const string& filePath);

void encryptSourceCodeFiles(const string& projectName);

void addDataToTargetFile(const string& data, const string& targetFileAddress, const string& lineBreak, const string& targetLocationInFile);

void logEvent(const string& message, const string& projectName);

void writeProjectFile(string& projectName);

void buildFile(const string& fileLocation, const string& data);

#endif
