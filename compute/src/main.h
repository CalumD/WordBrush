#include <stdbool.h>

#ifndef WORDBRUSH_MAIN_H
#define WORDBRUSH_MAIN_H

typedef struct Config {
    bool successfullyInitialised;
    bool multiFileOutput;
    char* inputFilePath;
    char* outputFilePath;
    int width;
    int height;
} Config;


#endif //WORDBRUSH_MAIN_H
