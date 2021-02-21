#ifndef WORDBRUSH_MAIN_H
#define WORDBRUSH_MAIN_H

#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <libgen.h>
#include <argz.h>

typedef struct Config {
    bool successfully_initialised;
    bool draw_background;
    long single_file_column_count;
    char* input_file_path;
    FILE* input_file;
    FILE* output_meta_file;
    char* start_timestamp;
    char* stop_timestamp;
    char* output_file_path;
    char* words;
    size_t args_legnth;
    long word_count;
    long width;
    long height;
} Config;

#include <malloc.h>
#include <argp.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "wordbrush.h"
#include "file_utils.h"
#include "debug.h"

#endif //WORDBRUSH_MAIN_H
