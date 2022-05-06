//
// Created by liam on 5/5/22.
//

#include "drawing.h"

#define CVC(n)          (((float)(n)/255))


// Draw a circle
void draw_dot(cairo_t *cr, point_t p, int line_width, int radius, color_t line_color, color_t fill_color)
{
        cairo_set_source_rgb(cr, line_color.r, line_color.g, line_color.b);
        cairo_set_line_width(cr, line_width);
        //cairo_translate(cr, X_OFFSET, Y_OFFSET);
        cairo_arc(cr, p.x * CAIRO_SETTINGS_s.scale.x, p.y * CAIRO_SETTINGS_s.scale.y, radius, 0, 2 * M_PI);
        cairo_stroke_preserve(cr);

        cairo_set_source_rgb(cr, fill_color.r, fill_color.g, fill_color.b);
        cairo_fill(cr);
}


// Draw a vector
void draw_vector(cairo_t *cr, vector_t v, int width, color_t color)
{
        cairo_set_source_rgb(cr, color.r, color.g, color.b);
        cairo_set_line_width(cr, width);
        cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
        cairo_move_to(cr, v.p1.x * CAIRO_SETTINGS_s.scale.x, v.p1.y * CAIRO_SETTINGS_s.scale.y);
        cairo_line_to(cr, v.p2.x * CAIRO_SETTINGS_s.scale.x, v.p2.y * CAIRO_SETTINGS_s.scale.y);
        cairo_stroke(cr);
}

// Write text
void write_text(cairo_t *cr, point_t p, color_t color, char str[], int font_size)
{
        cairo_set_source_rgb(cr, color.r, color.g, color.b);
        cairo_set_font_size(cr, font_size);
        cairo_select_font_face(cr, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_move_to(cr, p.x * CAIRO_SETTINGS_s.scale.x, p.y * CAIRO_SETTINGS_s.scale.y);
        cairo_show_text(cr, str);
}


// Initialize Cairo
cairo_objects_t init_cairo(void)
{
        // Create cairo surface
        cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 2560, 1440);
        cairo_t *cr = cairo_create(surface);

        cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_NEAREST);
        // Set origin
        cairo_move_to(cr, 0, 0);



        return (cairo_objects_t){.surface = &surface, .cr = &cr};
}


const colors_t COLORS_s = {{CVC(0x00), CVC(0x00), CVC(0x00)},       // Black
                           {CVC(0xC3), CVC(0xC3), CVC(0xC3)},       // Silver
                           {CVC(0xFF), CVC(0xFF), CVC(0xFF)},       // White
                           {CVC(0xFF), CVC(0x00), CVC(0x00)},       // Red
                           {CVC(0x00), CVC(0x00), CVC(0xFF)},       // Blue
                           {CVC(0x00), CVC(0xFF), CVC(0x00)},       // Green
                           {CVC(0xFF), CVC(0xFF), CVC(0x00)},       // Yellow
};

struct cairo_settings_s CAIRO_SETTINGS_s = {.scale=(fpoint_t){.x = 1.0f, .y = 1.0f}, .offset=(point_t){.x = 0, .y = 0}};
