//
// Created by liam on 5/5/22.
//

#ifndef NETHER_SUBWAY_SUBWAY_H
#define NETHER_SUBWAY_SUBWAY_H

#include "drawing.h"

#define NUM_DRAW_COMMANDS       4
#define TEXT_BUF_SIZE           128



typedef struct {
        point_t p;
        char text[TEXT_BUF_SIZE];
        int font_size;
        color_t color;
} text_t;


typedef struct {
        vector_t v;
        int width;
        color_t color;
        char label[TEXT_BUF_SIZE];
        point_t lp;
        int font_size;
} line_t;


typedef struct {
        point_t p;
        int radius;
        int line_width;
        color_t line_color;
        color_t fill_color;
        char label[TEXT_BUF_SIZE];
        point_t lp;
        int font_size;
} stop_t;


typedef struct {
        point_t p;
        int radius;
        int line_width;
        color_t line_color;
        color_t fill_color;
        char label[TEXT_BUF_SIZE];
        point_t lp;
        int font_size;
} station_t;


typedef struct {
        text_t *texts;
        station_t *stations;
        line_t *lines;
        stop_t *stops;
        int ntexts;
        int nstations;
        int nlines;
        int nstops;
} subway_t;



void read_input(cairo_t *cr, subway_t *subway, char filename[256]);
void render_subway(cairo_t *cr, subway_t *subway);
void free_subway(subway_t *subway);


#endif //NETHER_SUBWAY_SUBWAY_H
