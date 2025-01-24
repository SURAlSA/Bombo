#include "CommandDispatcher.h"

// variables

// functions

void handleCommand(string& command)
{
    ProjectManager ProjectManager;
    UserManager UserManager;
    BackupManager BackupManager;
    Checker Checker;
    Bombo Bombo;

    if (command == "help")
    {
        cout << "Common commands: \n\tcreateProject()\n\tloadProject()\n" 
             << "\tcreateFile()\n\tcreateFunction()\n\tcreateClass()\n" 
             << "\tcreateStruct()\n\tcreateVariable()\n\taddInclude()\n" 
             << "\teditClass()\n\teditFunction()\n";
    }
    if (command == "createFile()" && !projectLoaded || command == "createFunction()" && !projectLoaded || command == "createClass()" && !projectLoaded
    || command == "createVariable()" && !projectLoaded || command == "createStruct()" && !projectLoaded || command == "compile()" && !projectLoaded
    || command == "clean()" && !projectLoaded || command == "enableUsers()" && !projectLoaded || command == "createUser()" && !projectLoaded
    || command == "enableBackups()" && !projectLoaded || command == "createBackup()" && !projectLoaded || command == "changeDestinationDirectory()" && !projectLoaded
    || command == "addLibrary()" && !projectLoaded || command == "deleteFile()" && !projectLoaded || command == "deleteFunction()" && !projectLoaded
    || command == "deleteClass()" && !projectLoaded || command == "deleteStruct()" && !projectLoaded || command == "deleteVariable()" && !projectLoaded
    || command == "deleteUser()" && !projectLoaded || command == "removeLibrary()" && !projectLoaded)
    {
        cerr << "Must load project before using this function." << endl;
        ProjectManager.loadProject();
    }
    if (command == "compile()" && projectLoaded)
    {
        compile();
    }
    if (command == "clean()" && projectLoaded)
    {
        clean();
    }
    if (command == "createFile()" && projectLoaded)
    {        
        ProjectManager.createFile();
    }  
    if (command == "loadProject()")
    {
        ProjectManager.loadProject();
    }
    if (command == "createFunction()" && projectLoaded)
    {
        ProjectManager.createFunction();
    }
    if (command == "createClass()" && projectLoaded)
    {
        ProjectManager.createClass();
    }
    if (command == "createVariable()" && projectLoaded)
    {
        ProjectManager.createVariable();
    }
    if (command == "createStruct()" && projectLoaded)
    {
        ProjectManager.createStruct();
    }
    if (command == "createUser()" && projectLoaded)
    {
        UserManager.createUser();
    }
    if (command == "createBackup()" && projectLoaded)
    {
        BackupManager.createBackup();
    }
    if (command == "enableUsers()" && projectLoaded)
    {
        UserManager.enableUsers();
    }
    if (command == "changeDestinationDirectory()" && projectLoaded)
    {
        BackupManager.changeDestinationDirectory();
    }
    if (command == "enableBackups()" && projectLoaded)
    {
        BackupManager.enableBackups();
    }
    if (command == "createProject()")
    {
        ProjectManager.createProject();
    }
    if (command == "addLibrary()" && projectLoaded)
    {
        ProjectManager.addLibrary();
    }
    if (command == "deleteFile()" && projectLoaded)
    {
        ProjectManager.deleteFile();
    }
    int randomJoke = getRandomInt(0, 9);
    if (randomJoke <= 1)
    {
        Bombo.joke();
    }
}

