#include "CommandDispatcher.h"
#include "FileManager.h"
#include "UserManager.h"
#include "util.h"

int main()
{
    UserManager UserManager;
    vector<string> commandHistory; // Stores last 5 commands
    int historyIndex = -1; // Tracks history navigation
    string command;
    while (true) 
    {
        handleInput(command, commandHistory, historyIndex);
        if (command == "exit") 
        {
            if (projectLoaded) 
            {
                if (UserManager.UsersEnabled()) 
                {
                    logEvent(userName + " logged out.", projectName);
                    encryptSourceCodeFiles(projectName);
                }
            }
            break;
        }
        handleCommand(command);
    }
    return 0;
}
// future additions
// enablePermissions() (adds an Admin account which has certain controls which normal users do not possess)
// mergeProjects() (able to grab multiple versions of a project and merge them into one) <- do this one last and only after every feature is working correctly
// aUserIsAlreadyLoggedIn() a value stored in the metadata which states whether a User is currently logged in to ensure that when another user attempts to access it doesnt encrypt the files
