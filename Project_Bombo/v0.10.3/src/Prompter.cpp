#include "Prompter.h"

// variables

// functions

string promptAndReturnProjectName()
{
    string projectName;
    cout << "Enter a project name: ";
    getline(cin, projectName);
    return projectName;
}


string promptAndReturnStructName()
{
    string structName;
    cout << "Enter a struct name: ";
    getline(cin, structName);
    return structName;
}


string promptAndReturnVariableType()
{
    string variableType;
    cout << "Enter a variable type: ";
    getline(cin, variableType);
    return variableType;
}


string promptAndReturnVariableName()
{
    string variableName;
    cout << "Enter a variable name: ";
    getline(cin, variableName);
    return variableName;
}


string promptAndReturnClassName()
{
    string className;
    cout << "Enter a class name: ";
    getline(cin, className);
    return className;
}


string promptAndReturnReturnType()
{
    string returnType;
    cout << "Enter a return type: ";
    getline(cin, returnType);
    return returnType;
}


string promptAndReturnFunctionName()
{
    string functionName;
    cout << "Enter a function name: ";
    getline(cin, functionName);
    return functionName;
}


string promptAndReturnFileName()
{
    string fileName;
    cout << "Enter a filename: ";
    getline(cin, fileName);
    return fileName;
}


string returnStruct(vector<pair<string, string>>& parameters, string& structName)
{
    // creates and returns tbe struct definition for the header
    string structDefinition = "struct " + structName + "\n{\n";
    for (size_t i = 0; i < parameters.size(); i++)
    {
        structDefinition += "\t" + parameters[i].first + " " + parameters[i].second + ";\n";
    }
    return structDefinition += "};";
}


string returnClassDefinition(string& className, vector<pair<string, string>>& parameters, vector<int>& functionStatus, vector<vector<string>> functionVariables, vector<pair<string, string>>& functions)
{
    // creates and returns the class definition for the header
    string classDefinition = "class "+ className + "\n{\n\tprivate:\n";    
    for (size_t i = 0; i < parameters.size(); i++)
    {
        classDefinition += "\t\t" + parameters[i].first + " " + parameters[i].second + ";\n";
    }
    for (size_t j = 0; j < functionStatus.size(); j++)
    {
        if (functionStatus[j] == 0)
        {
            string functionVariableList;
            for (const auto& variable : functionVariables[j])
            {
                functionVariableList += variable;
            }
            classDefinition += "\t\t" + functions[j].first + " " + functions[j].second + "(" + functionVariableList + ");\n";
        }
    }
    classDefinition += "\tprotected:\n";
    for (size_t k = 0; k < functionStatus.size(); k++)
    {
        if (functionStatus[k] == 2)
        {
            string functionVariableList;
            for (const auto& variable : functionVariables[k])
            {
                functionVariableList += variable;
            }
            classDefinition += "\t\t" + functions[k].first + " " + functions[k].second + "(" + functionVariableList + ");\n";
        }
    }
    classDefinition += "\tpublic:\n";
    for (size_t l = 0; l < parameters.size(); l++)
    {
        classDefinition += "\t\t" + parameters[l].first + " get" + parameters[l].second + "();\n";
    }
    for (size_t m = 0; m < functionStatus.size(); m++)
    {
        if (functionStatus[m] == 1)
        {
            string functionVariableList;
            for (const auto& variable : functionVariables[m])
            {
                functionVariableList += variable;
            }
            classDefinition += "\t\t" + functions[m].first + " " + functions[m].second + "(" + functionVariableList + ");\n";
        }
    }
    return classDefinition += "};";
}


string returnAllFunctions(string& className, vector<pair<string, string>>& parameters, vector<int>& functionStatus, vector<vector<string>> functionVariables, vector<pair<string, string>>& functions)
{
    // creates and returns all the functions for the class
    string allFunctions;
    for (size_t n = 0; n < parameters.size(); n++)
    {
        allFunctions += parameters[n].first + " " + className + "::get" + parameters[n].second + "()\n{\n\treturn " + parameters[n].second + ";\n}\n"; 
    }
    for (size_t o = 0; o < functions.size(); o++)
    {
        string functionVariableList;
        for (const auto& variable : functionVariables[o])
        {
            functionVariableList += variable;
        }
        allFunctions += functions[o].first + " " + className + "::" + functions[o].second + "(" + functionVariableList + ")\n{\n// TODO: Implement function\n}\n";
    }
    return allFunctions;
}


void determineFunctionStatus(vector<int>& functionStatus)
{
    string status;
    cout << "Enter for (public, private or protected): ";
    getline(cin, status);
    if (status == "private")
    {
        functionStatus.emplace_back(0);
    }
    else if (status == "public")
    {
        functionStatus.emplace_back(1);
    }
    else if (status == "protected")
    {
        functionStatus.emplace_back(2);
    }
    else
    {
        functionStatus.emplace_back(0);
    }
}


void promptForFunctions(vector<int>& functionStatus, vector<pair<string, string>>& functions, vector<vector<string>>& functionVariables)
{
    // while loop collects functions and emplaces them into a vector containing pairs of strings
    while (true)
    {
        int counter = 0;
        string functionName;
        cout << "Enter a function name(or type 'done' to finish): ";
        getline(cin, functionName);
        
        if (functionName == "done")
        {
            break;
        }

        string returnType = promptAndReturnReturnType();
        
        functions.emplace_back(returnType, functionName);
        
        // function determines the status (public, protected, or private)
        determineFunctionStatus(functionStatus);
        
        string needVariable;
        cout << "Does this function need variables?(yes or no): ";
        getline(cin, needVariable);
        vector<string> variables;
        if (needVariable == "yes" || needVariable == "y")
        {
            string functionVariableType;
            // while loop collection variables
            while (true)
            {
                string functionVariableName;
                cout << "Enter a function variable type(enter 'done' to finish): ";
                getline(cin, functionVariableType);
                if (functionVariableType == "done")
                {
                    break;
                }
                cout << "Enter a function variable name: ";
                getline(cin, functionVariableName);
                if (counter == 0)
                {
                    variables.emplace_back(functionVariableType + " " + functionVariableName);
                }
                else
                {
                    variables.emplace_back(", " + functionVariableType + " " + functionVariableName);
                }
                counter++;
            }
        }
        functionVariables.emplace_back(variables);
    }
}


void promptForParameters(vector<pair<string, string>>& parameters)
{
    string variableName, variableType;

    // while loop that collects all the parameters and places them in the vector containing pairs of strings
    while (true)
    {
        cout << "Enter parameter type (or type 'done' to finish): ";
        getline(cin, variableType);
        if (variableType == "done")
        {
            break;
        }
        cout << "Enter parameter name: ";
        getline(cin, variableName);
        parameters.emplace_back(variableType, variableName);
    }
}

bool promptUserForDeletionConfirmation(const string& objectName)
{
    string confirmation;
    cout << "Please confirm yes or no if you wish to delete " << objectName << " (Warning: this will delete anything associated or stored in the object): ";
    getline(cin, confirmation);
    return (confirmation == "yes" || confirmation == "y");
}

bool promptErrorIfUtil(string objectName)
{
    if (objectName == "util")
    {
        cerr << "Error: Cannot perform this operation on util." << endl;
        return false;
    }
    return true;
}

bool promptErrorIfMain(string objectName)
{
    if (objectName == "main")
    {
        cerr << "Error: Cannot perform this operation on main." << endl;
        return false;
    }
    return true;
}