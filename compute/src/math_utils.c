//
// Created by calum on 23/09/2020.
//

#include "math_utils.h"

Point magnify(Point point, float amount) {
    return (Point) {
            .x = point.x * amount,
            .y = point.y * amount
    };
}

Point add_points(Point point_a, Point point_b) {
    return (Point) {
            .x = point_a.x + point_b.x,
            .y = point_a.y + point_b.y
    };
}

Point reflect_control_point(Point control_point, Point associated_point) {
    return (Point) {
            .x = (2 * associated_point.x) - control_point.x,
            .y = (2 * associated_point.y) - control_point.y
    };
}

Point interpolate(Point point_a, Point point_b, float weighting) {
    float b_weight = 1. - weighting;

    Point a_weighted = magnify(point_a, weighting);
    Point b_weighted = magnify(point_b, b_weight);

    return add_points(a_weighted, b_weighted);
}

Point find_next_control_point(Point previous_control_point, Point current_control_point) {
    Point reflected_cp = reflect_control_point(previous_control_point, current_control_point);
    return interpolate(current_control_point, reflected_cp, NEXT_CONTROL_POINT_DRIFT_WEIGHT);
}

float random_number_between(float min, float max) {
    float scale = rand() / (float) RAND_MAX;
    return min + scale * (max - min);
}

Point get_random_point_in_bounds(KeyBounds bounds) {
    return (Point) {
            .x = random_number_between(bounds.x, bounds.x + bounds.width),
            .y = random_number_between(bounds.y, bounds.y + bounds.height)
    };
}