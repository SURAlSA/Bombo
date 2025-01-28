#ifndef GENERATOR_H
#define GENERATOR_H

#include "util.h"

// structs

// classes

// variables

extern int LOWER_CASE_CHARACTER_COUNT;

extern int UPPER_CASE_CHARACTER_COUNT;

extern int INTEGER_CHARACTER_COUNT;

extern int SPECIAL_CHARACTER_COUNT;

extern char LOWER_CASE_CHARACTERS[];

extern char UPPER_CASE_CHARACTERS[];

extern char INTEGER_CHARACTERS[];

extern char SPECIAL_CHARACTERS[];

// functions

void Generate(char *password, int size);

void OutputPassword(char *password, int size);

#endif
