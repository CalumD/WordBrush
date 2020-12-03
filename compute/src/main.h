#ifndef WORDBRUSH_MAIN_H
#define WORDBRUSH_MAIN_H

#include <stdbool.h>
#include <stdio.h>

typedef struct Config {
    bool successfullyInitialised;
    bool multiFileOutput;
    char* inputFilePath;
    FILE* inputFile;
    char* outputFilePath;
    char** extra_args;
    char** current_arg;
    long width;
    long height;
} Config;

#include <malloc.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "wordbrush.h"
#include "file_utils.h"
#include "debug.h"

#endif //WORDBRUSH_MAIN_H
