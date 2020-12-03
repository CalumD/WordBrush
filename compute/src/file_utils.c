#include "file_utils.h"

bool canWriteDirectory(char* pathToDirectory) {
    return access(pathToDirectory, W_OK) == 0 ? true : false;
}

bool canReadFile(char* pathToFile) {
    return access(pathToFile, F_OK | R_OK) == 0 ? true : false;
}

bool canWriteFile(char* pathToFile) {
    return access(pathToFile, W_OK) == 0 ? true : false;
}
