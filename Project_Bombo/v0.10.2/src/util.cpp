#include "util.h"

// functions

string getCurrentTimeStamp()
{
    time_t t = time(nullptr);
    char buffer[100];
    if (strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", localtime(&t))) 
    {
        return string(buffer);
    }
    return "unknown";
}


string removeFromList(const string& line, const string& item)
{
    string result = line;
    size_t pos = result.find(item);
    if (pos != string::npos)
    {
        size_t start = (pos > 0 && result[pos - 1] == ',') ? pos - 1 : pos;
        size_t end = pos + item.length();
        if (end < result.length() && result[end] == ',') ++end;
        result.erase(start, end - start);
    }
    return result;
}


int getRandomInt(int min, int max)
{
    random_device rd;
    mt19937 generator(rd());

    uniform_int_distribution<int> distribution(min, max);

    return distribution(generator);
}

void throwFileDoesntExistError() {cerr << "Error: File doesnt exist." << endl;}

void throwFileAlreadyExistsError() {cerr << "Error: File already exists." << endl;}

void throwClassAlreadyExistsError() {cerr << "Error: Class already exists." << endl;}

void throwFunctionAlreadyExistsError() {cerr << "Error: Function already exists." << endl;}

void throwVariableAlreadyExistsError() {cerr << "Error: Variable already exists." << endl;}

void throwStructAlreadyExistsError() {cerr << "Error: Struct already exists." << endl;}

void throwCantFindMetaDataError() {cerr << "Error: Cant find meta data." << endl;}

void throwCantLoadProjectError(const string& projectName) {cerr << "Error: Failed to load project " << projectName << "." << endl;}

void throwVerificationUnsuccessfulError() {cerr << "Error: Verification Unsuccessful." << endl;}

void throwCantOpenMetaDataError() {cerr << "Error: Cant open metadata." << endl;}

void throwCantWriteToMetaDataError() {cerr << "Error: Cant write to metadata." << endl;}

void throwUsersAlreadyEnabledError() {cerr << "Error: Users already enabled." << endl;}

void throwUserAlreadyExistsError() {cerr << "Error: User already exists." << endl;}

void throwInvalidKeyError() {cerr << "Error: Invalid key." << endl;}

void throwBackupsNotEnabledError() {cerr << "Error: Backups not enabled." << endl;}

void throwBackupsAlreadyEnabledError() {cerr << "Error: Backups already enabled." << endl;}

void throwCantOpenProjectFileError() {cerr << "Error: Cant open project file." << endl;}

void throwSourceDirectoryDoesNotExistOrIsNotADirectoryError() {cerr << "Error: Source directory does not exist or is not a directory." << endl;}

void throwDestinationDirectoryDoesNotExistError() {cerr << "Error: Destination directory does not exist." << endl;}

void throwMakeFileNotFoundError(const string& makeFilePath) {cerr << "Error: Makefile not found in " << makeFilePath << "." << endl;}

void throwFailedToRunMakeCleanCommandError() {cerr << "Error: Failed to run make clean command." << endl;}

void throwUnableToOpenLogFileError() {cerr << "Error: Unable to open log file." << endl;}

void throwCantWriteToHeaderFileError() {cerr << "Error: Cant write to header file." << endl;}

void throwCantOpenTargetFileError() {cerr << "Error: Cant open target file." << endl;}

void throwCantCreateProjectFileError() {cerr << "Error: Cant create project file." << endl;}

void throwCantOpenFileError() {cerr << "Error: Cant open file." << endl;}

void throwCantFindBackupDestinationInFileError() {cerr << "Error: Cant find 'BackupDestination:' in file." << endl;}

void throwFailedToDeleteFileError() {cerr << "Error: Failed to delete file." << endl;}

void throwFailedToRunMakeCommand() {cerr << "Error: Failed to run make command." << endl;}

void throwFailedToOpenHeaderFile(const string& headerFile) {cerr << "Error: Failed to open header file " << headerFile << "." << endl;}

void throwFailedToOpenMetaDataFile(const string& metadataFile) {cerr << "Error: Failed to open metadata file " << metadataFile << "." << endl;}

void throwCantOpenFileAtFilePathForWritingError(const string& filePath) {cerr << "Error: Cant open file for writing at " << filePath << "." << endl;}

void throwCantOpenFileAtFilePathError(const string& filePath) {cerr << "Error: Cant open file at " << filePath << "." << endl;}

void throwInvalidUserNameError() {cerr << "Error: Invalid user name." << endl;}

void throwMustLoadProjectBeforeUsingThisFunction() {cerr << "Error: Must load project before using this function." << endl;}