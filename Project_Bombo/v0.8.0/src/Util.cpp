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
                 << "OtherHeader:\nOtherSource:\nMakeFile: Makefile\nFunctions:\nClasses:\nIncluded Libraries:\nUsersEnabled: false\nUsers:\nBackupsEnabled: false\nBackupDestination:\n";
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
        if (line.find("Function:") == 0 && line.find(functionName) != string::npos)
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

string getDestinationDirectory(const string& metaDataFilePath)
{
    ifstream inFile(metaDataFilePath);
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

void encryptSourceCodeFiles()
{

}

void decryptSourceCodeFiles()
{

}