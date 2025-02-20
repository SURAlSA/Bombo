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
// logComment() (adds a comment into the log file) <- do first, probably the easiest
// setWorkSpace() (allows a user to set a file to work on without needing to type in the filename everytime they want to do something, also allows user to 
// call functions numbered in a list instead of typing them out every time, include an ability to save different workspaces)
// not a function but allow a user to use previously called commands by pressing the up arrow in Bombo
