#ifndef PROMPTER_H
#define PROMPTER_H

#include "util.h"

// structs

// classes

// variables

// functions

string promptAndReturnProjectName();

string promptAndReturnStructName();

string promptAndReturnVariableType();

string promptAndReturnVariableName();

string promptAndReturnClassName();

string promptAndReturnReturnType();

string promptAndReturnFunctionName();

string promptAndReturnFileName();

string returnStruct(vector<pair<string, string>>& parameters, string& structName);

string returnClassDefinition(string& className, vector<pair<string, string>>& parameters, vector<int>& functionStatus, vector<vector<string>> functionVariables, vector<pair<string, string>>& functions);

string returnAllFunctions(string& className, vector<pair<string, string>>& parameters, vector<int>& functionStatus, vector<vector<string>> functionVariables, vector<pair<string, string>>& functions);

void determineFunctionStatus(vector<int>& functionStatus);

void promptForFunctions(vector<int>& functionStatus, vector<pair<string, string>>& functions, vector<vector<string>>& functionVariables);

void promptForParameters(vector<pair<string, string>>& parameters);

bool promptUserForDeletionConfirmation(const string& objectName);

bool promptErrorIfUtil(string objectName);

bool promptErrorIfMain(string objectName);

#endif
