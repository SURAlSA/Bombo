#ifndef BACKUPMANAGER_H
#define BACKUPMANAGER_H

#include "util.h"
#include "UserManager.h"
#include "Variables.h"

// structs

// classes

class BackupManager
{
	private:
	protected:
	public:
		void enableBackups();
		void changeDestinationDirectory(); // The Backup Related Operations use the Absolute File Path
		void createBackup();
		bool BackupsEnabled();
};

// variables

// functions

#endif
