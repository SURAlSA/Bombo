#ifndef BOMBO_H
#define BOMBO_H

#include "Util.h"
#include "Generator.h"

// structs

// classes

class Bombo
{
    public:
        // ==== Parent function ====
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

        // ==== User Related Operations ====
        void enableUsers();
        void createUser();
        bool UsersEnabled();
        bool verifyUser(string &userName, string &Key);

        // ==== Bombo Personality Functions ====
        void greeting();
        void joke();

};

// variables

extern bool projectLoaded;

extern string projectName;

extern string userName;


// functions

#endif