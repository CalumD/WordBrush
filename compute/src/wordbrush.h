#include "main.h"

#ifndef WORDBRUSH_WORDBRUSH_H
#define WORDBRUSH_WORDBRUSH_H

#define KEY_WIDTH_PERCENTAGE 0.1
#define KEY_HEIGHT_PERCENTAGE 0.25

typedef struct KeyBounds {
    float x;
    float y;
    float width;
    float height;
} KeyBounds;

typedef struct Point {
    float x;
    float y;
} Point;

KeyBounds getCharacterOffset(Config* config, char character);

Point getRandomPointOnKey(KeyBounds key);

void computeCurves(Config *config);



#endif //WORDBRUSH_WORDBRUSH_H
