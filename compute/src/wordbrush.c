#include <stdio.h>

#include "wordbrush.h"

#define key(character, x_point, y_point) \
    case character:\
        return (KeyBounds) {\
            .x = (x_point * KEY_WIDTH_PERCENTAGE * config->width),\
            .y = (y_point * KEY_HEIGHT_PERCENTAGE * config->height),\
            .width = KEY_WIDTH_PERCENTAGE * config->width,\
            .height = KEY_HEIGHT_PERCENTAGE * config->height\
        }

KeyBounds getCharacterOffset(Config *config, char character) {
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
            return (KeyBounds) {
                    .x = (2.5 * KEY_WIDTH_PERCENTAGE * config->width),
                    .y = (3 * KEY_HEIGHT_PERCENTAGE * config->height),
                    .width = (KEY_WIDTH_PERCENTAGE * 5) * config->width,
                    .height = KEY_HEIGHT_PERCENTAGE * config->height
            };

        default:
            return (KeyBounds) {0, 0, 0, 0};
    }
}

Point getRandomPointOnKey(KeyBounds key) {
    // Find the centre point of key
    Point ret = {
            .x=(key.x + (key.width / 2)),
            .y=(key.y + (key.height / 2))
    };

    // TODO Choose direction to move in

    // TODO Move some percentage of the size of the key in that direction

    // Return the resultant point.
    return ret;
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
        KeyBounds bounds = getCharacterOffset(config, 'a' + i);
        printf("<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" style=\"fill:#%X;\"/>\n",
               bounds.x, bounds.y, bounds.width, bounds.height,
               500000 * i);
        getRandomPointOnKey(bounds);
    }
    KeyBounds bounds = getCharacterOffset(config, ' ');
    printf("<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" style=\"fill:#%X;\"/>\n",
           bounds.x, bounds.y, bounds.width, bounds.height,
           27 * 500000);
    getRandomPointOnKey(bounds);



    const char *sentence = "hello";

    for (const char* index = sentence; *index; index++) {
        if (index == sentence) {
            printf("<path d=\"M %f %f",
                   getRandomPointOnKey(getCharacterOffset(config, 'h')).x,
                   getRandomPointOnKey(getCharacterOffset(config, 'h')).y);
            continue;
        }
        printf("L %f %f ",
               getRandomPointOnKey(getCharacterOffset(config, *index)).x,
               getRandomPointOnKey(getCharacterOffset(config, *index)).y);
    }

    printf("\" stroke=\"black\" stroke-width=\"5\" fill=\"none\" /></svg>\n");
}
