#include "Bombo.h"

// variables

bool projectLoaded;

string projectName;

string userName;

// functions

// ==== Parent function ====
void Bombo::handleCommand(string& command)
{   
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
        loadProject();
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
        createFile();
    }  
    if (command == "loadProject()")
    {
        loadProject();
    }
    if (command == "createFunction()" && projectLoaded)
    {
        createFunction();
    }
    if (command == "createClass()" && projectLoaded)
    {
        createClass();
    }
    if (command == "createVariable()" && projectLoaded)
    {
        createVariable();
    }
    if (command == "createStruct()" && projectLoaded)
    {
        createStruct();
    }
    if (command == "createUser()" && projectLoaded)
    {
        createUser();
    }
    if (command == "createBackup()" && projectLoaded)
    {
        createBackup();
    }
    if (command == "enableUsers()" && projectLoaded)
    {
        enableUsers();
    }
    if (command == "changeDestinationDirectory()" && projectLoaded)
    {
        changeDestinationDirectory();
    }
    if (command == "enableBackups()" && projectLoaded)
    {
        enableBackups();
    }
    if (command == "createProject()")
    {
        createProject();
    }
    if (command == "addLibrary()" && projectLoaded)
    {
        addLibrary();
    }
    if (command == "deleteFile()")
    {
        deleteFile();
    }
    int randomJoke = getRandomInt(0, 9);
    if (randomJoke <= 1)
    {
        joke();
    }
}




// ==== Generic Operations ====
void Bombo::createProject()
{
    // get project name
    projectName = promptAndReturnProjectName();


    // creates directories
    filesystem::create_directory(projectName);
    cout << "directory /" << projectName << " created." << endl;
    filesystem::create_directory(projectName + "/src");
    cout << "directory /" << projectName << "/src created." << endl;  
    filesystem::create_directory(projectName + "/obj");
    cout << "directory /" << projectName << "/obj created." << endl;

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

void Bombo::compile()
{
    // Construct the path to the Makefile within the project directory
    filesystem::path makefilePath = filesystem::path(projectName) / "Makefile";

    // check if MakeFile exists
    if (!filesystem::exists(makefilePath)) 
    {
        cerr << "Error: Makefile not found in " << makefilePath << endl;
        return;
    }

    // construct the command
    string command = "make -C " + projectName + " 2>&1";
    array<char, 128> buffer; // buffer to hold output
    string result; // string to accumilate command output

    // Use a unique_ptr to manage the pipe opened by popen
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

    // check if pipe was successfully opened
    if (!pipe)
    {
        cerr << "Failed to run make command." << endl;
        return;
    }

    // Read the output of the make command line by line
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }

    // output the result
    cout << result << endl;
}

void Bombo::clean()
{
    // Construct the path to the Makefile within the project directory
    filesystem::path makefilePath = filesystem::path(projectName) / "Makefile";

    // check if MakeFile exists
    if (!filesystem::exists(makefilePath)) 
    {
        cerr << "Error: Makefile not found in " << makefilePath << endl;
        return;
    }

    // construct the command 
    string command = "make -C " + projectName + " clean 2>&1";
    array<char, 128> buffer; // buffer to hold output
    string result; // string to accumilate command output

    // use a unique_ptr to manage the pipe opened by popen
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

    // check if pipe was successfully opened
    if (!pipe)
    {
        cerr << "Failed to run make clean command." << endl;
        return;
    }

    // Read the output of the make command line by line
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }

    // output the result
    cout << result << endl;
}

void Bombo::createFile()
{
    // get file name
    string fileName = promptAndReturnFileName();
    
    // check if file already exists
    if (!checkFileAlreadyExists(projectName + "/.PROJECT", fileName))
    {
        // construct header file
        string str = fileName;
        ofstream newHeaderFile(projectName + "/src/" + fileName + ".h");
        transform(str.begin(), str.end(), str.begin(), ::toupper);
        newHeaderFile << "#ifndef " << str << "_H\n#define " << str << "_H\n\n"
                    << "#include \"util.h\"\n\n// structs\n\n// classes\n\n// variables\n\n// functions\n\n"
                    << "#endif";

        newHeaderFile.close();
        cout << "file /" << projectName << "/src/" << fileName << ".h created." << endl;

        // construct the source file
        ofstream newSourceFile(projectName + "/src/" + fileName + ".cpp");
        newSourceFile << "#include " << "\"" << fileName << ".h\"\n\n// variables\n\n// functions";
        newSourceFile.close();
        cout << "file /" << projectName << "/src/" << fileName << ".cpp created." << endl;

        // add the header to the metadata
        addDataToTargetFile("src/" + fileName + ".h", projectName + "/.PROJECT", " ", "OtherHeader:");
     
        // add compile information to the makefile regarding what to name the object
        addDataToTargetFile("obj/" + fileName + ".o", projectName + "/Makefile", " ", "OBJS =");
        
        // add the source file to the metadata
        addDataToTargetFile("src/" + fileName + ".cpp", projectName + "/.PROJECT", " ", "OtherSource:");
        
        // add the compile information of the source to the makefile
        addDataToTargetFile("src/" + fileName + ".cpp", projectName + "/Makefile", " ", "SRCS =");
        
        // log the creation of the file
        if (UsersEnabled()) // if users enabled we log who created the file
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
        cerr << "File Already Exists. File name must be different." << endl;
    }
}

void Bombo::createFunction()
{
    // get filename
    string fileName = promptAndReturnFileName();

    // check if filename inputed is main (cannot add functions to main as it violates bombo format)
    if (fileName == "main")
    {
        cerr << "Cannot add a function to main." << endl;
        return;
    }
    // check that file doesnt equal util
    if (fileName != "util")
    {
        // check if file doesnt exist or filename doesnt equal util (util always exists)
        if (!checkFileAlreadyExists(projectName + "/.PROJECT", fileName))
        {
            cerr << "File doesn't exist." << endl;
            return;
        }
    }

    // get return type
    string returnType = promptAndReturnReturnType();

    // get function name
    string functionName = promptAndReturnFunctionName();

    // check if function already exists (must be original)
    if (checkFunctionAlreadyExists(projectName + "/.PROJECT", functionName)) // FIXED and is working as expected
    {
        cerr << "Function name already exists. Name must be different." << endl;
        return;
    }

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
    if (UsersEnabled()) // if users enabled we log who created the function
    {
        logEvent("Function " + functionName + " created in " + fileName + " by " + userName + ".", projectName);
    }
    else
    {
        logEvent("Function " + functionName + " created in " + fileName + ".", projectName);
    }
    
}

void Bombo::createClass()
{
    // get the file name
    string fileName = promptAndReturnFileName();
    
    // cannot add class to main as it violates bombo format
    if (fileName == "main")
    {
        cerr << "Cannot add a class to main." << endl;
        return;
    }

    // cannot add class to util as it violates bombo format
    if (fileName == "util")
    {
        cerr << "Cannot add a class to util." << endl;
        return;
    }

    // check if file doesnt exist
    if (!checkFileAlreadyExists(projectName + "/.PROJECT", fileName))
    {
        cerr << "File doesn't exist." << endl;
        return;
    }

    // get class name
    string className = promptAndReturnClassName();

    // checks if class already exists
    if (checkClassAlreadyExists(projectName + "/.PROJECT", className))
    {
        cerr << "Class name already exists. Name must be different." << endl;
        return;
    }
    
    
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
    if (UsersEnabled()) // if users enabled we log who created the class
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

void Bombo::createVariable()
{
    // get file name
    string fileName = promptAndReturnFileName();
    
    // cant add variable to either main or util as it violates bombo format
    if (fileName == "main")
    {
        cerr << "Cannot add a variable to main." << endl;
        return;
    }
    if (fileName == "util")
    {
        cerr << "Cannot add a variable to util." << endl;
        return;
    }

    // check if file doesnt exist
    if (!checkFileAlreadyExists(projectName + "/.PROJECT", fileName))
    {
        cerr << "File doesn't exist." << endl;
        return;
    }

    // get variable type
    string variableType = promptAndReturnVariableType();
    
    // get variable name
    string variableName = promptAndReturnVariableName();

    // checks if Variable already exists
    if (checkVariableAlreadyExists(projectName + "/.PROJECT", variableName))
    {
        cerr << "Variable name already exists. Name must be different." << endl;
        return;
    }
    

    // add variable to source file
    addDataToTargetFile(variableType + " " + variableName + ";", projectName + "/src/" + fileName + ".cpp", "\n\n", "// variables");
    
    // add variable to header file
    addDataToTargetFile("extern " + variableType + " " + variableName + ";", projectName + "/src/" + fileName + ".h", "\n\n", "// variables");

    // add variable to the metadata
    addDataToTargetFile(variableName, projectName + "/.PROJECT", " ", "Variables:");
    
    // output successful creation
    cout << "Variable " << variableName << " created successfully." << endl;
    
    
    // log the creation of the variable
    if (UsersEnabled()) // if users enabled we log who created the variable
    {
        logEvent("Variable " + variableName + " created in " + fileName + " by " + userName + ".", projectName);
    }
    else
    {
        logEvent("Variable " + variableName + " created in " + fileName + ".", projectName);
    }    
}

void Bombo::createStruct()
{
    // get file name
    string fileName = promptAndReturnFileName();

    // cannot add struct to main or util as it violates bombo format
    if (fileName == "main")
    {
        cout << "Cant create struct in main." << endl;
        return;
    }
    if (fileName == "util")
    {
        cout << "Cant create struct in util." << endl;
        return;
    }

    // check if file doesnt exist
    if (!checkFileAlreadyExists(projectName + "/.PROJECT", fileName))
    {
        cerr << "File doesn't exist." << endl;
        return;
    }

    // get struct name
    string structName = promptAndReturnStructName();

    if (checkStructAlreadyExists(projectName + "/.PROJECT", structName)) // FIXED and is working as expected
    {
        cerr << "Struct name already exists. Name must be different." << endl;
        return;
    }

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
    if (UsersEnabled()) // if users enabled we log who created the variable
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

void Bombo::addLibrary()
{
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
    if (UsersEnabled())
    {
        logEvent("Library " + libraryName + " added to project by " + userName + ".", projectName); 
    }
    else
    {
        logEvent("Library " + libraryName + " added to project.", projectName);
    }

    cout << "Library " << libraryName << " added to the project successfully." << endl;

}




void Bombo::loadProject()
{
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
        if (!UsersEnabled())
        {
            // set project as loaded
            projectLoaded = true;

            cout << "Project loaded." << endl;
            
            // log and greet the user
            logEvent("Project loaded.", projectName);
            greeting();
        }
        else
        {
            // verify the user using the boolean function verifyUser
            if (verifyUser(userName, Key))
            {
                // set project as loaded
                projectLoaded = true;

                // decrypt code
                decryptSourceCodeFiles(projectName);
                cout << "Project loaded." << endl;
                
                // log and greet the user
                logEvent("Project loaded by " + userName + ".", projectName);
                greeting();
            }
            else
            {
                // output error
                cerr << "Verification unsuccessful." << endl;
                return;
            }
        }
    }
    else
    {
        cout << "Cant find meta data." << endl;
        cout << "Failed to load project " << projectName << "." << endl;
    }
}




// ==== Generic Operation Helper Functions ====
void Bombo::promptForParameters(vector<pair<string, string>>& parameters)
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

void Bombo::promptForFunctions(vector<int>& functionStatus, vector<pair<string, string>>& functions, vector<vector<string>>& functionVariables)
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




void Bombo::determineFunctionStatus(vector<int>& functionStatus)
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




string Bombo::returnAllFunctions(string& className, vector<pair<string, string>>& parameters, vector<int>& functionStatus, vector<vector<string>> functionVariables, vector<pair<string, string>>& functions)
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

string Bombo::returnClassDefinition(string& className, vector<pair<string, string>>& parameters, vector<int>& functionStatus, vector<vector<string>> functionVariables, vector<pair<string, string>>& functions)
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

string Bombo::returnStruct(vector<pair<string, string>>& parameters, string& structName)
{
    // creates and returns tbe struct definition for the header
    string structDefinition = "struct " + structName + "\n{\n";
    for (size_t i = 0; i < parameters.size(); i++)
    {
        structDefinition += "\t" + parameters[i].first + " " + parameters[i].second + ";\n";
    }
    return structDefinition += "};";
}



// all prompt and return functions are generic and self explanatory
string Bombo::promptAndReturnFileName()
{
    string fileName;
    cout << "Enter a filename: ";
    getline(cin, fileName);
    return fileName;
}

string Bombo::promptAndReturnFunctionName()
{
    string functionName;
    cout << "Enter a function name: ";
    getline(cin, functionName);
    return functionName;
}

string Bombo::promptAndReturnReturnType()
{
    string returnType;
    cout << "Enter a return type: ";
    getline(cin, returnType);
    return returnType;
}

string Bombo::promptAndReturnClassName()
{
    string className;
    cout << "Enter a class name: ";
    getline(cin, className);
    return className;
}

string Bombo::promptAndReturnVariableName()
{
    string variableName;
    cout << "Enter a variable name: ";
    getline(cin, variableName);
    return variableName;
}

string Bombo::promptAndReturnVariableType()
{
    string variableType;
    cout << "Enter a variable type: ";
    getline(cin, variableType);
    return variableType;
}

string Bombo::promptAndReturnStructName()
{
    string structName;
    cout << "Enter a struct name: ";
    getline(cin, structName);
    return structName;
}

string Bombo::promptAndReturnProjectName()
{
    string projectName;
    cout << "Enter a project name: ";
    getline(cin, projectName);
    return projectName;
}




// ==== User Related Operations ====
void Bombo::enableUsers()
{
    // checks metadata
    string metaDataPath = projectName + "/.PROJECT";
    ifstream inFile(metaDataPath);
    if (!inFile)
    {
        cerr << "Cannot open metadata file." << endl;
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
            cerr << "Cannot write to metadata file." << endl;
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
        loadProject();
    }
    else if (UsersEnabled())
    {
        cout << "Users already enabled." << endl;
    }
}

void Bombo::createUser()
{
    // get a user name
    string newUserName;
    cout << "Enter a user name: ";
    getline(cin, newUserName);

    // check if the user doesnt already exist
    if (!checkUserAlreadyExists(projectName + "/.PROJECT", newUserName))
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
        cerr << "User already exists." << endl;
    }
}

bool Bombo::UsersEnabled()
{
    string metaDataPath = projectName + "/.PROJECT";
    ifstream inFile(metaDataPath);
    if (!inFile)
    {
        cerr << "Cannot open metadata file." << endl;
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

bool Bombo::verifyUser(string &userName, string &Key)
{
    // prompt for user name and check if it exists
    cout << "Enter your user name: ";
    getline(cin, userName);
    if (checkUserAlreadyExists(projectName + "/.PROJECT", userName))
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
            cerr << "Invalid Key." << endl;
            return false;
        }


    }
    else
    {
        cerr << "Invalid user name." << endl;
        return false;
    }
    return false;
}




// ==== Bombo Personality Functions ====
void Bombo::greeting()
{
    string greetings[] = {"Hi ", "Hello ", "こんにちは ", "你好 "};
    string emoticons[] = {" :)", " :>", " :|", " (˶ᵔ ᵕ ᵔ˶)", " >ᴗ<", " (ᵕ—ᴗ—)", " (,,>﹏<,,)", " (˶˃⤙˂˶)"};
    int index = getRandomInt(0, 3);
    int index2 = getRandomInt(0, 7);
    if (UsersEnabled())
    {
        cout << greetings[index] << userName << emoticons[index2] << endl;
    }
    else
    {
        cout << greetings[index] << "whoever you are" << emoticons[index2] << endl;
    }
}

void Bombo::joke()
{
    string animal[] = {"Dog", "Cat", "Chicken", "Pig", "Cow", "Goat", "Crocodile", "Duck", "Bombo"};
    int animalIndex = getRandomInt(0, 8);
    string question[] = {"cross the road?", "chase the chicken?", "order lemonade?", "get water?", "forget its homework?", "forget its birthday?", "eat a cake?", "fight a kangaroo?",
    "fight a Bombo?", "commit treason?", "lite fire to a cows tail?"};
    int questionIndex = getRandomInt(0, 10);
    string response[] = {"To eat KFC on the otherside.", "To run from a capybara.", "Because it felt like it.", "Because it was funny.", "I dont know, do you?", "Because it was hungry.",
    "To get to the other side.", "To get a drink of water.", "Because it was not not not hungry.", "We will never know.", "To cross into the other universe.", 
    "Because it defecated itself in an Engadine Maccas."};
    int responseIndex = getRandomInt(0, 11);

    cout << "Why did the " << animal[animalIndex] << " " << question[questionIndex] << endl;
    cout << response[responseIndex] << endl;
}




// ==== Backup Related Operations ====
void Bombo::enableBackups()
{
    string metaDataPath = projectName + "/.PROJECT";
    ifstream inFile(metaDataPath);
    if (!inFile)
    {
        cerr << "Cannot open metadata file." << endl;
        return;
    }
    string line;
    bool found = false;
    stringstream fileContent;

    if (!BackupsEnabled())
    {
        while (getline(inFile, line))
        {
            if (line.find("BackupsEnabled: false") != string::npos)
            {
                line = "BackupsEnabled: true";
                found = true;
            }
            fileContent << line << '\n';
        }
        inFile.close();

        ofstream outFile(metaDataPath);
        if (!outFile)
        {
            cerr << "Cannot write to metadata file." << endl;
            return;
        }
        outFile << fileContent.str();
        outFile.close();
        cout << "Backups successfully enabled." << endl;
        changeDestinationDirectory();
    }
    else if (BackupsEnabled())
    {
        cout << "Backups already Enabled." << endl;
    }
}

void Bombo::changeDestinationDirectory()
{
    if (!BackupsEnabled())
    {
        cerr << "Backups not enabled." << endl;
        return;
    }
    string destinationDirectory;
    cout << "Enter a Destination Directory(Absolute Path): ";
    getline(cin, destinationDirectory);

    ifstream inFile(projectName + "/.PROJECT");

    if (!inFile.is_open())
    {
        cerr << "Error: Cannot open .PROJECT file." << endl;
        return;
    }

    vector<string> lines;
    string line;
    string marker = "BackupDestination:";
    bool markerFound = false;

    while (getline(inFile, line))
    {
        if (line.find(marker) != string::npos)
        {
            line = marker + " " + destinationDirectory;
            markerFound = true;
        }
        lines.push_back(line);
    }
    inFile.close();

    ofstream outFile(projectName + "/.PROJECT");

    if (!outFile.is_open())
    {
        cerr << "Error: Cannot open .PROJECT file." << endl;
        return;
    }

    for (const auto& modifiedLine : lines)
    {
        outFile << modifiedLine << "\n";
    }

    cout << "Destination Directory Successfully set." << endl;

}

void Bombo::createBackup()
{
    if (BackupsEnabled())
    {
        string destinationDirectory = getDestinationDirectory(projectName + "/.PROJECT");
        if (destinationDirectory == "")
        {
            return;
        }
        string sourceDirectory = projectName;
        if (!filesystem::exists(sourceDirectory) || !filesystem::is_directory(sourceDirectory))
        {
            cerr << "Source directory does not exist or is not a directory." << endl;
            return;
        }
        filesystem::path absDestinationDirectory = filesystem::absolute(destinationDirectory);
        if (!filesystem::exists(absDestinationDirectory))
        {
            cerr << "Destination Directory does not exist." << endl;
            return;
        }
        // encrypts source code files so the backup has encrypted files
        if (UsersEnabled())
        {
            encryptSourceCodeFiles(projectName);
        }
        string sourceName = filesystem::path(sourceDirectory).filename().string();
        string timeStamp = getCurrentTimestamp();
        string backupDirectoryName = sourceName + "_" + timeStamp;

        filesystem::path backupDirectory = absDestinationDirectory / backupDirectoryName;

        filesystem::create_directories(backupDirectory);
        filesystem::copy(sourceDirectory, backupDirectory, filesystem::copy_options::recursive);

        cout << "Backup created successfully: " << backupDirectory << endl;
        
        if (UsersEnabled())
        {
            decryptSourceCodeFiles(projectName);
        }
    }
    else if(!BackupsEnabled())
    {
        cerr << "Backups not enabled." << endl;
    }
}

bool Bombo::BackupsEnabled()
{
    string metaDataPath = projectName + "/.PROJECT";
    ifstream inFile(metaDataPath);
    if (!inFile)
    {
        cerr << "Cannot open metadata file." << endl;
        return false;
    }

    string line;
    while (getline(inFile, line))
    {
        if (line.find("BackupsEnabled: true") != string::npos)
        {
            inFile.close();
            return true;
        }
    }
    inFile.close();
    return false;
}




//  ==== Delete and Remove Operations ====
void Bombo::deleteFile()
{
    string fileName = promptAndReturnFileName();

    // cannot delete util as it's required in all Bombo projects
    if (fileName == "util")
    {
        cerr << "Cannot delete util files." << endl;
        return;
    }

    // cannot delete main as it's the entry point of all Bombo projects
    if (fileName == "main")
    {
        cerr << "Cannot delete main file." << endl;
        return;
    }

    if (checkFileAlreadyExists(projectName + "/.PROJECT", fileName)) // check that the file exists
    {
        string confirmation;
        cout << "Please confirm yes or no if you wish to delete " << fileName << " file (warning: this will also delete anything stored in the file): ";
        getline(cin, confirmation);

        if (confirmation == "yes" || confirmation == "y")
        {
            // Step 1: Parse the header file to extract associated objects
            string headerFile = projectName + "/src/" + fileName + ".h";
            vector<string> structs, functions, variables, classes;

            ifstream file(headerFile);
            if (file.is_open())
            {
                string line;
                while (getline(file, line))
                {
                    // Extract structs
                    if (line.find("struct ") != string::npos)
                    {
                        size_t pos = line.find("struct ") + 7;
                        string structName = line.substr(pos, line.find("{") - pos);
                        structs.push_back(trim(structName));
                    }

                    // Extract classes
                    if (line.find("class ") != string::npos)
                    {
                        size_t pos = line.find("class ") + 6;
                        string className = line.substr(pos, line.find("{") - pos);
                        classes.push_back(trim(className));
                    }

                    // Extract variables (look for "extern <type> <variable_name>;")
                    if (line.find("extern ") != string::npos && line.find(";") != string::npos)
                    {
                        size_t start = line.find_last_of(" ");
                        if (start != string::npos)
                        {
                            string variableName = line.substr(start + 1, line.find(";") - start - 1);
                            variables.push_back(trim(variableName));
                        }
                    }

                    // Extract functions
                    size_t parenPos = line.find("(");
                    if (parenPos != string::npos && line.find(";") != string::npos)
                    {
                        size_t start = line.find_last_of(" \t", parenPos - 1);
                        if (start != string::npos)
                        {
                            string functionName = line.substr(start + 1, parenPos - start - 1);
                            functions.push_back(trim(functionName));
                        }
                    }
                }
                file.close();
            }

            // Step 2: Update metadata
            string metadataFile = projectName + "/.PROJECT";
            fstream metadata(metadataFile, ios::in);
            stringstream updatedMetadata;
            string line;

            while (getline(metadata, line))
            {
                // Remove specific file references (OtherHeader and OtherSource)
                if (line.find("OtherHeader:") != string::npos)
                {
                    line = removeFromList(line, "src/" + fileName + ".h");
                }
                else if (line.find("OtherSource:") != string::npos)
                {
                    line = removeFromList(line, "src/" + fileName + ".cpp");
                }

                // Remove structs
                if (line.find("Structs:") != string::npos)
                {
                    for (const auto& s : structs)
                        line = removeFromList(line, s);
                }

                // Remove classes
                if (line.find("Classes:") != string::npos)
                {
                    for (const auto& c : classes)
                        line = removeFromList(line, c);
                }

                // Remove variables
                if (line.find("Variables:") != string::npos)
                {
                    for (const auto& v : variables)
                        line = removeFromList(line, v);
                }

                // Remove functions
                if (line.find("Functions:") != string::npos)
                {
                    for (const auto& f : functions)
                        line = removeFromList(line, f);
                }

                // Ensure the line is well-formatted
                updatedMetadata << line << endl;
            }
            metadata.close();

            // Write updated metadata back to the file
            ofstream outMetadata(metadataFile, ios::out | ios::trunc);
            outMetadata << updatedMetadata.str();
            outMetadata.close();

            // Step 3: Delete the .h and .cpp files
            string sourceFile = projectName + "/src/" + fileName + ".cpp";

            if (remove(headerFile.c_str()) == 0)
            {
                cout << "Deleted header file: " << headerFile << endl;
            }
            else
            {
                cerr << "Failed to delete header file: " << headerFile << endl;
            }

            if (remove(sourceFile.c_str()) == 0)
            {
                cout << "Deleted source file: " << sourceFile << endl;
            }
            else
            {
                cerr << "Failed to delete source file: " << sourceFile << endl;
            }
        }
        else
        {
            cout << "File deletion canceled." << endl;
        }
    }
    else
    {
        cerr << "File does not exist in the project." << endl;
    }
}

// Helper function to remove an item from a comma-separated list
string Bombo::removeFromList(const string& line, const string& item)
{
    string result = line;
    size_t pos = result.find(item);
    if (pos != string::npos)
    {
        size_t start = (pos > 0 && result[pos - 1] == ',') ? pos - 1 : pos;
        size_t end = pos + item.length();
        if (end < result.length() && result[end] == ',') ++end;
        result.erase(start, end - start);
    }
    return result;
}








void Bombo::deleteFunction()
{

}

void Bombo::deleteClass()
{

}

void Bombo::deleteStruct()
{

}

void Bombo::deleteUser()
{

}

void Bombo::removeLibrary()
{

}