#include "CommandDispatcher.h"

// variables

// functions

void handleCommand(string& command) 
{
    ProjectManager ProjectManager;
    UserManager UserManager;
    BackupManager BackupManager;
    Bombo Bombo;

    // Define commands
    unordered_map<string, function<void()>> commandMap = 
    {
        {"help", [&]() {
            cout << "Common commands: \n\tcreateProject()\n\tloadProject()\n"
                 << "\tcreateFile()\n\tcreateFunction()\n\tcreateClass()\n"
                 << "\tcreateStruct()\n\tcreateVariable()\n";
        }},
        {"loadProject()", [&]() { ProjectManager.loadProject(); }},
        {"createProject()", [&]() { ProjectManager.createProject(); }},
        {"compile()", [&]() { if (projectLoaded) compile(); }},
        {"clean()", [&]() { if (projectLoaded) clean(); }},
        {"createFile()", [&]() { if (projectLoaded) ProjectManager.createFile(); }},
        {"createFunction()", [&]() { if (projectLoaded) ProjectManager.createFunction(); }},
        {"createClass()", [&]() { if (projectLoaded) ProjectManager.createClass(); }},
        {"createVariable()", [&]() { if (projectLoaded) ProjectManager.createVariable(); }},
        {"createStruct()", [&]() { if (projectLoaded) ProjectManager.createStruct(); }},
        {"createUser()", [&]() { if (projectLoaded) UserManager.createUser(); }},
        {"createBackup()", [&]() { if (projectLoaded) BackupManager.createBackup(); }},
        {"enableUsers()", [&]() { if (projectLoaded) UserManager.enableUsers(); }},
        {"changeDestinationDirectory()", [&]() { if (projectLoaded) BackupManager.changeDestinationDirectory(); }},
        {"enableBackups()", [&]() { if (projectLoaded) BackupManager.enableBackups(); }},
        {"addLibrary()", [&]() { if (projectLoaded) ProjectManager.addLibrary(); }},
        {"deleteFile()", [&]() { if (projectLoaded) ProjectManager.deleteFile(); }},
        {"setWorkEnvironment()", [&]() {if (projectLoaded) ProjectManager.setWorkEnvironment(); }},
    };

    // Check if the command is valid and handle it
    if (commandMap.find(command) != commandMap.end()) 
    {
        if (!projectLoaded && command != "help" && command != "createProject()" && command != "loadProject()") 
        {
            throwMustLoadProjectBeforeUsingThisFunction();
            ProjectManager.loadProject();

            if (projectLoaded)
            {
                commandMap[command]();
            }
        } 
        else 
        {
            commandMap[command]();
        }
    }

    int randomJoke = getRandomInt(0, 9);
    if (randomJoke <= 1) 
    {
        Bombo.joke();
    }
}

