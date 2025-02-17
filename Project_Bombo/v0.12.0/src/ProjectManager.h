#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "util.h"
#include "Prompter.h"
#include "FileManager.h"
#include "UserManager.h"
#include "Bombo.h"
#include "Variables.h"

// structs

// classes

class ProjectManager
{
	private:
		void enterWorkEnvironment(string fileName);
		void enterWorkEnvironmentForUtil();
	protected:
	public:
		void createProject();
		void createFile();
		void createFunction();
		void createClass();
		void createVariable();
		void createStruct();
		void setWorkEnvironment();
		void addLibrary();
		void loadProject();
		void deleteFile();
		void deleteFunction(); // TODO
		void deleteClass(); // TODO
		void deleteStruct(); // TODO
		void deleteVariable(); // TODO
		void removeLibrary(); // TODO
		void logComment(); // Future Additions
		void mergeProjects();  // Future Additions
};

// variables

// functions

#endif
