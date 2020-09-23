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
    KeyBounds subdivision_bound = (KeyBounds) {
            .x = (current_key.x + (current_key.width / 2)) - (scale.x / 2),
            .y = (current_key.y + (current_key.height / 2)) - (scale.y / 2),
            .width = scale.x,
            .height = scale.y
    };
//    printf("<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" style=\"fill:none;stroke:black;stroke-width:1;\"/>\n",
//           subdivision_bound.x, subdivision_bound.y, subdivision_bound.width, subdivision_bound.height);
    Point p = get_random_point_in_bounds(subdivision_bound);
//    printf("<circle cx=\"%f\" cy=\"%f\" r=\"3\" style=\"fill:red\"/>\n", p.x, p.y);
    return p;
}

Point get_random_point_on_next_key(KeyBounds current_key, KeyBounds next_key) {
    // Find the centre point of current Key
    Point current = {
            .x = (current_key.x + (current_key.width / 2)),
            .y = (current_key.y + (current_key.height / 2))
    };
//    printf("<circle cx=\"%f\" cy=\"%f\" r=\"2\"/>\n", current.x, current.y);

    // Find the centre point of next key
    Point next = {
            .x=(next_key.x + (next_key.width / 2)),
            .y=(next_key.y + (next_key.height / 2))
    };
//    printf("<circle cx=\"%f\" cy=\"%f\" r=\"2\"/>\n", next.x, next.y);
    Point scale = {.x = next_key.width * KEY_ACTIVE_ZONE_PERCENTAGE, .y = next_key.height * KEY_ACTIVE_ZONE_PERCENTAGE};

    /*
     * Figure out which compass direction the old key is to the new one... using:
     *
     * 0 1 2
     * 7 8 3
     * 6 5 4
     */
    KeyBounds sub_division_bound;

    debug("Xc: %f, Yc: %f\n", current.x, current.y);
    debug("Xn: %f, Yn: %f\n", next.x, next.y);

    if (current.x < next.x && current.y < next.y) {             // 0
        debug("Choose 0\n");
        sub_division_bound = (KeyBounds) {.x = next.x - scale.x, .y = next.y - scale.y,
                .width = scale.x, .height = scale.y};
    } else if (current.x == next.x && current.y < next.y) {     // 1
        debug("Choose 1\n");
        sub_division_bound = (KeyBounds) {.x = next.x - (scale.x / 2), .y = next.y - scale.y,
                .width = scale.x, .height = scale.y};
    } else if (current.x > next.x && current.y < next.y) {      // 2
        debug("Choose 2\n");
        sub_division_bound = (KeyBounds) {.x = next.x, .y = next.y - scale.y,
                .width = scale.x, .height = scale.y};
    } else if (current.x > next.x && current.y == next.y) {     // 3
        debug("Choose 3\n");
        sub_division_bound = (KeyBounds) {.x = next.x, .y = next.y - (scale.y / 2),
                .width = scale.x, .height = scale.y};
    } else if (current.x > next.x && current.y > next.y) {      // 4
        debug("Choose 4\n");
        sub_division_bound = (KeyBounds) {.x = next.x, .y = next.y,
                .width = scale.x, .height = scale.y};
    } else if (current.x == next.x && current.y > next.y) {  // 5
        debug("Choose 5\n");
        sub_division_bound = (KeyBounds) {.x = next.x - (scale.x / 2), .y = next.y,
                .width = scale.x, .height = scale.y};
    } else if (current.x < next.x && current.y > next.y) {     // 6
        debug("Choose 6\n");
        sub_division_bound = (KeyBounds) {.x = next.x - scale.x, .y = next.y,
                .width = scale.x, .height = scale.y};
    } else if (current.x < next.x && current.y == next.y) {      // 7
        debug("Choose 7\n");
        sub_division_bound = (KeyBounds) {.x = next.x - scale.x, .y = next.y - (scale.y / 2),
                .width = scale.x, .height = scale.y};
    } else {
        debug("Choose 8\n");
        sub_division_bound = (KeyBounds) {.x = next.x - (scale.x / 2), .y = next.y - (scale.y / 2),
                .width = scale.x, .height = scale.y};
    }
//    printf("<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" style=\"fill:none;stroke:black;stroke-width:1;\"/>\n",
//           sub_division_bound.x, sub_division_bound.y, sub_division_bound.width, sub_division_bound.height);


    // Find then return a random point in the sub division bounds.
    Point p = get_random_point_in_bounds(sub_division_bound);
//    printf("<circle cx=\"%f\" cy=\"%f\" r=\"3\" style=\"fill:red\"/>\n", p.x, p.y);
    return p;
}

void compute_curves(Config* config) {
    debug("In the compute curves method -------------\n");
    debug("Input Path: %s\n", config->inputFilePath);
    debug("Output Path: %s\n", config->outputFilePath);
    debug("Multi-file Output: %s\n", config->multiFileOutput ? "true" : "false");
    debug("\n\n");

    printf("<svg xmlns=\"http://www.w3.org/2000/svg\""
           " width=\"%dpx\""
           " height=\"%dpx\""
           " viewBox=\"0 0 %d %d\">\n", config->width, config->height, config->width, config->height);

    char* KEY_STYLE = "fill:white;stroke:blue;stroke-width:3;";

    for (int i = 0; i < 26; i++) {
        KeyBounds bounds = get_key_bounds(config, 'a' + i);
        printf("<rect x=\"%f\" y=\"%f\" rx=\"10\" ry=\"10\" width=\"%f\" "
               "height=\"%f\" style=\"%s\"/>\n",
               bounds.x, bounds.y, bounds.width, bounds.height, KEY_STYLE);
    }
    KeyBounds bounds = get_key_bounds(config, ' ');
    printf("<rect x=\"%f\" y=\"%f\" rx=\"10\" ry=\"10\" width=\"%f\" height=\"%f\" style=\"%s\"/>\n",
           bounds.x, bounds.y, bounds.width, bounds.height, KEY_STYLE);

    const char* sentence = "hello";

    int l = strlen(sentence);
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
//
//    Point fst = key_locations[0];
//    Point snd = key_locations[1];
//
//    Point cp = fst;
//
//    printf("<path d=\"M%f,%f Q%f,%f %f,%f",
//           fst.x, fst.y,
//           cp.x, cp.y,
//           snd.x, snd.y);
//
//    for (int i = 2; i < l; i++) {
//        Point cur_p = key_locations[i];
//        cp = find_next_control_point(cp, key_locations[i - 1]);
//        printf(" Q%f,%f %f,%f", cp.x, cp.y, cur_p.x, cur_p.y);
//    }

    free(key_locations);
//    printf("\" stroke=\"black\" stroke-width=\"5\" fill=\"none\" /></svg>\n");
    printf("</svg>\n");
}
