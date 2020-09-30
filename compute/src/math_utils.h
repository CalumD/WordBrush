#ifndef WORDBRUSH_MATH_UTILS_H
#define WORDBRUSH_MATH_UTILS_H

typedef struct Point {
    float x;
    float y;
} Point;

typedef struct KeyBounds {
    float x;
    float y;
    float width;
    float height;
} KeyBounds;

#include "stdlib.h"

#define NEXT_CONTROL_POINT_DRIFT_WEIGHT 0.8


Point magnify(Point point, float amount);

Point add_points(Point point_a, Point point_b);

Point reflect_control_point(Point control_point, Point associated_point);

Point interpolate(Point point_a, Point point_b, float weighting);

Point find_next_control_point(Point previous_control_point, Point current_control_point);

float random_number_between(float min, float max);

Point get_random_point_in_bounds(KeyBounds bounds);

KeyBounds calculate_directional_key_subdivision(Point current, Point next, Point scale);

#endif //WORDBRUSH_MATH_UTILS_H
