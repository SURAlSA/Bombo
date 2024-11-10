#ifndef BOMBO_H
#define BOMBO_H

#include "Util.h"

// structs

// classes

class Bombo
{
    public:
        void handleCommand(string& command);

        void createProject();
        void compile();
        void clean();
        void createFile();
        void createFunction();
        void createClass();
        void createVariable();
        void createStruct();

        void loadProject();

        void promptForParameters(vector<pair<string, string>>& parameters);
        void promptForFunctions(vector<int>& functionStatus, vector<pair<string, string>>& functions, vector<vector<string>>& functionVariables);
        
        void determineFunctionStatus(vector<int>& functionSatus);

        string returnAllFunctions(string& className, vector<pair<string, string>>& parameters, vector<int>& functionStatus, vector<vector<string>> functionVariables, vector<pair<string, string>>& functions);
        string returnClassDefinition(string& className, vector<pair<string, string>>& parameters, vector<int>& functionStatus, vector<vector<string>> functionVariables, vector<pair<string, string>>& functions);
        string returnStruct(vector<pair<string, string>>& parameters, string& structName);


        string promptAndReturnFileName();
        string promptAndReturnClassName();
        string promptAndReturnFunctionName();        
        string promptAndReturnReturnType();
        string promptAndReturnVariableName();
        string promptAndReturnVariableType();
        string promptAndReturnStructName();
};

// variables

extern bool projectLoaded;

extern string projectName;

// functions

#endif