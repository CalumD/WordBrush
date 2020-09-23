#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>

#include "file_utils.h"

bool canWriteDirectory(char* pathToDirectory) {
    return access(pathToDirectory, W_OK) == 0 ? true : false;
}


bool canReadFile(char* pathToFile) {
    return access(pathToFile, F_OK | R_OK) == 0 ? true : false;
}
