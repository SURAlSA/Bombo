#include "UserManager.h"

// variables


// functions

void UserManager::enableUsers()
{
    ProjectManager ProjectManager;
    // checks metadata
    string metaDataPath = projectName + "/.PROJECT";
    ifstream inFile(metaDataPath);
    if (!inFile)
    {
        throwCantOpenObjectError("metadata");
        return;
    }
    string line;
    bool found = false;
    stringstream fileContent;


    // if users isnt enabled, find in the metadata where it says false and change it to true
    if (!UsersEnabled())
    {
        while (getline(inFile, line))
        {
            if (line.find("UsersEnabled: false") != string::npos)
            {
                line = "UsersEnabled: true";
                found = true;
            }
            fileContent << line << '\n';
        }
        inFile.close();
        // attempt to output
        ofstream outFile(metaDataPath);
        if (!outFile)
        {
            throwCantWriteToMetaDataError();
            return;
        }
        outFile << fileContent.str();
        outFile.close();
        cout << "Users successfully enabled." << endl;

        // encrypt the code
        encryptSourceCodeFiles(projectName);

        // then run the createUser function
        createUser();

        // then prompt the user to reload the project (they will now have one user created and a key to use)
        ProjectManager.loadProject();
    }
    else if (UsersEnabled())
    {
        throwUsersAlreadyEnabledError();
    }
}
void UserManager::createUser()
{
    Checker Checker;
    // get a user name
    string newUserName;
    cout << "Enter a user name: ";
    getline(cin, newUserName);

    // check if the user doesnt already exist
    if (!Checker.checkUserAlreadyExists(projectName + "/.PROJECT", newUserName))
    {

        // generate a key
        char Key[16];
        Generate(Key, 16);

        // output the key 
        cout << "Key: ";
        OutputPassword(Key, 16);
        // set the last character as null or 0
        Key[16] = 0;
        // build a file containing the key
        string filePath = projectName + "/.temp" + newUserName + "Key";
        buildFile(filePath, Key);
        // encrypt the newly created key
        encryptKey(filePath);
        // add the new user to the metadata
        addDataToTargetFile(newUserName, projectName + "/.PROJECT", " ", "Users:");
        // log the creation
        logEvent("User " + newUserName + " created.", projectName);
    }
    else
    {
        throwObjectAlreadyExistsError("user");
    }
}
bool UserManager::UsersEnabled()
{
    string metaDataPath = projectName + "/.PROJECT";
    ifstream inFile(metaDataPath);
    if (!inFile)
    {
        throwCantOpenObjectError("metadata");
        return false;
    }
    // recursively search each line for where it says that the UsersEnabled = true and return true if its found
    string line;
    while (getline(inFile, line))
    {
        if (line.find("UsersEnabled: true") != string::npos)
        {
            inFile.close();
            return true;
        }
    }
    inFile.close();
    return false;  // Return false only if "UsersEnabled: true" was not found in the entire file
}
bool UserManager::verifyUser(string &userName, string &Key)
{
    Checker Checker;
    // prompt for user name and check if it exists
    cout << "Enter your user name: ";
    getline(cin, userName);
    if (Checker.checkUserAlreadyExists(projectName + "/.PROJECT", userName))
    {
        // prompt for key, check the stored key by decrypting it and then comparing the two
        cout << "Enter your Key: ";
        getline(cin, Key);
        string KeyLocation = projectName + "/.temp" + userName + "Key";
        
        string storedKey = decryptKey(KeyLocation);
        // if its a match return true
        if (storedKey == Key) // temporarily removing the generation of new key as it may be problematic with People remembering their keys
        {
           // cout << "Generating a new key..." << endl;
           // char Key[16];
           // Generate(Key, 16);
           // cout << "Key: ";
           // OutputPassword(Key, 16);
           // Key[16] = 0;
           // buildFile(KeyLocation, Key);
           // encryptKey(KeyLocation);
            return true;
        }
        else
        {
            throwInvalidKeyError();
            return false;
        }


    }
    else
    {
        throwInvalidUserNameError();
        return false;
    }
    return false;
}
void UserManager::deleteUser()
{
// TODO: Implement function
}
void UserManager::enablePermissions()
{
// TODO: Implement function
}

