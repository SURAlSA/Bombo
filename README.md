# Bombo

**Bombo** is a C++ Terminal-Based Development Tool designed to streamline code organization and ensure consistency across large-scale projects. It provides a structured format for development, enabling efficient collaboration and project cohesion. (Developed for Linux operation systems)

---

## Features

### Generic Operations
- **Project Management**: Create, load, and compile projects with ease.
- **File Creation**: Generate boilerplate files for `.cpp` and `.h` files.
- **Code Organization**: Automatically position functions, classes, structs, and variables using standardized markers.
- **Metadata Management**: Maintain all project details in a `.PROJECT` metadata file.

### User Management
- Enable multi-user support with unique encrypted keys for security.
- Track user actions in a detailed `Bombo.log` file.

### Backup Functionality
- Enable and configure backups to an external destination.
- Automatically timestamp and store backups for project safety.

### Build Tools
- Seamless integration with Makefiles for compiling and cleaning projects.

---

## Installation

1. Clone the repository:
	```bash
	git clone https://github.com/SURAlSA/Bombo.git

2. Compile the code:
- Ensure your terminal is in same directory as Makefile
- make clean
- make
	
3. Launch Bombo
- ./Bombo.exe
- type "help" for some basic functions
	
## List of callable commands:
	// ==== Generic Operations ==== 
	void createProject(); 
	void compile(); 
	void clean(); 
	void createFile(); 
	void createFunction(); 
	void createClass(); 
	void createVariable(); 
	void createStruct(); 
	void addLibrary(); 
	void loadProject(); 
	void logCommment();
	void mergeProjects(); // TODO
	 
	// ==== User Related Operations ==== 
	void enableUsers(); 
	void createUser();
	void deleteUser(); // TODO
	void enablePermissions(); // TODO 
	 
	// ==== Bombo Personality Functions ==== 
	void joke(); 
	 
	// ==== Backup Related Operations ==== 
	void enableBackups(); 
	void changeDestinationDirectory(); 
	void createBackup(); 
	bool BackupsEnabled(); 
	 
	 //  ==== Delete and Remove Operations ==== 
	void deleteFile();
	void deleteFunction(); 
	void deleteClass(); 
	void deleteStruct();
	void deleteVariable(); 
	void deleteUser(); // TODO 
	void removeLibrary(); // TODO 

## Author
- Zane Miller (Surai) - Creater and Maintainer - https://github.com/SURAlSA 
