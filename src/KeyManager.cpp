#include "KeyManager.h"

// variables

// functions

void replaceKey(const string& filePath, const string& key)
{
    ofstream outFile(filePath);
    if (outFile.is_open())
    {
        outFile << key;  // Write the new key (encrypted or decrypted) to the file
        outFile.close();
    }
    else
    {
        throwCantOpenFileAtFilePathForWritingError(filePath);
    }
}


string readKey(const string& filePath)
{
    ifstream inFile(filePath);
    if (!inFile)
    {
        throwCantOpenFileAtFilePathError(filePath);
        return "";
    }

    string key;
    getline(inFile, key);  // Read the first line as the key
    inFile.close();

    return key;
}


string decryptKey(const string& filePath)
{
    string key = readKey(filePath);  // Read the key from file

    char xorConstant = 0x5A;  // XOR constant

    string decryptedKey = key;  // Copy key to decryptedKey
    for (size_t i = 0; i < key.size(); i++)
    {
        decryptedKey[i] = ~key[i];  // Invert binary
        decryptedKey[i] = decryptedKey[i] ^ xorConstant;  // XOR with constant to decrypt
    }
    //cout << "Testing: " << decryptedKey << endl;
    return decryptedKey;
}


void encryptKey(const string& filePath)
{
    string key = readKey(filePath);  // Read the key from file

    char xorConstant = 0x5A;  // XOR constant

    string encryptedKey = key ;  // Copy key to encryptedKey
    for (size_t i = 0; i < key.size(); i++)
    {
        encryptedKey[i] = key [i] ^ xorConstant;  // XOR with constant
        encryptedKey[i] = ~encryptedKey[i];      // Invert binary
    }
    
    replaceKey(filePath, encryptedKey);  // Save the encrypted key back to file
}

