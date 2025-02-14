#include "ProjectManager.h"

// variables

// functions

void ProjectManager::createProject()
{
    // get project name
    projectName = promptAndReturnProjectName();


    // creates directories
    createDirectories();
    

    // build the Files
    buildFile(projectName + "/src/main.cpp", "#include \"util.h\"\n\nint main()\n{\n\n    return 0;\n}\n"); // main.cpp

    buildFile(projectName + "/src/util.h", "#ifndef UTIL_H\n#define UTIL_H\n\n#include <iostream>\n\nusing namespace std;\n\n// functions\n\n#endif\n"); // util.h

    buildFile(projectName + "/src/util.cpp", "#include \"util.h\"\n\n// functions\n"); // util.cpp

    // MakeFile
    buildFile(projectName + "/Makefile", "CXX = g++\n\nTARGET = " + projectName + 
    ".exe\n\nSRCS = src/main.cpp src/util.cpp\n\nOBJS = obj/main.o obj/util.o\n\nall: $(TARGET)\n\n$(TARGET): $(OBJS)\n\t$(CXX) -o $(TARGET) $(OBJS)\n\nobj/%.o: src/%.cpp\n\t$(CXX) -c $< -o $@\n\nclean:\n\trm -f $(TARGET) obj/*.o");
    
    buildFile(projectName + "/Bombo.log", ""); // bombo.log

    // Log the creation of the project
    logEvent("Project " + projectName + " created.", projectName);
    
    // create the Project File
    writeProjectFile(projectName);
    
    // set variable projectLoaded to true, this is used for decision making and signifies when a project is and isnt loaded
    projectLoaded = true;
}
void ProjectManager::createFile()
{
    Checker Checker;
    UserManager UserManager;
    // get file name
    string fileName = promptAndReturnFileName();
    
    // check if file already exists
    if (!Checker.checkFileAlreadyExists(projectName + "/.PROJECT", fileName))
    {
        // construct header file
        constructFile(fileName);
        

        // add the header to the metadata
        addDataToTargetFile("src/" + fileName + ".h", projectName + "/.PROJECT", " ", "OtherHeader:");
     
        // add compile information to the makefile regarding what to name the object
        addDataToTargetFile("obj/" + fileName + ".o", projectName + "/Makefile", " ", "OBJS =");
        
        // add the source file to the metadata
        addDataToTargetFile("src/" + fileName + ".cpp", projectName + "/.PROJECT", " ", "OtherSource:");
        
        // add the compile information of the source to the makefile
        addDataToTargetFile("src/" + fileName + ".cpp", projectName + "/Makefile", " ", "SRCS =");
        
        // log the creation of the file
        if (UserManager.UsersEnabled()) // if users enabled we log who created the file
        {
            logEvent(fileName + " file created by " + userName + ".", projectName);
        }
        else
        {
            logEvent(fileName + " file created.", projectName);
        }
        
    }
    else
    {
        throwFileAlreadyExistsError();
    }
}
void ProjectManager::createFunction()
{
    Checker Checker;
    UserManager UserManager;
    string fileName;

    if (inWorkEnvironment) // if in work environment, the fileName is stored in a seperate variable
    {
        fileName = workEnvironmentFileName;
    }
    else
    {
        fileName = promptAndReturnFileName();
    }

    // check if filename inputed is main (cannot add functions to main as it violates bombo format)
    if (!promptErrorIfMain(fileName))
    {
        return;
    }

    // get return type
    string returnType = promptAndReturnReturnType();
    // get function name
    string functionName = promptAndReturnFunctionName();

    if (!Checker.checkFunctionAlreadyExists(projectName + "/.PROJECT", functionName))
    {
        // vector to take parameters for the function
        vector<pair<string, string>> parameters;
        // function to collect parameters
        promptForParameters(parameters);
        
        // for loop to add commas inbetween the paramaters
        string parameterList;
        for (size_t i = 0; i < parameters.size(); i++)
        {
            parameterList += parameters[i].first + " " + parameters[i].second; // adds space between the variable type and the variable name
            if (i != parameters.size() - 1) // if not the last parameter
            {
                parameterList += ", "; // add comma
            }
        }
        // construct function declaration in string
        string functionDeclaration = returnType + " " + functionName + "(" + parameterList + ");";
        // construct function definition in string
        string functionDefinition = returnType + " " + functionName + "(" + parameterList + ")\n{\n    // TODO: Implement function\n}\n";
        
        // adding function declaration to the header file.
        addDataToTargetFile(functionDeclaration, projectName + "/src/" + fileName + ".h", "\n\n", "// functions");
        
        // adding function definition to the source file
        addDataToTargetFile(functionDefinition, projectName + "/src/" + fileName + ".cpp", "\n\n", "// functions");
        
        // adding function to the metadata
        addDataToTargetFile(functionName, projectName + "/.PROJECT", " ", "Functions:");
        // output function success
        cout << "Function " << functionName << " created successfully." << endl;
        
        // log the creation of the function
        if (UserManager.UsersEnabled()) // if users enabled we log who created the function
        {
            logEvent("Function " + functionName + " created in " + fileName + " by " + userName + ".", projectName);
        }
        else
        {
            logEvent("Function " + functionName + " created in " + fileName + ".", projectName);
        }
    }
    else
    {
        throwFunctionAlreadyExistsError();
    }
    
}
void ProjectManager::createClass()
{
    Checker Checker;
    UserManager UserManager;
    string fileName;

    if (inWorkEnvironment) // if in work environment, the fileName is stored in a seperate variable
    {
        fileName = workEnvironmentFileName;
    }
    else
    {
        fileName = promptAndReturnFileName();
    }
    
    // cannot add class to main as it violates bombo format
    if (!promptErrorIfMain(fileName))
    {
        return;
    }
    // cannot add class to util as it violates bombo format
    if (!promptErrorIfUtil(fileName))
    {
        return;
    }

    // check file exists
    if (Checker.checkFileAlreadyExists(projectName + "/.PROJECT", fileName))
    {
        // get class name
        string className = promptAndReturnClassName();
        if (!Checker.checkClassAlreadyExists(projectName + "/.PROJECT", className))
        {
            // get parameters
            vector<pair<string, string>> parameters;
            promptForParameters(parameters);
            // vector to store status of functions
            vector<int> functionStatus;
            // vector of the functions
            vector<pair<string, string>> functions;
            // vector of the function variables
            vector<vector<string>> functionVariables;
            // prompt for functions
            promptForFunctions(functionStatus, functions, functionVariables);
            
            // get class definition
            string classDefinition = returnClassDefinition(className, parameters, functionStatus, functionVariables, functions);
            
            // get functions for that class
            string allFunctions = returnAllFunctions(className, parameters, functionStatus, functionVariables, functions);
            
            // adds class definition to the header
            addDataToTargetFile(classDefinition, projectName + "/src/" + fileName + ".h", "\n\n", "// classes");
            
            // adds functions to the source file
            addDataToTargetFile(allFunctions, projectName + "/src/" + fileName + ".cpp", "\n\n", "// functions");
            
            // adds the defined class to the metadata
            addDataToTargetFile(className, projectName + "/.PROJECT", " ", "Classes:");
            
            // log the creation of the class
            if (UserManager.UsersEnabled()) // if users enabled we log who created the class
            {
                logEvent("Class " + className + " created in " + fileName + " by " + userName + ".", projectName);
            }
            else
            {
                logEvent("Class " + className + " created in " + fileName + ".", projectName);
            }
            
            
            // output class successfully created
            cout << "Class " << className << " created successfully." << endl;
        }
        else
        {
            throwClassAlreadyExistsError();
        }
    }
    else
    {
        throwFileDoesntExistError();
    }
        
    
}
void ProjectManager::createVariable()
{
    Checker Checker;
    UserManager UserManager;

    string fileName;

    if (inWorkEnvironment) // if in work environment, the fileName is stored in a seperate variable
    {
        fileName = workEnvironmentFileName;
    }
    else
    {
        fileName = promptAndReturnFileName();
    }
    
    // cant add variable to either main or util as it violates bombo format
    if (!promptErrorIfMain(fileName))
    {
        return;
    }
    if (!promptErrorIfUtil(fileName))
    {
        return;
    }

    if (Checker.checkFileAlreadyExists(projectName + "/.PROJECT", fileName))
    {
        // get variable type
        string variableType = promptAndReturnVariableType();
                
        // get variable name
        string variableName = promptAndReturnVariableName();

        if (!Checker.checkVariableAlreadyExists(projectName + "/.PROJECT", variableName))
        {
            // add variable to source file
            addDataToTargetFile(variableType + " " + variableName + ";", projectName + "/src/" + fileName + ".cpp", "\n\n", "// variables");
                    
            // add variable to header file
            addDataToTargetFile("extern " + variableType + " " + variableName + ";", projectName + "/src/" + fileName + ".h", "\n\n", "// variables");

            // add variable to the metadata
            addDataToTargetFile(variableName, projectName + "/.PROJECT", " ", "Variables:");
                    
            // output successful creation
            cout << "Variable " << variableName << " created successfully." << endl;
                    
                    
            // log the creation of the variable
            if (UserManager.UsersEnabled()) // if users enabled we log who created the variable
            {
                logEvent("Variable " + variableName + " created in " + fileName + " by " + userName + ".", projectName);
            }
            else
            {
                logEvent("Variable " + variableName + " created in " + fileName + ".", projectName);
            }   
        }
        else
        {
            throwVariableAlreadyExistsError();
        }
    }
    else
    {
        throwFileDoesntExistError();
    }
}
void ProjectManager::createStruct()
{
    Checker Checker;
    UserManager UserManager;

    string fileName;

    if (inWorkEnvironment) // if in work environment, the fileName is stored in a seperate variable
    {
        fileName = workEnvironmentFileName;
    }
    else
    {
        fileName = promptAndReturnFileName();
    }

    // cant add variable to either main or util as it violates bombo format
    if (!promptErrorIfMain(fileName))
    {
        return;
    }

    if (!promptErrorIfUtil(fileName))
    {
        return;
    }

    if (Checker.checkFileAlreadyExists(projectName + "/.PROJECT", fileName))
    {
        // get struct name
        string structName = promptAndReturnStructName();

        if (!Checker.checkStructAlreadyExists(projectName + "/.PROJECT", structName))
        {    
            // vector for parameters
            vector<pair<string, string>> parameters;
            // get parameters
            promptForParameters(parameters);

            // get struct definition
            string structDefinition = returnStruct(parameters, structName);

            // add struct to header file
            addDataToTargetFile(structDefinition, projectName + "/src/" + fileName + ".h", "\n\n", "// structs");

            // add struct to the metadata
            addDataToTargetFile(structName, projectName + "/.PROJECT", " ", "Structs:");
                    
            // log the creation of the variable
            if (UserManager.UsersEnabled()) // if users enabled we log who created the variable
            {
                logEvent("Struct " + structName + " created in " + fileName + " by " + userName + ".", projectName);
            }
            else
            {
                logEvent("Struct " + structName + " created in " + fileName + ".", projectName);
            }       
            // output successful creation of struct
            cout << "Struct " << structName << " created successfully." << endl;
        }
        else
        {
            throwStructAlreadyExistsError();
        }
    }
    else
    {
        throwFileDoesntExistError();
    }
}
void ProjectManager::setWorkEnvironment()
{
    Checker Checker;
    string fileName = promptAndReturnFileName();

    if (!promptErrorIfMain(fileName))
    {
        return;
    }

    if (fileName == "util")
    {
        workEnvironmentFileName = fileName;
        enterWorkEnvironmentForUtil();
    }

    if (Checker.checkFileAlreadyExists(projectName + "/.PROJECT", fileName))
    {
        workEnvironmentFileName = fileName; // change the global variable so it can be accessed by necessary functions
        if (fileName != "util")
        {
            enterWorkEnvironment(fileName);
        }
    }

    inWorkEnvironment = false;
}
void ProjectManager::enterWorkEnvironment(string fileName)
{
    inWorkEnvironment = true;
    UserManager UserManager;
    string stringInput;

    if (UserManager.UsersEnabled())
    {
        logEvent(userName + " has entered work environment on " + fileName + ".", projectName);
    }
    else
    {
        logEvent("Entered work environment on " + fileName + ".", projectName);
    }

    while (true)
    {
        cout << "\nFile: " << fileName << "\n\n"; // display file, usable functions in this mode which are usable using the numbers
        cout << "1. createFunction()" << "\n"
             << "2. createVariable()" << "\n"
             << "3. createClass()" << "\n"
             << "4. createStruct()" << "\n"
             << "5. exit" << endl;

        cout << "> ";
        getline(cin, stringInput);
        int inputInteger = stoi(stringInput); // translate the string into an integer


        switch (inputInteger)
        {
            case 1:
                createFunction();
                break;
            case 2:
                createVariable();
                break;
            case 3:
                createClass();
                break;
            case 4:
                createStruct();
                break;
            case 5:
                break;
        }

        if (inputInteger == 5)
        {
            break;
        }

    }
}
void ProjectManager::enterWorkEnvironmentForUtil()
{
    inWorkEnvironment = true;
    UserManager UserManager;
    string stringInput;

    if (UserManager.UsersEnabled())
    {
        logEvent(userName + " has entered work environment on util.", projectName);
    }
    else
    {
        logEvent("Entered work environment on util.", projectName);
    }

    while (true)
    {
        cout << "\nFile: util" << "\n\n";
        cout << "1. createFunction()" << "\n"
             << "5. exit" << endl;

        cout << "> ";
        getline(cin, stringInput);
        int inputInteger = stoi(stringInput);
     
     
        switch (inputInteger)
        {
            case 1:
                createFunction();
                break;
            case 5:
                break;
        }

        if (inputInteger == 5)
        {
            break;
        }
    }
}
void ProjectManager::addLibrary()
{
    UserManager UserManager;

    string libraryName;
    cout << "Enter the library name to add (e.g., -l<library>): ";
    getline(cin, libraryName);

    if (libraryName.empty()) 
    {
        cout << "Library name cannot be empty." << endl;
        return;
    }

    // Update the .PROJECT metadata file
    string projectFilePath = projectName + "/.PROJECT";
    string targetLocationInProjectFile = "Included Libraries:";
    string lineBreak = " ";
    addDataToTargetFile(libraryName, projectFilePath, lineBreak, targetLocationInProjectFile);

    // Update the Makefile
    string makefilePath = projectName + "/Makefile";
    ifstream inFile(makefilePath);
    if (!inFile.is_open()) 
    {
        cerr << "Cannot open Makefile to update library." << endl;
        return;
    }

    ostringstream buffer;
    string line;
    bool foundTarget = false;

    // Modify linker line in the Makefile
    while (getline(inFile, line)) 
    {
        if (line.find("$(TARGET): $(OBJS)") != string::npos) 
        {
            foundTarget = true;
        } 
        else if (foundTarget && line.find("\t$(CXX) -o $(TARGET) $(OBJS)") != string::npos) 
        {
            size_t pos = line.find("$(OBJS)");
            if (pos != string::npos) 
            {
                line.insert(pos + 7, " " + libraryName); // Insert after $(OBJS)
            }
            foundTarget = false; // Stop modifying after linker line
        }
        buffer << line << "\n";
    }
    inFile.close();

    ofstream outFile(makefilePath);
    if (!outFile.is_open()) 
    {
        cerr << "Cannot write to Makefile." << endl;
        return;
    }

    outFile << buffer.str();
    outFile.close();

    // Log the addition of the library
    if (UserManager.UsersEnabled())
    {
        logEvent("Library " + libraryName + " added to project by " + userName + ".", projectName); 
    }
    else
    {
        logEvent("Library " + libraryName + " added to project.", projectName);
    }

    cout << "Library " << libraryName << " added to the project successfully." << endl;
}
void ProjectManager::loadProject()
{
    Bombo Bombo;
    UserManager UserManager;
    // ensure projectLoaded is false
    projectLoaded = false;

    // prompt for projectName
    projectName = promptAndReturnProjectName();
    
    
    string metaData = projectName + "/.PROJECT";
    string Key;

    // check if metadata exists
    if (filesystem::exists(metaData))
    {
        cout << "Meta data found." << endl;

        // useing the boolean function to check if users is not enabled
        if (!UserManager.UsersEnabled())
        {
            // set project as loaded
            projectLoaded = true;

            cout << "Project loaded." << endl;
            
            // log and greet the user
            logEvent("Project loaded.", projectName);
            Bombo.greeting();
        }
        else
        {
            // verify the user using the boolean function verifyUser
            if (UserManager.verifyUser(userName, Key))
            {
                // set project as loaded
                projectLoaded = true;

                // decrypt code
                decryptSourceCodeFiles(projectName);
                cout << "Project loaded." << endl;
                
                // log and greet the user
                logEvent("Project loaded by " + userName + ".", projectName);
                Bombo.greeting();
            }
            else
            {
                throwVerificationUnsuccessfulError();
            }
        }
    }
    else
    {
        throwCantFindMetaDataError();
        throwCantLoadProjectError(projectName);
    }
}
void ProjectManager::deleteFile()
{
    Checker Checker;
    string fileName = promptAndReturnFileName();

    // cannot delete util as it's required in all Bombo projects
    if (promptErrorIfUtil(fileName))
    {
        // cannot delete main as it's the entry point of all Bombo projects
        if (promptErrorIfMain(fileName))
        {
            // check if file exists
            if (Checker.checkFileAlreadyExists(projectName + "/.PROJECT", fileName))
            {
                // confirm User wants to delete the object selected
                if (promptUserForDeletionConfirmation(fileName))
                {
                    // Parse header file to extract associated objects
                    string headerFile = projectName + "/src/" + fileName + ".h";
                    string sourceFile = projectName + "/src/" + fileName + ".cpp";
                    vector<string> structs, classes, variables, functions;

                    parseHeaderFile(headerFile, structs, classes, variables, functions);

                    // update metadata
                    string metaDataFile = projectName + "/.PROJECT";
                    updateMetadata(metaDataFile, fileName, structs, classes, variables, functions);

                    // delete files
                    deleteAFile(headerFile);
                    deleteAFile(sourceFile);


                }
            }
            else
            {
                throwFileDoesntExistError();
            }
        }
    }
}
void ProjectManager::deleteFunction()
{
// TODO: Implement function
}
void ProjectManager::deleteClass()
{
// TODO: Implement function
}
void ProjectManager::deleteStruct()
{
// TODO: Implement function
}
void ProjectManager::deleteVariable()
{
// TODO: Implement function
}
void ProjectManager::removeLibrary()
{
// TODO: Implement function
}
void ProjectManager::logComment()
{
// TODO: Implement function
}
void ProjectManager::mergeProjects()
{
// TODO: Implement function
}

