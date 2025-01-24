#include "ProjectManager.h"

// variables

// functions

void ProjectManager::createProject()
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
        cerr << "File Already Exists. File name must be different." << endl;
    }
}
void ProjectManager::createFunction()
{
    Checker Checker;
    UserManager UserManager;
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
        if (!Checker.checkFileAlreadyExists(projectName + "/.PROJECT", fileName))
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
    if (Checker.checkFunctionAlreadyExists(projectName + "/.PROJECT", functionName)) // FIXED and is working as expected
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
    if (UserManager.UsersEnabled()) // if users enabled we log who created the function
    {
        logEvent("Function " + functionName + " created in " + fileName + " by " + userName + ".", projectName);
    }
    else
    {
        logEvent("Function " + functionName + " created in " + fileName + ".", projectName);
    }
}
void ProjectManager::createClass()
{
    Checker Checker;
    UserManager UserManager;
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
    if (!Checker.checkFileAlreadyExists(projectName + "/.PROJECT", fileName))
    {
        cerr << "File doesn't exist." << endl;
        return;
    }

    // get class name
    string className = promptAndReturnClassName();

    // checks if class already exists
    if (Checker.checkClassAlreadyExists(projectName + "/.PROJECT", className))
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
void ProjectManager::createVariable()
{
    Checker Checker;
    UserManager UserManager;

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
    if (!Checker.checkFileAlreadyExists(projectName + "/.PROJECT", fileName))
    {
        cerr << "File doesn't exist." << endl;
        return;
    }

    // get variable type
    string variableType = promptAndReturnVariableType();
    
    // get variable name
    string variableName = promptAndReturnVariableName();

    // checks if Variable already exists
    if (Checker.checkVariableAlreadyExists(projectName + "/.PROJECT", variableName))
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
    if (UserManager.UsersEnabled()) // if users enabled we log who created the variable
    {
        logEvent("Variable " + variableName + " created in " + fileName + " by " + userName + ".", projectName);
    }
    else
    {
        logEvent("Variable " + variableName + " created in " + fileName + ".", projectName);
    }    
}
void ProjectManager::createStruct()
{
    Checker Checker;
    UserManager UserManager;

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
    if (!Checker.checkFileAlreadyExists(projectName + "/.PROJECT", fileName))
    {
        cerr << "File doesn't exist." << endl;
        return;
    }

    // get struct name
    string structName = promptAndReturnStructName();

    if (Checker.checkStructAlreadyExists(projectName + "/.PROJECT", structName)) // FIXED and is working as expected
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
void ProjectManager::deleteFile()
{
    Checker Checker;
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

    if (Checker.checkFileAlreadyExists(projectName + "/.PROJECT", fileName)) // check that the file exists
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

