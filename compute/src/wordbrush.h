#ifndef WORDBRUSH_WORDBRUSH_H
#define WORDBRUSH_WORDBRUSH_H

#include "math_utils.h"
#include "main.h"
#include "svg_utils.h"
#include "ctype.h"

#define KEY_WIDTH_PERCENTAGE 0.1
#define KEY_HEIGHT_PERCENTAGE 0.25
#define KEY_ACTIVE_ZONE_PERCENTAGE 0.33

KeyBounds get_key_bounds(Config* config, char character);

Point get_random_point_on_next_key(KeyBounds previous_key, KeyBounds next_key);
Point get_random_point_on_current_key(KeyBounds current_key);

void compute_curves(Config* config, svg* svg, char* word);

#endif //WORDBRUSH_WORDBRUSH_H
