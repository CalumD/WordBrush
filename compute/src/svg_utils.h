#ifndef WORDBRUSH_SVG_UTILS_H
#define WORDBRUSH_SVG_UTILS_H

#include <stdlib.h>

typedef struct svg{
    size_t size_available;
    size_t size_remaining;
    char* start_of_svg;
    char* current_point_in_svg;
} svg;

#include "main.h"

#define SVG_BUF_START_SIZE 1024

#define add_to_svg(svg, ...) do { \
    do { \
        size_t req = snprintf((svg)->current_point_in_svg, (svg)->size_remaining, __VA_ARGS__); \
        if (req > (svg)->size_remaining) { \
            char* new_buf = realloc((svg)->start_of_svg, (svg)->size_available * 2); \
            (svg)->current_point_in_svg += new_buf - (svg)->start_of_svg; \
            (svg)->start_of_svg = new_buf; \
            (svg)->size_remaining += (svg)->size_available; \
            (svg)->size_available += (svg)->size_available; \
        } else { \
            (svg)->size_remaining -= req; \
            (svg)->current_point_in_svg += req; \
            break; \
        } \
    } while (1); \
} while (0)

svg* svg_start(long xOffset, long yOffset, long width, long height);

void svg_rect(svg* svg, float x, float y, float rx, float ry, float width,float height, char* style);

void svg_key(svg* svg, Config* cfg, char c);

void svg_quadratic_bezier(svg* svg, int n, Point* ps);

void svg_write_to_file(svg* svg, FILE* fp);

void svg_end(svg* svg);

void svg_free(svg* svg);


#endif //WORDBRUSH_SVG_UTILS_H
