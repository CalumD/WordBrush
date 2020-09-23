#include "main.h"

#ifndef WORDBRUSH_WORDBRUSH_H
#define WORDBRUSH_WORDBRUSH_H

#define KEY_WIDTH_PERCENTAGE 0.1
#define KEY_HEIGHT_PERCENTAGE 0.25
#define KEY_ACTIVE_ZONE_PERCENTAGE 0.33

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

Point get_random_point_on_next_key(KeyBounds previous_key, KeyBounds next_key);
Point get_random_point_on_current_key(KeyBounds current_key);

void compute_curves(Config *config);



#endif //WORDBRUSH_WORDBRUSH_H
