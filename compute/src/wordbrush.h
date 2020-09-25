#include <stdio.h>

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

KeyBounds get_key_bounds(Config* config, char character);

Point get_random_point_on_key(KeyBounds key);

void compute_curves(Config* config, char* word, FILE* output_file);



#endif //WORDBRUSH_WORDBRUSH_H
