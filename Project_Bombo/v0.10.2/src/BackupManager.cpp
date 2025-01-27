#include "BackupManager.h"

// variables

// functions

void BackupManager::enableBackups()
{
    string metaDataPath = projectName + "/.PROJECT";
    ifstream inFile(metaDataPath);
    if (!inFile)
    {
        throwCantOpenMetaDataError();
        return;
    }
    string line;
    bool found = false;
    stringstream fileContent;

    if (!BackupsEnabled())
    {
        while (getline(inFile, line))
        {
            if (line.find("BackupsEnabled: false") != string::npos)
            {
                line = "BackupsEnabled: true";
                found = true;
            }
            fileContent << line << '\n';
        }
        inFile.close();

        ofstream outFile(metaDataPath);
        if (!outFile)
        {
            throwCantWriteToMetaDataError();
            return;
        }
        outFile << fileContent.str();
        outFile.close();
        cout << "Backups successfully enabled." << endl;
        changeDestinationDirectory();
    }
    else if (BackupsEnabled())
    {
        cout << "Backups already Enabled." << endl;
    }
}
void BackupManager::changeDestinationDirectory()
{
    if (!BackupsEnabled())
    {
        cerr << "Backups not enabled." << endl;
        return;
    }
    string destinationDirectory;
    cout << "Enter a Destination Directory(Absolute Path): ";
    getline(cin, destinationDirectory);

    ifstream inFile(projectName + "/.PROJECT");

    if (!inFile.is_open())
    {
        throwCantOpenProjectFileError();
        return;
    }

    vector<string> lines;
    string line;
    string marker = "BackupDestination:";
    bool markerFound = false;

    while (getline(inFile, line))
    {
        if (line.find(marker) != string::npos)
        {
            line = marker + " " + destinationDirectory;
            markerFound = true;
        }
        lines.push_back(line);
    }
    inFile.close();

    ofstream outFile(projectName + "/.PROJECT");

    if (!outFile.is_open())
    {
        throwCantOpenProjectFileError();
        return;
    }

    for (const auto& modifiedLine : lines)
    {
        outFile << modifiedLine << "\n";
    }

    cout << "Destination Directory Successfully set." << endl;
}
void BackupManager::createBackup()
{
    Checker Checker;
    UserManager UserManager;
    if (BackupsEnabled())
    {
        string destinationDirectory = Checker.getDestinationDirectory(projectName + "/.PROJECT");
        if (destinationDirectory == "")
        {
            return;
        }
        string sourceDirectory = projectName;
        if (!filesystem::exists(sourceDirectory) || !filesystem::is_directory(sourceDirectory))
        {
            throwSourceDirectoryDoesNotExistOrIsNotADirectoryError();
            return;
        }
        filesystem::path absDestinationDirectory = filesystem::absolute(destinationDirectory);
        if (!filesystem::exists(absDestinationDirectory))
        {
            throwDestinationDirectoryDoesNotExistError();
            return;
        }
        // encrypts source code files so the backup has encrypted files
        if (UserManager.UsersEnabled())
        {
            encryptSourceCodeFiles(projectName);
        }
        string sourceName = filesystem::path(sourceDirectory).filename().string();
        string timeStamp = getCurrentTimeStamp();
        string backupDirectoryName = sourceName + "_" + timeStamp;

        filesystem::path backupDirectory = absDestinationDirectory / backupDirectoryName;

        filesystem::create_directories(backupDirectory);
        filesystem::copy(sourceDirectory, backupDirectory, filesystem::copy_options::recursive);

        cout << "Backup created successfully: " << backupDirectory << endl;
        
        if (UserManager.UsersEnabled())
        {
            decryptSourceCodeFiles(projectName);
        }
    }
    else if(!BackupsEnabled())
    {
        throwBackupsNotEnabledError();
    }
}
bool BackupManager::BackupsEnabled()
{
    string metaDataPath = projectName + "/.PROJECT";
    ifstream inFile(metaDataPath);
    if (!inFile)
    {
        throwCantOpenMetaDataError();
        return false;
    }

    string line;
    while (getline(inFile, line))
    {
        if (line.find("BackupsEnabled: true") != string::npos)
        {
            inFile.close();
            return true;
        }
    }
    inFile.close();
    return false;
}

