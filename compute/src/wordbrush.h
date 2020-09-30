#ifndef WORDBRUSH_WORDBRUSH_H
#define WORDBRUSH_WORDBRUSH_H

#include "math_utils.h"
#include "main.h"
#include "svg_utils.h"

#define KEY_WIDTH_PERCENTAGE 0.1
#define KEY_HEIGHT_PERCENTAGE 0.25
#define KEY_ACTIVE_ZONE_PERCENTAGE 0.33

KeyBounds get_key_bounds(Config* config, char character);

Point get_random_point_on_next_key(svg* svg, KeyBounds previous_key, KeyBounds next_key);
Point get_random_point_on_current_key(svg* svg, KeyBounds current_key);

void compute_curves(Config* config, char* word, FILE* output_file);

#endif //WORDBRUSH_WORDBRUSH_H
