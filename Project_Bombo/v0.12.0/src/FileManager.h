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

string extractFilePath(const string& line);

vector<string> extractFileList(const string& line);

vector<string> getHeaderFiles(const string& metadataFile, const string& projectName);

void updateFunctionMetadata(const string& metadataFile, const string& functionName);

void removeFunctionFromFile(const string& filePath, const string& functionName);

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

void parseHeaderFile(const string& headerFile, vector<string>& structs, vector<string>& classes, vector<string>& variables, vector<string>& functions);

void updateMetadata(const string& metadataFile, const string& fileName, const vector<string>& structs,
                    const vector<string>& classes, const vector<string>& variables, const vector<string>& functions);

void deleteAFile(const string& filePath);

void createDirectories();

void constructFile(const string& fileName);

#endif
