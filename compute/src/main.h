#ifndef WORDBRUSH_MAIN_H
#define WORDBRUSH_MAIN_H

#include <stdbool.h>

typedef struct Config {
    bool successfullyInitialised;
    bool multiFileOutput;
    char* inputFilePath;
    char* outputFilePath;
    char** extra_args;
    long width;
    long height;
} Config;

#include <stdio.h>
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
