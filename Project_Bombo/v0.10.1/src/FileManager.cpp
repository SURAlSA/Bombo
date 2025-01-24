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
        cout << "Cannot open target file." << endl;
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
        cerr << "Cannot write to header file." << endl;
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
        cerr << "Unable to open log file." << endl;
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
        cout << "Error creating project file." << endl;
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
        cerr << "Error: Cannot open metadata file." << endl;
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
        cerr << "Cannot open metadata file." << endl;
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
        cerr << "Cannot open metadata file." << endl;
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
        cerr << "Cannot open metadata file." << endl;
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
        cerr << "Cannot open metadata file." << endl;
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
        cerr << "Cannot open metadata file." << endl;
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
        cerr << "Unable to open file." << endl;
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
    cerr << "Error finding 'BackupDestination:' in the file." << endl;
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

