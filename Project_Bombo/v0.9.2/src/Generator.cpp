#include "Generator.h"

// variables

int LOWER_CASE_CHARACTER_COUNT = 26;

int UPPER_CASE_CHARACTER_COUNT = 26;

int INTEGER_CHARACTER_COUNT = 10;

int SPECIAL_CHARACTER_COUNT = 5;

char LOWER_CASE_CHARACTERS[] {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

char UPPER_CASE_CHARACTERS[] {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

char INTEGER_CHARACTERS[] {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

char SPECIAL_CHARACTERS[] {'!', '@', '#', '$', '*'};

// functions

void OutputPassword(char *password, int size)
{
    // TODO: Implement function
    for (int i = 0; i < size; i++)
    {
        cout << password[i];
    }
    cout << "\n";
}


void Generate(char *password, int size)
{
    // TODO: Implement function
    int count = 0;
    char tempWord[size];
    for (int i = 0; i < size; i++)
    {
        switch (count)
        {
            case 0:
                tempWord[i] = UPPER_CASE_CHARACTERS[getRandomInt(0, UPPER_CASE_CHARACTER_COUNT-1)];
                count++;
                break;
            case 1:
                tempWord[i] = LOWER_CASE_CHARACTERS[getRandomInt(0, LOWER_CASE_CHARACTER_COUNT-1)];
                count++;
                break;
            case 2:
                tempWord[i] = SPECIAL_CHARACTERS[getRandomInt(0, SPECIAL_CHARACTER_COUNT-1)];
                count++;
                break;
            case 3:
                tempWord[i] = INTEGER_CHARACTERS[getRandomInt(0, INTEGER_CHARACTER_COUNT-1)];
                count = 0;
                break;
        }
        
    }

    for (int i = 0; i < size; i++)
    {
        password[i] = tempWord[i];
    }
}

