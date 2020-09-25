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

KeyBounds get_key_bounds(Config *config, char character) {
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

Point get_random_point_on_key(KeyBounds key) {
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

typedef struct {
    size_t size;
    size_t size_remaining;
    char *buf;
    char *cur;
} svg;

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

void svg_start(svg *svg, int width, int height) {
    char *format =
        "<svg xmlns='%s' width='%dpx' height='%dpx'"
        " viewBox='0 0 %d %d'>\n";

    char *XML_NAMESPACE = "http://www.w3.org/2000/svg";

    add_to_svg(svg, format, XML_NAMESPACE, width, height, width, height);
}

void svg_rect(svg *svg, float x, float y, float rx, float ry, float width,
              float height, char *style) {

    char *format =
        "<rect x='%f' y='%f' rx='%f' ry='%f' width='%f' height='%f'"
        " style='%s'/>\n";

    add_to_svg(svg, format, x, y, rx, ry, width, height, style ? style : "");
}

void svg_key(svg *svg, Config *cfg, char c) {
    char *KEY_STYLE = "fill:white;stroke:blue;stroke-width:3;";
    float KEY_ROUNDEDNESS = 10.;

    KeyBounds bounds = get_key_bounds(cfg, c);
    svg_rect(svg, bounds.x, bounds.y, KEY_ROUNDEDNESS, KEY_ROUNDEDNESS,
             bounds.width, bounds.height, KEY_STYLE);
}

void svg_quadratic_bezier(svg *svg, int n, Point *ps) {
    char *PATH_START = "<path d='M%f,%f Q%f,%f %f,%f";
    char *PATH_REPEAT = " Q%f,%f %f,%f";
    char *PATH_END = "' stroke='%s' stroke-width='%i' fill='%s' />";

    char *PATH_STROKE = "black";
    int PATH_STROKE_WIDTH = 5;
    char *PATH_FILL = "none";

    Point ctrl_p = ps[0];

    add_to_svg(svg, PATH_START,
               ps[0].x, ps[0].y,
               ctrl_p.x, ctrl_p.y,
               ps[1].x, ps[1].y);

    for (int i = 2; i < n; i++) {
        Point cur_p = ps[i];
        ctrl_p = find_next_control_point(ctrl_p, ps[i - 1]);

        add_to_svg(svg, PATH_REPEAT,
                   ctrl_p.x, ctrl_p.y,
                   cur_p.x, cur_p.y);
    }

    add_to_svg(svg, PATH_END, PATH_STROKE, PATH_STROKE_WIDTH, PATH_FILL);
}

void svg_write_to_file(svg *svg, FILE *fp) {
    fwrite(svg->buf, 1, svg->size - svg->size_remaining, fp);
}

void svg_end(svg *svg) {
    add_to_svg(svg, "</svg>");
}

void svg_free(svg *svg) {
    free(svg->buf);
}

void compute_curves(Config* config, char* word, FILE* out_fp) {
    debug("In the compute curves method -------------\n");
    debug("Input Path: %s\n", config->inputFilePath);
    debug("Output Dir: %s\n", config->outputFilePath);
    debug("Multi-file Output: %s\n", config->multiFileOutput ? "true" : "false");
    debug("\n\n");

    size_t SVG_BUF_START_SIZE = 1024;
    char *svg_buf = malloc(SVG_BUF_START_SIZE);
    svg svg = {
        .size = SVG_BUF_START_SIZE,
        .size_remaining = SVG_BUF_START_SIZE,
        .buf = svg_buf,
        .cur = svg_buf
    };

    svg_start(&svg, config->width, config->height);

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
    Point *key_locations = malloc(sizeof(Point) * l);
    for (int i = 0; i < l; i++) {
        key_locations[i] =
            get_random_point_on_key(get_key_bounds(config, word[i]));
    }

    svg_quadratic_bezier(&svg, l, key_locations);

    svg_end(&svg);
    svg_write_to_file(&svg, out_fp);

    free(svg_buf);
    free(key_locations);
}
