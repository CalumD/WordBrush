#include <stdio.h>

#include "wordbrush.h"

void computeCurves(Config* config) {
    printf("In the compute curves method------------\n");

    printf("Found outputDirectory: %s\n", config->outputFilePath);
    printf("Found inputPath: %s\n", config->inputFilePath);
}