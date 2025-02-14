#include "util.h"

// functions

void enableRawMode(termios &oldTermios)
{
    // Enables raw mode for terminal input, allowing single-character reading without echoing.
    struct termios newTermios;
    tcgetattr(STDIN_FILENO, &oldTermios);
    newTermios = oldTermios;
    newTermios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);
}

void disableRawMode(termios &oldTermios)
{
    // Restores the original terminal settings.
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTermios);
}

void processArrowKeyInput(char direction, string &command, vector<string> &commandHistory, int &historyIndex) 
{
    // Processes arrow key input for command history navigation.
    if (direction == 'A') 
    { // Up arrow pressed
        if (!commandHistory.empty() && historyIndex < (int)commandHistory.size() - 1) 
        {
            historyIndex++;
            command = commandHistory[commandHistory.size() - 1 - historyIndex];
        }
    } 
    else if (direction == 'B') 
    { 
        // Down arrow pressed
        if (historyIndex > 0) 
        {
            historyIndex--;
            command = commandHistory[commandHistory.size() - 1 - historyIndex];
        } 
        else 
        {
            historyIndex = -1;
            command.clear(); // Empty the command if at the last history index
        }
    }
    // Clear line and display updated command
    cout << "\rBombo > " << string(50, ' ') << "\rBombo > " << command << flush;
}

void handleInput(string &command, vector<string> &commandHistory, int &historyIndex)
{
    // Handles user input, including command history navigation with the up arrow key.
    termios oldTermios;
    enableRawMode(oldTermios);
    char c;
    command.clear();
    cout << "Bombo > " << flush;
    
    while (read(STDIN_FILENO, &c, 1) == 1) 
    {
        if (c == '\n') 
        { 
            // User pressed Enter
            cout << endl;
            if (!command.empty()) 
            {
                commandHistory.push_back(command);
                if (commandHistory.size() > 5) commandHistory.erase(commandHistory.begin());
                historyIndex = -1;
            }
            break;
        } 
        
        if (c == 127) 
        { 
            // Handle backspace
            if (!command.empty()) 
            {
                cout << "\b \b" << flush;
                command.pop_back();
            }
            continue;
        }
        
        if (c == 27) 
        { 
            // Handle escape sequences (arrow keys)
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) == 1 && read(STDIN_FILENO, &seq[1], 1) == 1) 
            {
                if (seq[0] == '[') 
                {
                    processArrowKeyInput(seq[1], command, commandHistory, historyIndex);
                }
            }
            continue;
        }
        
        // Regular character input
        command += c;
        cout << c << flush;
    }
    
    disableRawMode(oldTermios);
}

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