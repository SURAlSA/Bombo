#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "util.h"
#include "Variables.h"
#include "ProjectManager.h"
#include "Generator.h"
#include "FileManager.h"

// structs

// classes

class UserManager
{
	private:
	protected:
	public:
		void enableUsers();
		void createUser();
		bool UsersEnabled();
		bool verifyUser(string &userName, string &Key);
		void deleteUser(); // TODO
		void enablePermissions(); // TODO
};

// variables


// functions

#endif
