#ifndef WORDBRUSH_FILEUTILS_H
#define WORDBRUSH_FILEUTILS_H

#include <stdbool.h>
#include <unistd.h>

bool canWriteDirectory(char* pathToDirectory);

bool canReadFile(char* pathToFile);

bool canWriteFile(char* pathToFile);

#endif //WORDBRUSH_FILEUTILS_H
