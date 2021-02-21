#include "wordbrush.h"

#define key(character, x_point, y_point) \
    case character:\
        return (KeyBounds) {\
            .x = (x_point * KEY_WIDTH_PERCENTAGE * config->width),\
            .y = (y_point * KEY_HEIGHT_PERCENTAGE * config->height),\
            .width = KEY_WIDTH_PERCENTAGE * config->width,\
            .height = KEY_HEIGHT_PERCENTAGE * config->height\
        }

KeyBounds get_key_bounds(Config* config, char character) {
    switch (tolower(character)) {
        key('q', 0, 0);
        key('w', 1, 0);
        key('e', 2, 0);
        key('r', 3, 0);
        key('t', 4, 0);
        key('y', 5, 0);
        key('u', 6, 0);
        key('i', 7, 0);
        key('o', 8, 0);
        key('p', 9, 0);
        key('a', 0.5, 1);
        key('s', 1.5, 1);
        key('d', 2.5, 1);
        key('f', 3.5, 1);
        key('g', 4.5, 1);
        key('h', 5.5, 1);
        key('j', 6.5, 1);
        key('k', 7.5, 1);
        key('l', 8.5, 1);
        key('z', 1.5, 2);
        key('x', 2.5, 2);
        key('c', 3.5, 2);
        key('v', 4.5, 2);
        key('b', 5.5, 2);
        key('n', 6.5, 2);
        key('m', 7.5, 2);
        case ' ':
            return (KeyBounds) {
                    .x = (2.5 * KEY_WIDTH_PERCENTAGE * config->width),
                    .y = (3 * KEY_HEIGHT_PERCENTAGE * config->height),
                    .width = (KEY_WIDTH_PERCENTAGE * 5) * config->width,
                    .height = KEY_HEIGHT_PERCENTAGE * config->height
            };

        default:
            return (KeyBounds) {0, 0, 0, 0};
    }
}

Point get_random_point_on_current_key(KeyBounds current_key) {
    Point scale = {
            .x = current_key.width * KEY_ACTIVE_ZONE_PERCENTAGE,
            .y = current_key.height * KEY_ACTIVE_ZONE_PERCENTAGE
    };
    KeyBounds sub_division = (KeyBounds) {
            .x = (current_key.x + (current_key.width / 2)) - (scale.x / 2),
            .y = (current_key.y + (current_key.height / 2)) - (scale.y / 2),
            .width = scale.x,
            .height = scale.y
    };
    return get_random_point_in_bounds(sub_division);
}

Point get_random_point_on_next_key(KeyBounds current_key, KeyBounds next_key) {
    // Find the centre point of current Key
    Point current = {
            .x = (current_key.x + (current_key.width / 2)),
            .y = (current_key.y + (current_key.height / 2))
    };

    // Find the centre point of next key
    Point next = {
            .x=(next_key.x + (next_key.width / 2)),
            .y=(next_key.y + (next_key.height / 2))
    };
    Point scale = {.x = next_key.width * KEY_ACTIVE_ZONE_PERCENTAGE, .y = next_key.height * KEY_ACTIVE_ZONE_PERCENTAGE};

    // Find then return a random point in the sub division bounds.
    KeyBounds sub_division_bound = calculate_directional_key_subdivision(current, next, scale);
    return get_random_point_in_bounds(sub_division_bound);
}

Point* get_random_points_on_keys(Config* config, int word_length, char* word) {
    Point* points = malloc(sizeof(Point) * word_length);
    bool is_first_char = true;

    for (size_t i = 0, char_index = 0; i < strlen(word); i++) {
        if (isalpha(word[i])) {
            if (is_first_char) {
                points[0] = get_random_point_on_current_key(get_key_bounds(config, word[i]));
                is_first_char = false;
            } else {
                points[char_index] = get_random_point_on_next_key(
                        get_key_bounds(config, word[i - 1]),
                        get_key_bounds(config, word[i])
                );
            }
            char_index++;
        }
    }

    return points;
}

void compute_curves(Config* config, svg* svg, char* word) {

    if (config->draw_background) {
        int key_count = 26;
        for (int i = 0; i < key_count; i++) {
            /*
             * TODO
             * I can't remember if modern C standards guarantee that
             * sequential characters are adjacent in terms of integer
             * value (i.e. that 'a' + 1 == 'b'). Should probably double-
             * check this.
             */
            svg_key(svg, config, 'a' + i);
        }
        svg_key(svg, config, ' ');
    }

    int alphaCharCount = 0;
    for (size_t i = 0; i < strlen(word); i++) {
        if (!isalpha(word[i])) continue;
        else alphaCharCount++;
    }

    Point* key_locations = get_random_points_on_keys(config, alphaCharCount, word);

    if (config->draw_background) {
        for (int i = 0; i < alphaCharCount; i++) {
            add_to_svg(svg, "<circle cx=\"%f\" cy=\"%f\" r=\"5\" style=\"fill:red\"/>\n", key_locations[i].x,
                       key_locations[i].y);
        }
    }

    svg_quadratic_bezier(svg, alphaCharCount, key_locations);

    free(key_locations);
}
