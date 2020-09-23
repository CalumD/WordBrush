//
// Created by calum on 23/09/2020.
//

#ifndef WORDBRUSH_MATH_UTILS_H
#define WORDBRUSH_MATH_UTILS_H

#include <stdlib.h>
#include "wordbrush.h"

#define NEXT_CONTROL_POINT_DRIFT_WEIGHT 0.8


Point magnify(Point point, float amount);

Point add_points(Point point_a, Point point_b);

Point reflect_control_point(Point control_point, Point associated_point);

Point interpolate(Point point_a, Point point_b, float weighting);

Point find_next_control_point(Point previous_control_point, Point current_control_point);

float random_number_between(float min, float max);

Point get_random_point_in_bounds(KeyBounds bounds);

#endif //WORDBRUSH_MATH_UTILS_H
