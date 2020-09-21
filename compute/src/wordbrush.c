#include <stdio.h>

#include "wordbrush.h"

void computeCurves(Config* config) {
    printf("In the compute curves method------------\n");

    printf("Found inputPath: %s\n", config->inputFilePath);
    printf("Found outputDirectory: %s\n", config->outputFilePath);
    printf("Multi-file output: %d\n", config->multiFileOutput);
    printf("Width Used: %d\n", config->width);
    printf("Height Used: %d\n", config->height);
}