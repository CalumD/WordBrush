#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "wordbrush.h"
#include "debug.h"

#define key(character, x_point, y_point) \
    case character:\
        return (KeyBounds) {\
            .x = (x_point * KEY_WIDTH_PERCENTAGE * config->width),\
            .y = (y_point * KEY_HEIGHT_PERCENTAGE * config->height),\
            .width = KEY_WIDTH_PERCENTAGE * config->width,\
            .height = KEY_HEIGHT_PERCENTAGE * config->height\
        }

KeyBounds getCharacterOffset(Config *config, char character) {
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

Point getRandomPointOnKey(KeyBounds key) {
    // Find the centre point of key
    Point ret = {
            .x=(key.x + (key.width / 2)),
            .y=(key.y + (key.height / 2))
    };

    // TODO Choose direction to move in

    // TODO Move some percentage of the size of the key in that direction

    // Return the resultant point.
    return ret;
}

Point magnify(Point p, float a) {
    return (Point) {
        .x = p.x * a,
        .y = p.y * a
    };
}

Point add_points(Point a, Point b) {
    return (Point) {
        .x = a.x + b.x,
        .y = a.y + b.y
    };
}

Point reflect_control_point(Point cp, Point p) {
    return (Point) {
        .x = (2 * p.x) - cp.x,
        .y = (2 * p.y) - cp.y
    };
}

Point interpolate(Point a, Point b, float a_weight) {
    float b_weight = 1. - a_weight;

    Point a_weighted = magnify(a, a_weight);
    Point b_weighted = magnify(b, b_weight);

    return add_points(a_weighted, b_weighted);
}

Point find_next_control_point(Point prev_cp, Point cur_p) {
    Point reflected_cp = reflect_control_point(prev_cp, cur_p);
    return interpolate(cur_p, reflected_cp, 0.8);
}

void computeCurves(Config *config) {
    debug("In the compute curves method -------------\n");
    debug("Input Path: %s\n", config->inputFilePath);
    debug("Output Path: %s\n", config->outputFilePath);
    debug("Multi-file Output: %s\n", config->multiFileOutput ? "true" : "false");
    debug("\n\n");

    printf("<svg xmlns=\"http://www.w3.org/2000/svg\""
           " width=\"%dpx\""
           " height=\"%dpx\""
           " viewBox=\"0 0 %d %d\">\n", config->width, config->height, config->width, config->height);

    char *KEY_STYLE = "fill:white;stroke:blue;stroke-width:3;";

    for (int i = 0; i < 26; i++) {
        KeyBounds bounds = getCharacterOffset(config, 'a' + i);
        printf("<rect x=\"%f\" y=\"%f\" rx=\"10\" ry=\"10\" width=\"%f\" "
                "height=\"%f\" style=\"%s\"/>\n",
               bounds.x, bounds.y, bounds.width, bounds.height, KEY_STYLE);
        getRandomPointOnKey(bounds);
    }
    KeyBounds bounds = getCharacterOffset(config, ' ');
    printf("<rect x=\"%f\" y=\"%f\" rx=\"10\" ry=\"10\" width=\"%f\" height=\"%f\" style=\"%s\"/>\n",
           bounds.x, bounds.y, bounds.width, bounds.height, KEY_STYLE);
    getRandomPointOnKey(bounds);

    const char *sentence = "gittable";

    int l = strlen(sentence);
    Point *key_locations = malloc(sizeof(Point) * l);
    for (int i = 0; i < l; i++) {
        key_locations[i] = getRandomPointOnKey(getCharacterOffset(config, sentence[i]));
    }

    Point fst = key_locations[0];
    Point snd = key_locations[1];

    Point cp = fst;

    printf("<path d=\"M%f,%f Q%f,%f %f,%f",
            fst.x, fst.y,
            cp.x, cp.y,
            snd.x, snd.y);

    for (int i = 2; i < l; i++) {
        Point cur_p = key_locations[i];
        cp = find_next_control_point(cp, key_locations[i-1]);
        printf(" Q%f,%f %f,%f", cp.x, cp.y, cur_p.x, cur_p.y);
    }

    printf("\" stroke=\"black\" stroke-width=\"5\" fill=\"none\" /></svg>\n");
}
