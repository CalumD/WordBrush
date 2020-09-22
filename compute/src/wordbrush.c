#include <stdio.h>

#include "wordbrush.h"

#define key(c, wo, ho) \
	case c: \
    	    return (KeyOffset) { \
    	        .width_offset = wo, \
	        .width_percentage = KEY_WIDTH_PERCENTAGE, \
	        .height_offset = ho, \
    	        .height_percentage = KEY_HEIGHT_PERCENTAGE \
    	    }

KeyOffset getCharacterOffset(char character) {
    switch (character) {
        key('q', 0, 0);
        key('w', 1, 0);
        key('e', 2, 0);
        key('r', 3, 0);
        key('t', 4, 0);
        key('y', 5, 0);
        key('u', 6, 0);
        key('i', 7, 0);
        key('o', 8, 0);
        key('p', 9, 0);
        key('a', 0.5, 1);
        key('s', 1.5, 1);
        key('d', 2.5, 1);
        key('f', 3.5, 1);
        key('g', 4.5, 1);
        key('h', 5.5, 1);
        key('j', 6.5, 1);
        key('k', 7.5, 1);
        key('l', 8.5, 1);
        key('z', 1.5, 2);
        key('x', 2.5, 2);
        key('c', 3.5, 2);
        key('v', 4.5, 2);
        key('b', 5.5, 2);
        key('n', 6.5, 2);
        key('m', 7.5, 2);
        case ' ':
            return (KeyOffset) {
                .width_offset = 2.5,
                .width_percentage = (KEY_WIDTH_PERCENTAGE * 5),
                .height_offset = 3,
                .height_percentage = KEY_HEIGHT_PERCENTAGE
            };

        default:
            return (KeyOffset) {0, 0, 0, 0};
    }
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
