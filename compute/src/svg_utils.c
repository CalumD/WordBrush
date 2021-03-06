#include "svg_utils.h"

svg* svg_init() {
    //Create the initial buffer for the svg
    char *svg_buf = malloc(SVG_BUF_START_SIZE);
    svg* svg = malloc(sizeof(struct svg));

    //setup default values
    *svg = (struct svg){
            .size_available = SVG_BUF_START_SIZE,
            .size_remaining = SVG_BUF_START_SIZE,
            .start_of_svg = svg_buf,
            .current_point_in_svg = svg_buf
    };

    return svg;
}

void svg_start(svg* svg, long xOffset, long yOffset, long width, long height) {
    // Define the boiler plate stuff to wrap the SVG data in
    char* boiler_plate ="<svg xmlns='%s' x='%dpx' y='%dpx' width='%dpx' height='%dpx' viewBox='0 0 %d %d'>\n";
    char* XML_NAMESPACE = "http://www.w3.org/2000/svg";

    // Write the boiler plate into the svg buffer
    add_to_svg(svg, boiler_plate, XML_NAMESPACE, xOffset, yOffset, width, height, width, height);
}

void svg_rect(svg* svg, float x, float y, float rx, float ry, float width,
              float height, char* style) {

    char* format = "<rect x='%f' y='%f' rx='%f' ry='%f' width='%f' height='%f' style='%s'/>\n";

    add_to_svg(svg, format, x, y, rx, ry, width, height, style ? style : "");
}

void svg_key(svg* svg, Config* cfg, char c) {
    char* KEY_STYLE = "fill:white;stroke:blue;stroke-width:3;";
    float KEY_ROUNDEDNESS = 10.0f;

    KeyBounds bounds = get_key_bounds(cfg, c);
    svg_rect(svg, bounds.x, bounds.y, KEY_ROUNDEDNESS, KEY_ROUNDEDNESS,
             bounds.width, bounds.height, KEY_STYLE);
}

void svg_quadratic_bezier(svg* svg, int n, Point* ps) {
    char* PATH_START = "<path d='M%f,%f Q%f,%f %f,%f";
    char* PATH_REPEAT = " %f,%f %f,%f";
    char* PATH_END = "' stroke='%s' stroke-width='%i' fill='%s' stroke-linecap='round'/>";

    char* PATH_STROKE = "black";
    int PATH_STROKE_WIDTH = 5;
    char* PATH_FILL = "none";

    Point ctrl_p = ps[0];

    if (n == 1) {
        add_to_svg(svg, PATH_START,
                   ps[1].x, ps[1].y,
                   ctrl_p.x, ctrl_p.y,
                   ps[1].x, ps[1].y);
    } else {
        add_to_svg(svg, PATH_START,
                   ps[0].x, ps[0].y,
                   ctrl_p.x, ctrl_p.y,
                   ps[1].x, ps[1].y);
    }

    for (int i = 2; i < n; i++) {
        Point cur_p = ps[i];
        ctrl_p = find_next_control_point(ctrl_p, ps[i - 1]);

        add_to_svg(svg, PATH_REPEAT,
                   ctrl_p.x, ctrl_p.y,
                   cur_p.x, cur_p.y);
    }

    add_to_svg(svg, PATH_END, PATH_STROKE, PATH_STROKE_WIDTH, PATH_FILL);
}

void svg_flush_to_file(svg* svg, FILE* fp) {
    fwrite(svg->start_of_svg, 1, svg->size_available - svg->size_remaining, fp);
    svg->size_remaining = svg->size_available;
    svg->current_point_in_svg = svg->start_of_svg;
}

void svg_end(svg* svg) {
    add_to_svg(svg, "</svg>");
}

void svg_free(svg* svg) {
    free(svg->start_of_svg);
    free(svg);
}
