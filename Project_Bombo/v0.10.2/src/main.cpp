#include "CommandDispatcher.h"
#include "FileManager.h"
#include "UserManager.h"
#include "util.h"

int main()
{
    UserManager UserManager;
    string command;
    while(true)
    {
        cout << "Bombo > ";
        getline(cin, command);
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
