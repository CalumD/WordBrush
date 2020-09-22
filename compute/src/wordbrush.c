#include <stdio.h>

#include "wordbrush.h"

KeyOffset getCharacterOffset(char character) {
    KeyOffset ret;
    switch (character) {
        case 'q':
            ret = (KeyOffset) {.width_offset=0, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=0, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'w':
            ret = (KeyOffset) {.width_offset=1, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=0, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'e':
            ret = (KeyOffset) {.width_offset=2, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=0, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'r':
            ret = (KeyOffset) {.width_offset=3, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=0, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 't':
            ret = (KeyOffset) {.width_offset=4, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=0, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'y':
            ret = (KeyOffset) {.width_offset=5, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=0, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'u':
            ret = (KeyOffset) {.width_offset=6, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=0, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'i':
            ret = (KeyOffset) {.width_offset=7, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=0, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'o':
            ret = (KeyOffset) {.width_offset=8, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=0, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'p':
            ret = (KeyOffset) {.width_offset=9, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=0, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'a':
            ret = (KeyOffset) {.width_offset=0.5, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=1, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 's':
            ret = (KeyOffset) {.width_offset=1.5, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=1, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'd':
            ret = (KeyOffset) {.width_offset=2.5, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=1, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'f':
            ret = (KeyOffset) {.width_offset=3.5, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=1, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'g':
            ret = (KeyOffset) {.width_offset=4.5, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=1, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'h':
            ret = (KeyOffset) {.width_offset=5.5, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=1, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'j':
            ret = (KeyOffset) {.width_offset=6.5, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=1, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'k':
            ret = (KeyOffset) {.width_offset=7.5, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=1, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'l':
            ret = (KeyOffset) {.width_offset=8.5, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=1, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'z':
            ret = (KeyOffset) {.width_offset=1.5, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=2, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'x':
            ret = (KeyOffset) {.width_offset=2.5, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=2, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'c':
            ret = (KeyOffset) {.width_offset=3.5, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=2, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'v':
            ret = (KeyOffset) {.width_offset=4.5, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=2, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'b':
            ret = (KeyOffset) {.width_offset=5.5, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=2, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'n':
            ret = (KeyOffset) {.width_offset=6.5, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=2, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        case 'm':
            ret = (KeyOffset) {.width_offset=7.5, .width_percentage=KEY_WIDTH_PERCENTAGE, .height_offset=2, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
        default:
            ret = (KeyOffset) {.width_offset=2.5, .width_percentage=(KEY_WIDTH_PERCENTAGE *
                               5), .height_offset=3, .height_percentage=KEY_HEIGHT_PERCENTAGE};
            break;
    }
    return
            ret;
}

void computeCurves(Config *config) {
    printf("In the compute curves method------------\n");

    printf("Found inputPath: %s\n", config->inputFilePath);
    printf("Found outputDirectory: %s\n", config->outputFilePath);
    printf("Multi-file output: %d\n", config->multiFileOutput);
    printf("Width Used: %d\n", config->width);
    printf("Height Used: %d\n", config->height);

    printf("\n\n");


    printf("<svg xmlns=\"http://www.w3.org/2000/svg\""
           " width=\"%dpx\""
           " height=\"%dpx\""
           " viewBox=\"0 0 %d %d\">\n", config->width, config->height, config->width, config->height);

    for (int i = 0; i < 26; i++) {
        KeyOffset offset = getCharacterOffset('a' + i);
        printf("<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" style=\"fill:#%X;\"/>\n",
               (offset.width_offset * offset.width_percentage * config->width),
               (offset.height_offset * offset.height_percentage) * config->height,
               offset.width_percentage * config->width,
               offset.height_percentage * config->height,
              500000 * i);
    }
    KeyOffset offset = getCharacterOffset(' ');
    printf("<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" style=\"fill:#%X;\"/>\n",
           (offset.width_offset * 0.1 * config->width),
           (offset.height_offset * offset.height_percentage) * config->height,
           offset.width_percentage * config->width,
           offset.height_percentage * config->height,
           27 * 500000);

    printf("</svg>\n");
}