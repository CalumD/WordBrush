#ifndef WORDBRUSH_SVG_UTILS_H
#define WORDBRUSH_SVG_UTILS_H

#include <stdlib.h>

typedef struct svg{
    size_t size_available;
    size_t size_remaining;
    char* buf;
    char* cur;
} svg;

#include "main.h"

#define add_to_svg(svg, ...) do { \
    do { \
        size_t req = snprintf((svg)->cur, (svg)->size_remaining, __VA_ARGS__); \
        if (req > (svg)->size_remaining) { \
            char* new_buf = realloc((svg)->buf, (svg)->size_available * 2); \
            (svg)->cur += new_buf - (svg)->buf; \
            (svg)->buf = new_buf; \
            (svg)->size_remaining += (svg)->size_available; \
            (svg)->size_available += (svg)->size_available; \
        } else { \
            (svg)->size_remaining -= req; \
            (svg)->cur += req; \
            break; \
        } \
    } while (1); \
} while (0);

void svg_start(svg* svg, int width, int height);

void svg_rect(svg* svg, float x, float y, float rx, float ry, float width,float height, char* style);

void svg_key(svg* svg, Config* cfg, char c);

void svg_quadratic_bezier(svg* svg, int n, Point* ps);

void svg_write_to_file(svg* svg, FILE* fp);

void svg_end(svg* svg);

void svg_free(svg* svg);


#endif //WORDBRUSH_SVG_UTILS_H
