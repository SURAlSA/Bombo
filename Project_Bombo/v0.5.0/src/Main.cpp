#include "Util.h"
#include "Bombo.h"

int main()
{
    Bombo bombo;
    projectLoaded = false; // start out as false

    string command;
    while(true)
    {
        cout << "Bombo > ";
        getline(cin, command);
        if (command == "exit")
        {
            break;
        }
        bombo.handleCommand(command);
    }

    return 0;
}

// future features to add after v1 release
// 1. (optional feature)
// A user system in which a project maintains multiple users with their own generated passwords to login into the project
// A users changes is logged with their name asserting they made the change.
// 2. (optional feature)
// a security system in which the entire code base is encrypted unless a user logs in upon attempting to load the project. 
// 3.
// a system in which upon a project being loaded, Bombo searchs through the code base collecting all classes, variables, structs and functions
// and compares these to the same objects stored in Bombo's metadata and logs a Warning about newly added objects in the code
// and attempts to add these to the metadata
