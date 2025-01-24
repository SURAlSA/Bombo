#include "CompileManager.h"

// variables

// functions

void clean()
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


void compile()
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

