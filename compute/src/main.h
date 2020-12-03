#ifndef WORDBRUSH_MAIN_H
#define WORDBRUSH_MAIN_H

#include <stdbool.h>
#include <stdio.h>

typedef struct Config {
    bool successfully_initialised;
    long single_file_column_count;
    char* input_file_path;
    FILE* inputFile;
    char* output_file_path;
    char** words;
    long word_count;
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
