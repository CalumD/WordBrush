#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "wordbrush.h"
#include "debug.h"
#include "math_utils.h"

#define key(character, x_point, y_point) \
    case character:\
        return (KeyBounds) {\
            .x = (x_point * KEY_WIDTH_PERCENTAGE * config->width),\
            .y = (y_point * KEY_HEIGHT_PERCENTAGE * config->height),\
            .width = KEY_WIDTH_PERCENTAGE * config->width,\
            .height = KEY_HEIGHT_PERCENTAGE * config->height\
        }

#define add_to_svg(svg, ...) do { \
    do { \
        size_t req = snprintf(svg->cur, svg->size_remaining, __VA_ARGS__); \
        if (req > svg->size_remaining) { \
            char* new_buf = realloc(svg->buf, svg->size * 2); \
            svg->cur += new_buf - svg->buf; \
            svg->buf = new_buf; \
            svg->size_remaining += svg->size; \
            svg->size += svg->size; \
        } else { \
            svg->size_remaining -= req; \
            svg->cur += req; \
            break; \
        } \
    } while (1); \
} while (0)

KeyBounds get_key_bounds(Config* config, char character) {
    switch (character) {
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
    return get_random_point_in_bounds((KeyBounds) {
            .x = (current_key.x + (current_key.width / 2)) - (scale.x / 2),
            .y = (current_key.y + (current_key.height / 2)) - (scale.y / 2),
            .width = scale.x,
            .height = scale.y
    });
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

typedef struct {
    size_t size;
    size_t size_remaining;
    char *buf;
    char *cur;
} svg;

void compute_curves(Config* config) {
    printf("<svg xmlns=\"http://www.w3.org/2000/svg\""
           " width=\"%dpx\""
           " height=\"%dpx\""
           " viewBox=\"0 0 %d %d\">\n", config->width, config->height, config->width, config->height);

    char* KEY_STYLE = "fill:white;stroke:blue;stroke-width:3;";
    int key_count = 26;

    for (int i = 0; i < key_count; i++) {

        /*
         * TODO
         * I can't remember if modern C standards guarantee that
         * sequential characters are adjacent in terms of integer
         * value (i.e. that 'a' + 1 == 'b'). Should probably double-
         * check this.
         */
        svg_key(&svg, config, 'a' + i);
    }
    svg_key(&svg, config, ' ');

    int l = strlen(word);
    Point* key_locations = malloc(sizeof(Point) * l);

    for (int i = 0; i < l; i++) {
        if (l == 1) {
            break;
        }
        if ((i == 0 && l > 1)) {
            get_random_point_on_current_key(get_key_bounds(config, sentence[i]));
        }
        key_locations[i] = get_random_point_on_next_key(
                get_key_bounds(config, sentence[i]),
                get_key_bounds(config, sentence[i + 1])
        );
    }

    svg_quadratic_bezier(&svg, l, key_locations);

    svg_end(&svg);
    svg_write_to_file(&svg, out_fp);

    free(svg_buf);
    free(key_locations);
}
