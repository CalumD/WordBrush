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

/**
 * Figure out which compass direction the old key is to the new one using the following layout:
 *
 * 0 1 2
 * 7 8 3
 * 6 5 4
 */
KeyBounds calculate_directional_key_subdivision(Point current, Point next, Point scale) {
    KeyBounds ret;
    if (current.x < next.x && current.y < next.y) {             // 0
        ret = (KeyBounds) {.x = next.x - scale.x, .y = next.y - scale.y,
                .width = scale.x, .height = scale.y};
    } else if (current.x == next.x && current.y < next.y) {     // 1
        ret = (KeyBounds) {.x = next.x - (scale.x / 2), .y = next.y - scale.y,
                .width = scale.x, .height = scale.y};
    } else if (current.x > next.x && current.y < next.y) {      // 2
        ret = (KeyBounds) {.x = next.x, .y = next.y - scale.y,
                .width = scale.x, .height = scale.y};
    } else if (current.x > next.x && current.y == next.y) {     // 3
        ret = (KeyBounds) {.x = next.x, .y = next.y - (scale.y / 2),
                .width = scale.x, .height = scale.y};
    } else if (current.x > next.x && current.y > next.y) {      // 4
        ret = (KeyBounds) {.x = next.x, .y = next.y,
                .width = scale.x, .height = scale.y};
    } else if (current.x == next.x && current.y > next.y) {     // 5
        ret = (KeyBounds) {.x = next.x - (scale.x / 2), .y = next.y,
                .width = scale.x, .height = scale.y};
    } else if (current.x < next.x && current.y > next.y) {      // 6
        ret = (KeyBounds) {.x = next.x - scale.x, .y = next.y,
                .width = scale.x, .height = scale.y};
    } else if (current.x < next.x && current.y == next.y) {     // 7
        ret = (KeyBounds) {.x = next.x - scale.x, .y = next.y - (scale.y / 2),
                .width = scale.x, .height = scale.y};
    } else {                                                    // 8
        ret = (KeyBounds) {.x = next.x - (scale.x / 2), .y = next.y - (scale.y / 2),
                .width = scale.x, .height = scale.y};
    }
    return ret;
}
