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
                 << "OtherHeader:\nOtherSource:\nMakeFile: Makefile\nFunctions:\nClasses:\nIncluded Libraries:\n";
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