//
// Created by liam on 5/5/22.
//

#ifndef NETHER_SUBWAY_DRAWING_H
#define NETHER_SUBWAY_DRAWING_H

#include <math.h>
#include <cairo.h>

#define CVC(n)          (((float)(n)/255))      // Convert Color



typedef struct {
        float r;
        float g;
        float b;
} color_t;

typedef struct {
        color_t black;
        color_t silver;
        color_t white;
        color_t red;
        color_t blue;
        color_t green;
        color_t yellow;
} colors_t;

typedef struct {
        cairo_surface_t *surface;
        cairo_t *cr;
} cairo_objects_t;

typedef struct {
        int x;
        int y;
} point_t;

typedef struct {
        float x;
        float y;
} fpoint_t;


typedef struct {
        point_t p1;
        point_t p2;
} vector_t;




void draw_dot(cairo_t *cr, point_t p, int line_width, int radius, color_t line_color, color_t fill_color);
void draw_vector(cairo_t *cr, vector_t v, int width, color_t color);
void write_text(cairo_t *cr, point_t p, color_t color, char str[], int font_size);
cairo_objects_t init_cairo(void);


extern const colors_t COLORS_s;
extern struct cairo_settings_s {
        fpoint_t scale;
        point_t offset;
        float alpha;
} CAIRO_SETTINGS_s;


#endif //NETHER_SUBWAY_DRAWING_H
