#include "Bombo.h"

// variables

bool projectLoaded;

string projectName;

// functions

void Bombo::handleCommand(string& command)
{   
    if (command == "help")
    {
        cout << "Common commands: \n\tcreateProject()\n\tloadProject()\n" 
             << "\tcreateFile()\n\tcreateFunction()\n\tcreateClass()\n" 
             << "\tcreateStruct()\n\tcreateVariable()\n\taddInclude()\n" 
             << "\teditClass()\n\teditFunction()\n";
    }
    if(command == "createFile()" && !projectLoaded || command == "createFunction()" && !projectLoaded || command == "createClass()" && !projectLoaded
    || command == "createVariable()" && !projectLoaded || command == "createStruct()" && !projectLoaded || command == "compile()" && !projectLoaded
    || command == "clean()" && !projectLoaded)
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
    if (command == "createProject()")
    {
        createProject();
    }

}

void Bombo::createProject()
{
    cout << "Enter a project name: ";
    getline(cin, projectName);
    // creates directories
    filesystem::create_directory(projectName);
    cout << "directory /" << projectName << " created." << endl;
    filesystem::create_directory(projectName + "/src");
    cout << "directory /" << projectName << "/src created." << endl;  
    filesystem::create_directory(projectName + "/obj");
    cout << "directory /" << projectName << "/obj created." << endl;

    // basic files
    buildFile(projectName + "/src/main.cpp", "#include \"util.h\"\n\nint main()\n{\n\n    return 0;\n}\n");
    buildFile(projectName + "/src/util.h", "#ifndef UTIL_H\n#define UTIL_H\n\n#include <iostream>\n\nusing namespace std;\n\n// functions\n\n#endif\n");
    buildFile(projectName + "/src/util.cpp", "#include \"util.h\"\n\n// functions\n");
    buildFile(projectName + "/Makefile", "CXX = g++\n\nTARGET = " + projectName + ".exe\n\nSRCS = src/main.cpp src/util.cpp\n\nOBJS = obj/main.o obj/util.o\n\nall: $(TARGET)\n\n$(TARGET): $(OBJS)\n\t$(CXX) -o $(TARGET) $(OBJS)\n\nobj/%.o: src/%.cpp\n\t$(CXX) -c $< -o $@\n\nclean:\n\trm -f $(TARGET) obj/*.o");
    buildFile(projectName + "/Bombo.log", "");

    logEvent("Project " + projectName + " created.", projectName);
    writeProjectFile(projectName);
    projectLoaded = true;
}

void Bombo::createFile()
{
    string fileName = promptAndReturnFileName();
    if (!checkFileAlreadyExists(projectName + "/.PROJECT", fileName))
    {
        string str = fileName;
        ofstream newHeaderFile(projectName + "/src/" + fileName + ".h");
        transform(str.begin(), str.end(), str.begin(), ::toupper);
        newHeaderFile << "#ifndef " << str << "_H\n#define " << str << "_H\n\n"
                    << "#include \"util.h\"\n\n// structs\n\n// classes\n\n// variables\n\n// functions\n\n"
                    << "#endif";

        newHeaderFile.close();
        cout << "file /" << projectName << "/src/" << fileName << ".h created." << endl;

        ofstream newSourceFile(projectName + "/src/" + fileName + ".cpp");
        newSourceFile << "#include " << "\"" << fileName << ".h\"\n\n// variables\n\n// functions";
        newSourceFile.close();
        cout << "file /" << projectName << "/src/" << fileName << ".cpp created." << endl;

        addDataToTargetFile("src/" + fileName + ".h", projectName + "/.PROJECT", " ", "OtherHeader:");
        addDataToTargetFile("obj/" + fileName + ".o", projectName + "/Makefile", " ", "OBJS =");
        addDataToTargetFile("src/" + fileName + ".cpp", projectName + "/.PROJECT", " ", "OtherSource:");
        addDataToTargetFile("src/" + fileName + ".cpp", projectName + "/Makefile", " ", "SRCS =");
        logEvent(fileName + " file created.", projectName);
    }
    else
    {
        cerr << "File Already Exists." << endl;
    }
}

void Bombo::compile()
{
    filesystem::path makefilePath = filesystem::path(projectName) / "Makefile";

    if (!std::filesystem::exists(makefilePath)) 
    {
        std::cerr << "Error: Makefile not found in " << makefilePath << std::endl;
        return;
    }

    string command = "make -C " + projectName + " 2>&1";
    array<char, 128> buffer;
    string result;

    unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

    if (!pipe)
    {
        cerr << "Failed to run make command." << endl;
        return;
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }

    cout << result << endl;
}

void Bombo::clean()
{
    filesystem::path makefilePath = filesystem::path(projectName) / "Makefile";

    if (!std::filesystem::exists(makefilePath)) 
    {
        std::cerr << "Error: Makefile not found in " << makefilePath << std::endl;
        return;
    }

    // Command to run 'make clean'
    string command = "make -C " + projectName + " clean 2>&1";
    array<char, 128> buffer;
    string result;

    unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

    if (!pipe)
    {
        cerr << "Failed to run make clean command." << endl;
        return;
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }

    cout << result << endl;
}

void Bombo::loadProject()
{
    string tempProjectName;
    cout << "Enter the name of the project: ";
    getline(cin, tempProjectName);
    string metaData = tempProjectName + "/.PROJECT";

    if (filesystem::exists(metaData))
    {
        cout << "Meta data found." << endl;
        projectName = tempProjectName;
        projectLoaded = true;
        cout << "Project loaded." << endl;
        logEvent("Project loaded.", projectName);
    }
    else
    {
        cout << "Cant find meta data." << endl;
        cout << "Failed to load project " << tempProjectName << "." << endl;
    }
}

void Bombo::createFunction()
{
    string fileName = promptAndReturnFileName();
    if (fileName == "main")
    {
        cerr << "Cannot add a function to main." << endl;
        return;
    }

    // Maybe implement the ability to catch illegal return types in the future
    string returnType = promptAndReturnReturnType();


    string functionName = promptAndReturnFunctionName();


    if (checkFunctionAlreadyExists(projectName + "/.PROJECT", functionName))
    {
        cerr << "Function name already exists. Name must be different." << endl;
        return;
    }

    vector<pair<string, string>> parameters;
    promptForParameters(parameters);
    

    string parameterList;
    for (size_t i = 0; i < parameters.size(); i++)
    {
        parameterList += parameters[i].first + " " + parameters[i].second;
        if (i != parameters.size() - 1)
        {
            parameterList += ", ";
        }
    }

    string functionDeclaration = returnType + " " + functionName + "(" + parameterList + ");";
    string functionDefinition = returnType + " " + functionName + "(" + parameterList + ")\n{\n    // TODO: Implement function\n}\n";
    // adding function declaration to the header file.
    addDataToTargetFile(functionDeclaration, projectName + "/src/" + fileName + ".h", "\n\n", "// functions");
    // adding function definition to the source file
    addDataToTargetFile(functionDefinition, projectName + "/src/" + fileName + ".cpp", "\n\n", "// functions");
    // adding function to the metadata
    addDataToTargetFile(functionName, projectName + "/.PROJECT", " ", "Functions:");



    cout << "Function " << functionName << " created successfully." << endl;
    logEvent("Function " + functionName + " created in " + fileName + ".", projectName);
}

void Bombo::createClass()
{
    string fileName = promptAndReturnFileName();
    
    if (fileName == "main")
    {
        cerr << "Cannot add a class to main." << endl;
        return;
    }
    if (fileName == "util")
    {
        cerr << "Cannot add a class to util." << endl;
        return;
    }

    string className = promptAndReturnClassName();
    

    vector<pair<string, string>> parameters;
    promptForParameters(parameters);

    vector<int> functionStatus;
    vector<pair<string, string>> functions;
    vector<vector<string>> functionVariables;
    promptForFunctions(functionStatus, functions, functionVariables);
    
    string classDefinition = returnClassDefinition(className, parameters, functionStatus, functionVariables, functions);
    
    string allFunctions = returnAllFunctions(className, parameters, functionStatus, functionVariables, functions);

    
    // adds class definition to the header
    addDataToTargetFile(classDefinition, projectName + "/src/" + fileName + ".h", "\n\n", "// classes");
    // adds functions to the source file
    addDataToTargetFile(allFunctions, projectName + "/src/" + fileName + ".cpp", "\n\n", "// functions");
    // adds the defined class to the metadata
    addDataToTargetFile(className, projectName + "/.PROJECT", " ", "Classes:");
    logEvent("Class " + className + " created in " + fileName + ".", projectName);
    cout << "Class " << className << " created successfully." << endl;
}

void Bombo::createVariable()
{
    string fileName = promptAndReturnFileName();
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
    string variableType = promptAndReturnVariableType();
    string variableName = promptAndReturnVariableName();

    addDataToTargetFile(variableType + " " + variableName + ";", projectName + "/src/" + fileName + ".cpp", "\n\n", "// variables");
    addDataToTargetFile("extern " + variableType + " " + variableName + ";", projectName + "/src/" + fileName + ".h", "\n\n", "// variables");
    cout << "Variable " << variableName << " created successfully." << endl;
    logEvent("Variable " + variableName + " created in " + fileName + ".", projectName);
    
}

string Bombo::returnStruct(vector<pair<string, string>>& parameters, string& structName)
{
    string structDefinition = "struct " + structName + "\n{\n";
    for (size_t i = 0; i < parameters.size(); i++)
    {
        structDefinition += "\t" + parameters[i].first + " " + parameters[i].second + ";\n";
    }
    return structDefinition += "};";
}

void Bombo::promptForParameters(vector<pair<string, string>>& parameters)
{
    string variableName, variableType;
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
        
        determineFunctionStatus(functionStatus);
        
        string needVariable;
        cout << "Does this function need variables?(yes or no): ";
        getline(cin, needVariable);
        vector<string> variables;
        if (needVariable == "yes" || needVariable == "y")
        {
            string functionVariableType;
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

string Bombo::promptAndReturnFileName()
{
    string fileName;
    cout << "Enter a filename: ";
    getline(cin, fileName);
    return fileName;
}

string Bombo::promptAndReturnClassName()
{
    string className;
    cout << "Enter a class name: ";
    getline(cin, className);
    return className;
}

string Bombo::returnClassDefinition(string& className, vector<pair<string, string>>& parameters, vector<int>& functionStatus, vector<vector<string>> functionVariables, vector<pair<string, string>>& functions)
{
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

string Bombo::promptAndReturnVariableName()
{
    string variableName;
    cout << "Enter a variable name: ";
    getline(cin, variableName);
    return variableName;
}

void Bombo::createStruct()
{
    string fileName = promptAndReturnFileName();
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

    string structName = promptAndReturnStructName();

    vector<pair<string, string>> parameters;
    promptForParameters(parameters);

    string structDefinition = returnStruct(parameters, structName);
    addDataToTargetFile(structDefinition, projectName + "/src/" + fileName + ".h", "\n\n", "// structs");
    logEvent("Struct " + structName + " created in " + fileName + ".", projectName);
    cout << "Struct " << structName << " created successfully." << endl;
}

string Bombo::promptAndReturnStructName()
{
    string structName;
    cout << "Enter a struct name: ";
    getline(cin, structName);
    return structName;
}

string Bombo::promptAndReturnVariableType()
{
    string variableType;
    cout << "Enter a variable type: ";
    getline(cin, variableType);
    return variableType;
}

string Bombo::returnAllFunctions(string& className, vector<pair<string, string>>& parameters, vector<int>& functionStatus, vector<vector<string>> functionVariables, vector<pair<string, string>>& functions)
{
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