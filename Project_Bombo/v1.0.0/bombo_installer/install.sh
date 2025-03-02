#!/bin/bash

set -e  # Exit on error

INSTALL_DIR="$(pwd)"
EXECUTABLE="Bombo.exe"

echo "Installing Bombo.exe in: $INSTALL_DIR"

# Check if Make is installed
if ! command -v make &> /dev/null; then
    echo "Error: Make is not installed. Please install it and try again."
    exit 1
fi

# Link the object files to create the executable
echo "Linking object files to create the executable..."
make  # This will use the obj/*.o files to create Bombo.exe

# Ensure the executable was created
if [ ! -f "$EXECUTABLE" ]; then
    echo "Error: Linking failed. Executable not found!"
    exit 1
fi

# Make the binary executable
chmod +x "$EXECUTABLE"

echo "Bombo.exe installed successfully in $INSTALL_DIR!"
echo "Run ./$EXECUTABLE to start."
