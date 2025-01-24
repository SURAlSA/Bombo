#ifndef BOMBO_H
#define BOMBO_H

#include "Util.h"
#include "Generator.h"

// structs

// classes

class Bombo
{
    public:
        // ==== Entry Point function ====
        void handleCommand(string& command);

        // ==== Generic Operations ====
        void createProject();
        void compile();
        void clean();
        void createFile();
        void createFunction();
        void createClass();
        void createVariable();
        void createStruct();
        void addLibrary();

        void loadProject();

        // ==== Generic Operation Helper Functions ====
        void promptForParameters(vector<pair<string, string>>& parameters);
        void promptForFunctions(vector<int>& functionStatus, vector<pair<string, string>>& functions, vector<vector<string>>& functionVariables);
        
        void determineFunctionStatus(vector<int>& functionStatus);

        string returnAllFunctions(string& className, vector<pair<string, string>>& parameters, vector<int>& functionStatus, vector<vector<string>> functionVariables, vector<pair<string, string>>& functions);
        string returnClassDefinition(string& className, vector<pair<string, string>>& parameters, vector<int>& functionStatus, vector<vector<string>> functionVariables, vector<pair<string, string>>& functions);
        string returnStruct(vector<pair<string, string>>& parameters, string& structName);

        string promptAndReturnFileName();
        string promptAndReturnFunctionName();
        string promptAndReturnReturnType();
        string promptAndReturnClassName();
        string promptAndReturnVariableName();
        string promptAndReturnVariableType();
        string promptAndReturnStructName();
        string promptAndReturnProjectName();

        // ==== User Related Operations ====
        void enableUsers();
        void createUser();
        bool UsersEnabled();
        bool verifyUser(string &userName, string &Key);

        // ==== Bombo Personality Functions ====
        void greeting();
        void joke();

        // ==== Backup Related Operations ====
        void enableBackups();
        void changeDestinationDirectory(); // The Backup Related Operations use the Absolute File Path
        void createBackup();
        bool BackupsEnabled();

        //  ==== Delete and Remove Operations ====
        void deleteFile(); // IN PROGRESS
        string removeFromList(const string& line, const string& item);
        void deleteFunction(); // TODO
        void deleteClass(); // TODO
        void deleteStruct(); // TODO
        void deleteVariable(); // TODO
        void deleteUser(); // TODO
        void removeLibrary(); // TODO

    // future additions
    // enablePermissions() (adds an Admin account which has certain controls which normal users do not possess)
    // mergeProjects() (able to grab multiple versions of a project and merge them into one) <- do this one last and only after every feature is working correctly
    // logComment() (adds a comment into the log file) <- do first, probably the easiest
};

// variables

extern bool projectLoaded;

extern string projectName;

extern string userName;


// functions

#endif