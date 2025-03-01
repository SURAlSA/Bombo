#include "FileManager.h"

// variables

// functions

string trim(const string& str)
{
    size_t first = str.find_first_not_of(" \t");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}


void encryptFile(const string& filePath)
{
    string content = readFile(filePath);
    char xorConstant = 0x5A; // XOR constant

    for (char& c : content) 
    {
        c = c ^ xorConstant; // XOR encryption
        c = ~c;              // Invert binary
    }

    writeFile(filePath, content);
}


void writeFile(const string& filePath, const string& content)
{
    ofstream file(filePath);
    if (!file.is_open()) 
    {
        throw runtime_error("Could not write to file: " + filePath);
    }
    file << content;
}


string readFile(const string& filePath)
{
    ifstream file(filePath);
    if (!file.is_open()) 
    {
        throw runtime_error("Could not open file: " + filePath);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


void decryptSourceCodeFiles(const string& projectName)
{
    Checker Checker;
    string metadata = readFile(projectName + "/.PROJECT");

    // Extract metadata values
    const string sourceDir = Checker.getMetaDataValue(metadata, "SourceDir");
    const vector<string> mainFiles = Checker.getMetaDataValues(metadata, "MainFile");
    const vector<string> utilHeaders = Checker.getMetaDataValues(metadata, "UtilHeader");
    const vector<string> utilSources = Checker.getMetaDataValues(metadata, "UtilSource");
    const vector<string> otherHeaders = Checker.getMetaDataValues(metadata, "OtherHeader");
    const vector<string> otherSources = Checker.getMetaDataValues(metadata, "OtherSource");

    // Collect all file paths
    vector<string> filesToDecrypt;
    filesToDecrypt.insert(filesToDecrypt.end(), mainFiles.begin(), mainFiles.end());
    filesToDecrypt.insert(filesToDecrypt.end(), utilHeaders.begin(), utilHeaders.end());
    filesToDecrypt.insert(filesToDecrypt.end(), utilSources.begin(), utilSources.end());
    filesToDecrypt.insert(filesToDecrypt.end(), otherHeaders.begin(), otherHeaders.end());
    filesToDecrypt.insert(filesToDecrypt.end(), otherSources.begin(), otherSources.end());

    // Decrypt each file
    for (const string& relativePath : filesToDecrypt) 
    {
        const string fullPath = filesystem::path(projectName) / relativePath;
        decryptFile(fullPath);
        cout << "Decrypted: " << fullPath << endl;
    }
}


void decryptFile(const string& filePath)
{
    string content = readFile(filePath);
    char xorConstant = 0x5A; // XOR constant

    for (char& c : content) 
    {
        c = ~c;              // Invert binary
        c = c ^ xorConstant; // XOR decryption
    }

    writeFile(filePath, content);
}


void encryptSourceCodeFiles(const string& projectName)
{
    Checker Checker;
    string metadata = readFile(projectName + "/.PROJECT");

    // Extract metadata values
    const string sourceDir = Checker.getMetaDataValue(metadata, "SourceDir");
    const vector<string> mainFiles = Checker.getMetaDataValues(metadata, "MainFile");
    const vector<string> utilHeaders = Checker.getMetaDataValues(metadata, "UtilHeader");
    const vector<string> utilSources = Checker.getMetaDataValues(metadata, "UtilSource");
    const vector<string> otherHeaders = Checker.getMetaDataValues(metadata, "OtherHeader");
    const vector<string> otherSources = Checker.getMetaDataValues(metadata, "OtherSource");

    // Collect all file paths
    vector<string> filesToEncrypt;
    filesToEncrypt.insert(filesToEncrypt.end(), mainFiles.begin(), mainFiles.end());
    filesToEncrypt.insert(filesToEncrypt.end(), utilHeaders.begin(), utilHeaders.end());
    filesToEncrypt.insert(filesToEncrypt.end(), utilSources.begin(), utilSources.end());
    filesToEncrypt.insert(filesToEncrypt.end(), otherHeaders.begin(), otherHeaders.end());
    filesToEncrypt.insert(filesToEncrypt.end(), otherSources.begin(), otherSources.end());

    // Encrypt each file
    for (const string& relativePath : filesToEncrypt) 
    {
        const string fullPath = filesystem::path(projectName) / relativePath;
        encryptFile(fullPath);
        cout << "Encrypted: " << fullPath << endl;
    }
}


void addDataToTargetFile(const string& data, const string& targetFileAddress, const string& lineBreak, const string& targetLocationInFile)
{
    ifstream inFile(targetFileAddress);
    if (!inFile)
    {
        throwCantOpenObjectError("target file");
        return;
    }
    ostringstream buffer;
    string line;

    while (getline(inFile, line))
    {
        if (line.find(targetLocationInFile) == 0)
        {
            line += lineBreak + data;
        }
        buffer << line << "\n";
    }
    inFile.close();

    ofstream outFile(targetFileAddress);
    if (!outFile)
    {
        throwCantWriteToHeaderFileError();
        return;
    }

    outFile << buffer.str();
    outFile.close();
}


void logEvent(const string& message, const string& projectName)
{
    ofstream logFile(projectName + "/Bombo.log", ios::app); // append mode
    if (logFile.is_open())
    {
        time_t now = time(nullptr);

        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

        logFile << "[" << timestamp << "]" << message << endl;
        logFile.close();
    }
    else
    {
        throwCantOpenObjectError("log file");
    }
}


void writeProjectFile(string& projectName)
{
    ofstream metaData(projectName + "/.PROJECT");
    if (metaData.is_open())
    {
        metaData << "ProjectName: " << projectName << "\n"
                 << "SourceDir: src/\nObjectDir: obj/\n" 
                 << "MainFile: src/main.cpp\nUtilHeader: src/util.h\nUtilSource: src/util.cpp\n"
                 << "OtherHeader:\nOtherSource:\nMakeFile: Makefile\nFunctions:\nClasses:\nVariables:\nStructs:\nIncluded Libraries:\nUsersEnabled: false\nUsers:\nBackupsEnabled: false\nBackupDestination:\n";
        metaData.close();
        cout << "file /" << projectName << "/.PROJECT created." << endl;         
    }
    else
    {
        throwCantCreateProjectFileError();
    }
}


void buildFile(const string& fileLocation, const string& data)
{
    ofstream outFile(fileLocation);
    outFile << data;
    outFile.close();
    cout << "file /" << fileLocation << " created." << endl;
}


bool Checker::checkFileAlreadyExists(const string& metaDataFileAddress, const string& fileName)
{
    string headerFileName = "src/" + fileName + ".h";
    string sourceFilename = "src/" + fileName + ".cpp";
    ifstream inFile(metaDataFileAddress);
    if (!inFile)
    {
        throwCantOpenObjectError("metadata");
        return true;
    }

    string line;
    while(getline(inFile, line))
    {
        if (line.find("OtherHeader:") == 0 || line.find("OtherSource:") == 0)
        {
            if (line.find(headerFileName) != string::npos || line.find(sourceFilename) != string::npos)
            {
                return true;
            }
        }
    }

    inFile.close();
    return false;
}
bool Checker::checkFunctionAlreadyExists(const string& metaDataFileAddress, const string& functionName)
{
    ifstream inFile(metaDataFileAddress);
    if (!inFile)
    {
        throwCantOpenObjectError("metadata");
        return true;
    }

    string line;
    while (getline(inFile, line))
    {
        if (line.find("Functions:") == 0 && line.find(functionName) != string::npos)
        {
            return true;
        }
    }
    inFile.close();
    return false;
}
bool Checker::checkUserAlreadyExists(const string& metaDataFileAddress, const string& userName)
{
    ifstream inFile(metaDataFileAddress);
    if (!inFile)
    {
        throwCantOpenObjectError("metadata");
        return true;
    }

    string line;
    while (getline(inFile, line))
    {
        if (line.find("Users:") == 0 && line.find(userName) != string::npos)
        {
            return true;
        }
    }
    inFile.close();
    return false;
}
bool Checker::checkClassAlreadyExists(const string& metaDataFileAddress, const string& className)
{
    ifstream inFile(metaDataFileAddress);
    if (!inFile)
    {
        throwCantOpenObjectError("metadata");
        return true;
    }
    string line;
    while (getline(inFile, line))
    {
        if (line.find("Classes:") == 0 && line.find(className) != string::npos)
        {
            return true;
        }
    }
    inFile.close();
    return false;
}
bool Checker::checkVariableAlreadyExists(const string& metaDataFileAddress, const string& variableName)
{
    ifstream inFile(metaDataFileAddress);
    if (!inFile)
    {
        throwCantOpenObjectError("metadata");
        return true;
    }
    string line;
    while (getline(inFile, line))
    {
        if (line.find("Variables:") == 0 && line.find(variableName) != string::npos)
        {
            return true;
        }
    }
    inFile.close();
    return false;
}
bool Checker::checkStructAlreadyExists(const string& metaDataFileAddress, const string& structName)
{
    ifstream inFile(metaDataFileAddress);
    if (!inFile)
    {
        throwCantOpenObjectError("metadata");
        return true;
    }
    string line;
    while (getline(inFile, line))
    {
        if (line.find("Structs:") == 0 && line.find(structName) != string::npos)
        {
            return true;
        }
    }
    inFile.close();
    return false;
}
string Checker::getDestinationDirectory(const string& metaDataFileAddress)
{
    ifstream inFile(metaDataFileAddress);
    if (!inFile.is_open())
    {
        throwCantOpenObjectError("file");
        return "";
    }
    string marker = "BackupDestination: "; 
    string line;
    while (getline(inFile, line))
    {
        if (line.find(marker) == 0)
        {
            string address = line.substr(marker.length());
            address.erase(0, address.find_first_not_of("\t"));
            inFile.close();
            return address;
        }
    }

    inFile.close();
    throwCantFindBackupDestinationInFileError();
    return "";
}
string Checker::getMetaDataValue(const string& metaDataFileAddress, const string& key)
{
    size_t startPos = metaDataFileAddress.find(key + ":");
    if (startPos == string::npos) 
    {
        return "";
    }
    startPos += key.length() + 1; // Move past "key:"
    size_t endPos = metaDataFileAddress.find("\n", startPos);
    return metaDataFileAddress.substr(startPos, endPos - startPos);
}
vector<string> Checker::getMetaDataValues(const string& metaDataFileAddress, const string& key)
{
    size_t startPos = metaDataFileAddress.find(key + ":");
    if (startPos == string::npos) 
    {
        return {};
    }
    startPos += key.length() + 1; // Move past "key:"
    size_t endPos = metaDataFileAddress.find("\n", startPos);
    string line = metaDataFileAddress.substr(startPos, endPos - startPos);

    vector<string> values;
    stringstream ss(line);
    string value;
    while (ss >> value) 
    {
        values.push_back(value);
    }

    return values;
}

void parseHeaderFile(const string& headerFile, vector<string>& structs, vector<string>& classes, vector<string>& variables, vector<string>& functions)
{
    ifstream file(headerFile);

    if (!file.is_open()) 
    {
        throwFailedToOpenHeaderFile(headerFile);
        return;
    }

    string line;
    while (getline(file, line)) 
    {
        // Extract structs
        if (line.find("struct ") != string::npos) 
        {
            size_t pos = line.find("struct ") + 7;
            string structName = line.substr(pos, line.find("{") - pos);
            structs.push_back(trim(structName));
        }

        // Extract classes
        if (line.find("class ") != string::npos) 
        {
            size_t pos = line.find("class ") + 6;
            string className = line.substr(pos, line.find("{") - pos);
            classes.push_back(trim(className));
        }

        // Extract variables
        if (line.find("extern ") != string::npos && line.find(";") != string::npos) 
        {
            size_t start = line.find_last_of(" ");
            if (start != string::npos) 
            {
                string variableName = line.substr(start + 1, line.find(";") - start - 1);
                variables.push_back(trim(variableName));
            }
        }

        // Extract functions
        size_t parenPos = line.find("(");
        if (parenPos != string::npos && line.find(";") != string::npos) 
        {
            size_t start = line.find_last_of(" \t", parenPos - 1);
            if (start != string::npos) 
            {
                string functionName = line.substr(start + 1, parenPos - start - 1);
                functions.push_back(trim(functionName));
            }
        }
    }
    file.close();       
}

void updateMetadata(const string& metadataFile, const string& fileName, const vector<string>& structs,
                    const vector<string>& classes, const vector<string>& variables, const vector<string>& functions)
{
    fstream metadata(metadataFile, ios::in);
    if (!metadata.is_open()) 
    {
        throwFailedToOpenMetaDataFile(metadataFile);
        return;
    }

    stringstream updatedMetadata;
    string line;

    while (getline(metadata, line)) 
    {
        // Remove specific file references
        if (line.find("OtherHeader:") != string::npos) 
        {
            line = removeFromList(line, "src/" + fileName + ".h");
        } else if (line.find("OtherSource:") != string::npos) 
        {
            line = removeFromList(line, "src/" + fileName + ".cpp");
        }

        // Remove structs, classes, variables, and functions
        for (const auto& s : structs) line = removeFromList(line, s);
        for (const auto& c : classes) line = removeFromList(line, c);
        for (const auto& v : variables) line = removeFromList(line, v);
        for (const auto& f : functions) line = removeFromList(line, f);

        updatedMetadata << line << endl;
    }
    metadata.close();

    // Write updated metadata back to the file
    ofstream outMetadata(metadataFile, ios::out | ios::trunc);
    outMetadata << updatedMetadata.str();
    outMetadata.close();    
}

void deleteAFile(const string& filePath)
{
    if (remove(filePath.c_str()) == 0) 
    {
        cout << "Deleted file: " << filePath << endl;
    } 
    else 
    {
        throwFailedToDeleteFileError();
    }
}

void createDirectories()
{
    filesystem::create_directory(projectName);
    cout << "directory /" << projectName << " created." << endl;
    filesystem::create_directory(projectName + "/src");
    cout << "directory /" << projectName << "/src created." << endl;  
    filesystem::create_directory(projectName + "/obj");
    cout << "directory /" << projectName << "/obj created." << endl;
}

void constructFile(const string& fileName)
{
    string str = fileName;
    ofstream newHeaderFile(projectName + "/src/" + fileName + ".h");
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    newHeaderFile << "#ifndef " << str << "_H\n#define " << str << "_H\n\n"
                << "#include \"util.h\"\n\n// structs\n\n// classes\n\n// variables\n\n// functions\n\n"
                << "#endif";
    newHeaderFile.close();
    cout << "file /" << projectName << "/src/" << fileName << ".h created." << endl;
    
    // construct the source file
    ofstream newSourceFile(projectName + "/src/" + fileName + ".cpp");
    newSourceFile << "#include " << "\"" << fileName << ".h\"\n\n// variables\n\n// functions";
    newSourceFile.close();
    cout << "file /" << projectName << "/src/" << fileName << ".cpp created." << endl;
}

void updateVariableMetadata(const string& metadataFile, const string& variableName)
{
    fstream metadata(metadataFile, ios::in);
    if (!metadata.is_open()) 
    {
        throwFailedToOpenMetaDataFile(metadataFile);
        return;
    }

    stringstream updatedMetadata;
    string line;

    while (getline(metadata, line)) 
    {
        if (line.find("Variables:") != string::npos)
        {
            line = removeFromList(line, variableName);
        }
        updatedMetadata << line << endl;
    }

    metadata.close();

    // Write updated metadata back to the file
    ofstream outMetadata(metadataFile, ios::out | ios::trunc);
    outMetadata << updatedMetadata.str();
    outMetadata.close();    
}

void removeVariableFromFile(const string& filePath, const string& variableName)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        throwFailedToOpenHeaderFile(filePath);
        return;
    }

    stringstream updatedContent;
    string line;
    bool insideVariable = false;

    while (getline(file, line))
    {
        if (line.find(variableName) != string::npos)
        {
            insideVariable = true; // Start ignoring lines
        }

        if (!insideVariable)
        {
            updatedContent << line << endl;
        }

        if (insideVariable && line.find(";") != string::npos)
        {
            insideVariable = false; // End function deletion
        }
    }

    file.close();

    ofstream outFile(filePath, ios::out | ios::trunc);
    outFile << updatedContent.str();
    outFile.close();

    cout << "Deleted Class " << variableName << " from file " << filePath << endl;    
}

void updateClassMetadata(const string& metadataFile, const string& className)
{
    fstream metadata(metadataFile, ios::in);
    if (!metadata.is_open()) 
    {
        throwFailedToOpenMetaDataFile(metadataFile);
        return;
    }

    stringstream updatedMetadata;
    string line;

    while (getline(metadata, line)) 
    {
        if (line.find("Classes:") != string::npos)
        {
            line = removeFromList(line, className);
        }
        updatedMetadata << line << endl;
    }

    metadata.close();

    // Write updated metadata back to the file
    ofstream outMetadata(metadataFile, ios::out | ios::trunc);
    outMetadata << updatedMetadata.str();
    outMetadata.close();    
}

void removeClassFromFile(const string& filePath, const string& className)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        throwFailedToOpenHeaderFile(filePath);
        return;
    }

    stringstream updatedContent;
    string line;
    bool insideClass = false;

    while (getline(file, line))
    {
        if (line.find("class " + className) != string::npos)
        {
            insideClass = true; // Start ignoring lines
        }

        if (!insideClass)
        {
            updatedContent << line << endl;
        }

        if (insideClass && line.find("};") != string::npos)
        {
            insideClass = false; // End function deletion
        }
    }

    file.close();

    ofstream outFile(filePath, ios::out | ios::trunc);
    outFile << updatedContent.str();
    outFile.close();

    cout << "Deleted Class " << className << " from file " << filePath << endl;    
}

void updateStructMetadata(const string& metadataFile, const string& structName)
{
    fstream metadata(metadataFile, ios::in);
    if (!metadata.is_open()) 
    {
        throwFailedToOpenMetaDataFile(metadataFile);
        return;
    }

    stringstream updatedMetadata;
    string line;

    while (getline(metadata, line)) 
    {
        if (line.find("Structs:") != string::npos)
        {
            line = removeFromList(line, structName);
        }
        updatedMetadata << line << endl;
    }

    metadata.close();

    // Write updated metadata back to the file
    ofstream outMetadata(metadataFile, ios::out | ios::trunc);
    outMetadata << updatedMetadata.str();
    outMetadata.close();    
}

void removeStructFromFile(const string& filePath, const string& structName)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        throwFailedToOpenHeaderFile(filePath);
        return;
    }

    stringstream updatedContent;
    string line;
    bool insideStruct = false;

    while (getline(file, line))
    {
        if (line.find("struct " + structName) != string::npos)
        {
            insideStruct = true; // Start ignoring lines
        }

        if (!insideStruct)
        {
            updatedContent << line << endl;
        }

        if (insideStruct && line.find("};") != string::npos)
        {
            insideStruct = false; // End function deletion
        }
    }

    file.close();

    ofstream outFile(filePath, ios::out | ios::trunc);
    outFile << updatedContent.str();
    outFile.close();

    cout << "Deleted Struct " << structName << " from file " << filePath << endl;    
}

void removeFunctionFromFile(const string& filePath, const string& functionName)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        throwFailedToOpenHeaderFile(filePath);
        return;
    }

    stringstream updatedContent;
    string line;
    bool insideFunction = false;

    while (getline(file, line))
    {
        if (line.find(functionName + "(") != string::npos)
        {
            insideFunction = true; // Start ignoring lines
        }

        if (!insideFunction)
        {
            updatedContent << line << endl;
        }

        if (insideFunction && line.find("}") != string::npos || insideFunction && line.find(");") != string::npos)
        {
            insideFunction = false; // End function deletion
        }
    }

    file.close();

    ofstream outFile(filePath, ios::out | ios::trunc);
    outFile << updatedContent.str();
    outFile.close();

    cout << "Deleted function " << functionName << " from file " << filePath << endl;
}

void updateFunctionMetadata(const string& metadataFile, const string& functionName)
{
    fstream metadata(metadataFile, ios::in);
    if (!metadata.is_open()) 
    {
        throwFailedToOpenMetaDataFile(metadataFile);
        return;
    }

    stringstream updatedMetadata;
    string line;

    while (getline(metadata, line)) 
    {
        if (line.find("Functions:") != string::npos)
        {
            line = removeFromList(line, functionName);
        }
        updatedMetadata << line << endl;
    }

    metadata.close();

    // Write updated metadata back to the file
    ofstream outMetadata(metadataFile, ios::out | ios::trunc);
    outMetadata << updatedMetadata.str();
    outMetadata.close();    
}
vector<string> getHeaderFiles(const string& metadataFile, const string& projectName)
{
    vector<string> headerFiles;
    ifstream metadata(metadataFile);
    
    if (!metadata.is_open()) 
    {
        throwFailedToOpenMetaDataFile(metadataFile);
        return headerFiles;
    }

    string line;
    while (getline(metadata, line)) 
    {
        if (line.find("UtilHeader:") == 0)
        {
            string header = extractFilePath(line);
            headerFiles.push_back(projectName + "/" + header);
        }
        else if (line.find("OtherHeader:") == 0)
        {
            vector<string> files = extractFileList(line);
            for (const string& file : files)
            {
                headerFiles.push_back(projectName + "/" + file);
            }
        }
    }

    metadata.close();
    return headerFiles;
}
string extractFilePath(const string& line)
{
    size_t pos = line.find(":");
    if (pos != string::npos)
    {
        return trim(line.substr(pos + 1)); // Trim spaces and return file path
    }
    return "";
}
vector<string> extractFileList(const string& line)
{
    vector<string> files;
    size_t pos = line.find(":");
    if (pos != string::npos)
    {
        string fileList = line.substr(pos + 1);
        stringstream ss(fileList);
        string file;

        while (ss >> file)
        {
            files.push_back(trim(file)); // Trim spaces
        }
    }
    return files;
}

