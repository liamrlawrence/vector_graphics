//
// Created by liam on 5/5/22.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "subway.h"


// Function definitions //
static color_t str2color(char str[8]);



// Read input from file
void read_input(cairo_t *cr, subway_t *subway, char filename[64])
{
        typedef enum {
                NONE = -4,
                OFFSET = -3,
                BACKGROUND = -2,
                SCALE = -1,
                TEXT,
                STATION,
                LINE,
                STOP,
        } commands_e;
        commands_e command = NONE;
        int cmd_cnt[NUM_DRAW_COMMANDS] = {0};

        FILE *fp = fopen(filename, "r");
        size_t bufsize = TEXT_BUF_SIZE;
        char *buffer;


        // Initialize buffer
        buffer = (char *)malloc(bufsize * sizeof(char));
        if(buffer == NULL) {
                perror("Unable to allocate buffer");
                exit(1);
        }


        // Calculate the memory required for malloc
        while (getline(&buffer, &bufsize, fp) != EOF) {
                if (buffer[0] == '#') {
                        continue;
                }
                if (buffer[0] == '\n') {
                        command = NONE;
                        continue;
                }

                if (buffer[0] == '[') {
                        if (strncmp(buffer, "[TEXT]", 6) == 0)
                                command = TEXT;
                        else if (strncmp(buffer, "[STATION]", 9) == 0)
                                command = STATION;
                        else if (strncmp(buffer, "[LINE]", 6) == 0)
                                command = LINE;
                        else if (strncmp(buffer, "[STOP]", 6) == 0)
                                command = STOP;
                        else if (strncmp(buffer, "[OFFSET]", 8) == 0)
                                command = OFFSET;
                        else if (strncmp(buffer, "[BACKGROUND]", 12) == 0)
                                command = BACKGROUND;
                        else if (strncmp(buffer, "[SCALE]", 7) == 0)
                                command = SCALE;
                        else {
                                printf("Error malformed line: %s\n", buffer);
                                exit(1);
                        }
                } else if (command >= 0) {
                        cmd_cnt[command]++;
                }
        }


        // Malloc objects
        for (int i = 0; i < NUM_DRAW_COMMANDS; i++) {
                switch ((commands_e)i) {
                        case TEXT:
                                subway->texts = malloc(sizeof(text_t) * cmd_cnt[TEXT]);
                                break;

                        case STATION:
                                subway->stations = malloc(sizeof(station_t) * cmd_cnt[STATION]);
                                break;

                        case LINE:
                                subway->lines = malloc(sizeof(line_t) * cmd_cnt[LINE]);
                                break;

                        case STOP:
                                subway->stops = malloc(sizeof(stop_t) * cmd_cnt[STOP]);
                                break;

                        case OFFSET:
                        case BACKGROUND:
                        case SCALE:
                                break;

                        default:
                                printf("Error code %d in function: void read_input(subway_t *subway);", i);
                                exit(1);
                }
        }


        // Run commands
        text_t *text_ptr = NULL;
        station_t *station_ptr = NULL;
        line_t *line_ptr = NULL;
        stop_t *stop_ptr = NULL;
        char color1_buf[8] = "";
        char color2_buf[8] = "";
        color_t color_buf = {0};
        rewind(fp);
        command = NONE;
        while (getline(&buffer, &bufsize, fp) != EOF) {
                if (buffer[0] == '#') {
                        continue;
                }
                if (buffer[0] == '\n') {
                        command = NONE;
                        continue;
                }

                if (buffer[0] == '[') {
                        if (strncmp(buffer, "[TEXT]", 6) == 0)
                                command = TEXT;
                        else if (strncmp(buffer, "[STATION]", 9) == 0)
                                command = STATION;
                        else if (strncmp(buffer, "[LINE]", 6) == 0)
                                command = LINE;
                        else if (strncmp(buffer, "[STOP]", 6) == 0)
                                command = STOP;
                        else if (strncmp(buffer, "[OFFSET]", 8) == 0)
                                command = OFFSET;
                        else if (strncmp(buffer, "[BACKGROUND]", 12) == 0)
                                command = BACKGROUND;
                        else if (strncmp(buffer, "[SCALE]", 7) == 0)
                                command = SCALE;
                        else {
                                printf("Error malformed line: %s\n", buffer);
                                exit(1);
                        }
                } else {
                        switch (command) {
                                case TEXT:
                                        text_ptr = &subway->texts[subway->ntexts++];
                                        sscanf(buffer, "(%d, %d) \"%49[^\"]\" color=%s\n font_size=%d\n",
                                               &text_ptr->p.x, &text_ptr->p.y,
                                               text_ptr->text, color1_buf,
                                               &text_ptr->font_size);
                                        text_ptr->color = str2color(color1_buf);
                                        break;

                                case STATION:
                                        station_ptr = &subway->stations[subway->nstations++];
                                        sscanf(buffer,
                                               "(%d, %d) radius=%d line_width=%d line_color=%s fill_color=%s label=\"%49[^\"]\" (%d, %d) font_size=%d\n",
                                               &station_ptr->p.x, &station_ptr->p.y,
                                               &station_ptr->radius, &station_ptr->line_width,
                                               color1_buf, color2_buf,
                                               station_ptr->label,
                                               &station_ptr->lp.x, &station_ptr->lp.y,
                                               &station_ptr->font_size);
                                        station_ptr->line_color = str2color(color1_buf);
                                        station_ptr->fill_color = str2color(color2_buf);
                                        break;

                                case LINE:
                                        line_ptr = &subway->lines[subway->nlines++];
                                        sscanf(buffer, "<(%d, %d), (%d, %d)> width=%d color=%s label=\"%49[^\"]\" (%d, %d) font_size=%d\n",
                                               &line_ptr->v.p1.x, &line_ptr->v.p1.y,
                                               &line_ptr->v.p2.x, &line_ptr->v.p2.y,
                                               &line_ptr->width, color1_buf,
                                               line_ptr->label,
                                               &line_ptr->lp.x, &line_ptr->lp.y,
                                               &line_ptr->font_size);
                                        line_ptr->color = str2color(color1_buf);
                                        break;

                                case STOP:
                                        stop_ptr = &subway->stops[subway->nstops++];
                                        sscanf(buffer,
                                               "(%d, %d) radius=%d line_width=%d line_color=%s fill_color=%s label=\"%49[^\"]\" (%d, %d) font_size=%d\n",
                                               &stop_ptr->p.x, &stop_ptr->p.y,
                                               &stop_ptr->radius, &stop_ptr->line_width,
                                               color1_buf, color2_buf,
                                               stop_ptr->label,
                                               &stop_ptr->lp.x, &stop_ptr->lp.y,
                                               &stop_ptr->font_size);
                                        stop_ptr->line_color = str2color(color1_buf);
                                        stop_ptr->fill_color = str2color(color2_buf);
                                        break;

                                case OFFSET:
                                        sscanf(buffer, "(%d, %d)\n", &CAIRO_SETTINGS_s.offset.x, &CAIRO_SETTINGS_s.offset.y);
                                        cairo_translate(cr,
                                                        CAIRO_SETTINGS_s.offset.x,      // Used to scale these, was too tricky
                                                        CAIRO_SETTINGS_s.offset.y);     // from the end user's perspective
                                        break;

                                case BACKGROUND:
                                        sscanf(buffer, "(%f, %f, %f)\n", &color_buf.r, &color_buf.g, &color_buf.b);
                                        if (color_buf.r > 1.0 || color_buf.g > 1.0 || color_buf.b > 1.0) {
                                                color_buf.r = CVC(color_buf.r);
                                                color_buf.g = CVC(color_buf.g);
                                                color_buf.b = CVC(color_buf.b);
                                        }
                                        cairo_set_source_rgb(cr, color_buf.r, color_buf.g, color_buf.b);
                                        cairo_paint(cr);
                                        break;

                                case SCALE:
                                        sscanf(buffer, "(%f, %f)\n", &CAIRO_SETTINGS_s.scale.x, &CAIRO_SETTINGS_s.scale.y);
                                        break;

                                default:
                                        printf("Error code %d in function: void read_input(subway_t *subway);", command);
                                        exit(1);
                        }
                }
        }

        // Cleanup memory
        free(buffer);
        return;
}



// Draw all text and shapes to the Cairo surface
void render_subway(cairo_t *cr, subway_t *subway)
{
        // Render text
        for (int i = 0; i < subway->ntexts; i++)
                write_text(cr, (point_t){.x = subway->texts[i].p.x, .
                y = subway->texts[i].p.y}, subway->texts[i].color, subway->texts[i].text, subway->texts[i].font_size);
        for (int i = 0; i < subway->nstations; i++)
                write_text(cr, (point_t){.x = subway->stations[i].lp.x, .y = subway->stations[i].lp.y}, COLORS_s.black, subway->stations[i].label, subway->stations[i].font_size);
        for (int i = 0; i < subway->nlines; i++)
                write_text(cr, (point_t){.x = subway->lines[i].lp.x, .y = subway->lines[i].lp.y}, COLORS_s.black, subway->lines[i].label, subway->lines[i].font_size);
        for (int i = 0; i < subway->nstops; i++)
                write_text(cr, (point_t){.x = subway->stops[i].lp.x, .y = subway->stops[i].lp.y}, COLORS_s.black, subway->stops[i].label, subway->stops[i].font_size);

        // Render lines
        for (int i = 0; i < subway->nlines; i++) {
                draw_vector(
                        cr,
                        (vector_t){.p1 = (point_t){.x = subway->lines[i].v.p1.x, .y = subway->lines[i].v.p1.y},
                                   .p2 = (point_t){.x = subway->lines[i].v.p2.x, .y = subway->lines[i].v.p2.y}},
                        subway->lines[i].width,
                        subway->lines[i].color);
        }

        // Render stations
        for (int i = 0; i < subway->nstations; i++) {

                draw_dot(
                        cr,
                        (point_t){.x = subway->stations[i].p.x, .y = subway->stations[i].p.y},
                        subway->stations[i].line_width,
                        subway->stations[i].radius,
                        subway->stations[i].line_color,
                        subway->stations[i].fill_color);
        }

        // Render stops
        for (int i = 0; i < subway->nstops; i++) {

                draw_dot(
                        cr,
                        (point_t){.x = subway->stops[i].p.x, .y = subway->stops[i].p.y},
                        subway->stops[i].line_width,
                        subway->stops[i].radius,
                        subway->stops[i].line_color,
                        subway->stops[i].fill_color);
        }
}



// Helper functions //
static color_t str2color(char str[8])
{
        if (strncmp(str, "BLACK", 5) == 0)
                return COLORS_s.black;
        else if (strncmp(str, "SILVER", 6) == 0)
                return COLORS_s.silver;
        else if (strncmp(str, "WHITE", 5) == 0)
                return COLORS_s.white;
        else if (strncmp(str, "RED", 3) == 0)
                return COLORS_s.red;
        else if (strncmp(str, "BLUE", 4) == 0)
                return COLORS_s.blue;
        else if (strncmp(str, "GREEN", 5) == 0)
                return COLORS_s.green;
        else if (strncmp(str, "YELLOW", 6) == 0)
                return COLORS_s.yellow;

        printf("Error unknown color: %s\n", str);
        exit(1);
}
