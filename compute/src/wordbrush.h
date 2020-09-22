#include "main.h"

#ifndef WORDBRUSH_WORDBRUSH_H
#define WORDBRUSH_WORDBRUSH_H

typedef struct KeyOffset {
    float width_offset;
    float width_percentage;
    float height_offset;
    float height_percentage;
} KeyOffset;

#define KEY_WIDTH_PERCENTAGE 0.1
#define KEY_HEIGHT_PERCENTAGE 0.25

KeyOffset getCharacterOffset(char character);


void computeCurves(Config *config);

#endif //WORDBRUSH_WORDBRUSH_H
