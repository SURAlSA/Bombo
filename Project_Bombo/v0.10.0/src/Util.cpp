#include "Util.h"

// functions

void buildFile(const string& fileLocation, const string& data)
{
    ofstream outFile(fileLocation);
    outFile << data;
    outFile.close();
    cout << "file /" << fileLocation << " created." << endl;
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

bool checkFileAlreadyExists(const string& metaDataFileAddress, const string& fileName)
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

bool checkFunctionAlreadyExists(const string& metaDataFileAddress, const string& functionName)
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

bool checkClassAlreadyExists(const string& metaDataFileAddress, const string& className)
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

bool checkVariableAlreadyExists(const string& metaDataFileAddress, const string& variableName)
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

bool checkStructAlreadyExists(const string& metaDataFileAddress, const string& structName)
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

bool checkUserAlreadyExists(const string& metaDataFileAddress, const string& userName)
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

void addDataToTargetFile(const string& data, const string& targetFileAddress,
const string& lineBreak, const string& targetLocationInFile)
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

int getRandomInt(int min, int max)
{
    random_device rd;
    mt19937 generator(rd());

    uniform_int_distribution<int> distribution(min, max);

    return distribution(generator);
}

void encryptKey(const string& filePath)
{
    string Key = readKey(filePath);  // Read the key from file

    char xorConstant = 0x5A;  // XOR constant

    string encryptedKey = Key;  // Copy key to encryptedKey
    for (size_t i = 0; i < Key.size(); i++)
    {
        encryptedKey[i] = Key[i] ^ xorConstant;  // XOR with constant
        encryptedKey[i] = ~encryptedKey[i];      // Invert binary
    }
    
    replaceKey(filePath, encryptedKey);  // Save the encrypted key back to file
}

string decryptKey(const string& filePath)
{
    string Key = readKey(filePath);  // Read the key from file

    char xorConstant = 0x5A;  // XOR constant

    string decryptedKey = Key;  // Copy key to decryptedKey
    for (size_t i = 0; i < Key.size(); i++)
    {
        decryptedKey[i] = ~Key[i];  // Invert binary
        decryptedKey[i] = decryptedKey[i] ^ xorConstant;  // XOR with constant to decrypt
    }
    //cout << "Testing: " << decryptedKey << endl;
    return decryptedKey;
}

string readKey(const string& filePath)
{
    ifstream inFile(filePath);
    if (!inFile)
    {
        cerr << "Cannot open file at " << filePath << endl;
        return "";
    }

    string key;
    getline(inFile, key);  // Read the first line as the key
    inFile.close();

    return key;
}

void replaceKey(const string& filePath, const string& Key)
{
    ofstream outFile(filePath);
    if (outFile.is_open())
    {
        outFile << Key;  // Write the new key (encrypted or decrypted) to the file
        outFile.close();
    }
    else
    {
        cerr << "Cannot open file for writing at " << filePath << endl;
    }
}

string getDestinationDirectory(const string& metaDataFileAddress)
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

string getCurrentTimestamp() 
{
    time_t t = time(nullptr);
    char buffer[100];
    if (strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", localtime(&t))) {
        return string(buffer);
    }
    return "unknown";
}

void encryptSourceCodeFiles(const string& projectName) 
{
    string metadata = readFile(projectName + "/.PROJECT");

    // Extract metadata values
    const string sourceDir = getMetadataValue(metadata, "SourceDir");
    const vector<string> mainFiles = getMetadataValues(metadata, "MainFile");
    const vector<string> utilHeaders = getMetadataValues(metadata, "UtilHeader");
    const vector<string> utilSources = getMetadataValues(metadata, "UtilSource");
    const vector<string> otherHeaders = getMetadataValues(metadata, "OtherHeader");
    const vector<string> otherSources = getMetadataValues(metadata, "OtherSource");

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


// Function to decrypt a file's content
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

// Function to decrypt source code files
void decryptSourceCodeFiles(const string& projectName) 
{
    string metadata = readFile(projectName + "/.PROJECT");

    // Extract metadata values
    const string sourceDir = getMetadataValue(metadata, "SourceDir");
    const vector<string> mainFiles = getMetadataValues(metadata, "MainFile");
    const vector<string> utilHeaders = getMetadataValues(metadata, "UtilHeader");
    const vector<string> utilSources = getMetadataValues(metadata, "UtilSource");
    const vector<string> otherHeaders = getMetadataValues(metadata, "OtherHeader");
    const vector<string> otherSources = getMetadataValues(metadata, "OtherSource");

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

void writeFile(const string& filePath, const string& content) 
{
    ofstream file(filePath);
    if (!file.is_open()) 
    {
        throw runtime_error("Could not write to file: " + filePath);
    }
    file << content;
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

// Function to retrieve a single metadata value
string getMetadataValue(const string& metadata, const string& key) 
{
    size_t startPos = metadata.find(key + ":");
    if (startPos == string::npos) 
    {
        return "";
    }
    startPos += key.length() + 1; // Move past "key:"
    size_t endPos = metadata.find("\n", startPos);
    return metadata.substr(startPos, endPos - startPos);
}

// Function to retrieve multiple metadata values
vector<string> getMetadataValues(const string& metadata, const string& key) 
{
    size_t startPos = metadata.find(key + ":");
    if (startPos == string::npos) 
    {
        return {};
    }
    startPos += key.length() + 1; // Move past "key:"
    size_t endPos = metadata.find("\n", startPos);
    string line = metadata.substr(startPos, endPos - startPos);

    vector<string> values;
    stringstream ss(line);
    string value;
    while (ss >> value) 
    {
        values.push_back(value);
    }

    return values;
}

string trim(const string& str)
{
    size_t first = str.find_first_not_of(" \t");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}
