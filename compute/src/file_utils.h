#ifndef WORDBRUSH_FILEUTILS_H
#define WORDBRUSH_FILEUTILS_H

#include <stdbool.h>
#include <unistd.h>
#include "main.h"

bool canWriteDirectory(char* pathToDirectory);

bool canReadFile(char* pathToFile);

bool canWriteFile(char* pathToFile);

bool check_if_delim(char c, char *delims);

char* clever_strtok(FILE* fp, char* delim);

char* next_word_file(Config* cfg);

char* next_word_arg(Config* cfg);

char* next_word(Config* cfg);

#endif //WORDBRUSH_FILEUTILS_H
