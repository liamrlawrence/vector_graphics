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


// Static Structs //
typedef enum {
        NONE = -5,
        OFFSET = -4,
        ALPHA = -3,
        BACKGROUND = -2,
        SCALE = -1,
        TEXT,
        STATION,
        LINE,
        STOP,
} commands_e;


// Read input from file
void read_input(cairo_t *cr, subway_t *subway, char filename[256])
{
        commands_e command = NONE;
        int cmd_cnt[NUM_DRAW_COMMANDS] = {0};

        FILE *fp = fopen(filename, "r");
        size_t bufsize = TEXT_BUF_SIZE;
        char *buffer;


        // Initialize buffer
        buffer = (char *)malloc(sizeof(char) * bufsize);
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
                        else if (strncmp(buffer, "[ALPHA]", 7) == 0)
                                command = ALPHA;
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
                                subway->texts = (text_t *)malloc(sizeof(text_t) * cmd_cnt[TEXT]);
                                if(subway->texts == NULL) {
                                        perror("Unable to allocate buffer subway->texts");
                                        exit(1);
                                }
                                break;

                        case STATION:
                                subway->stations = (station_t *)malloc(sizeof(station_t) * cmd_cnt[STATION]);
                                if(subway->stations == NULL) {
                                        perror("Unable to allocate buffer subway->stations");
                                        exit(1);
                                }
                                break;

                        case LINE:
                                subway->lines = (line_t *)malloc(sizeof(line_t) * cmd_cnt[LINE]);
                                if(subway->lines == NULL) {
                                        perror("Unable to allocate buffer subway->lines");
                                        exit(1);
                                }
                                break;

                        case STOP:
                                subway->stops = (stop_t *)malloc(sizeof(stop_t) * cmd_cnt[STOP]);
                                if(subway->stops == NULL) {
                                        perror("Unable to allocate buffer subway->stops");
                                        exit(1);
                                }
                                break;

                        case OFFSET:
                        case ALPHA:
                        case BACKGROUND:
                        case SCALE:
                                break;

                        default:
                                printf("Error code %d in function: void read_input();", i);
                                exit(1);
                }
        }


        // Process commands
        text_t *text_ptr = NULL;
        station_t *station_ptr = NULL;
        line_t *line_ptr = NULL;
        stop_t *stop_ptr = NULL;
        color_t color_buf = {0};
        char color1_buf[8] = "";
        char color2_buf[8] = "";
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
                        else if (strncmp(buffer, "[ALPHA]", 7) == 0)
                                command = ALPHA;
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

                                case ALPHA:
                                        sscanf(buffer, "(%f)\n", &CAIRO_SETTINGS_s.alpha);
                                        break;

                                case BACKGROUND:
                                        sscanf(buffer, "(%f, %f, %f)\n", &color_buf.r, &color_buf.g, &color_buf.b);
                                        if (color_buf.r > 1.0 || color_buf.g > 1.0 || color_buf.b > 1.0) {
                                                color_buf.r = CVC(color_buf.r);
                                                color_buf.g = CVC(color_buf.g);
                                                color_buf.b = CVC(color_buf.b);
                                        }
                                        cairo_set_source_rgba(cr, color_buf.r, color_buf.g, color_buf.b, CAIRO_SETTINGS_s.alpha);
                                        cairo_paint(cr);
                                        break;

                                case SCALE:
                                        sscanf(buffer, "(%f, %f)\n", &CAIRO_SETTINGS_s.scale.x, &CAIRO_SETTINGS_s.scale.y);
                                        break;

                                default:
                                        printf("Error code %d in function: void read_input();", command);
                                        exit(1);
                        }
                }
        }

        // Cleanup memory
        fclose(fp);
        free(buffer);
}


// Draw all text and shapes to the Cairo surface
void render_subway(cairo_t *cr, subway_t *subway)
{
        // Render text
        for (int i = 0; i < subway->ntexts; i++)
                write_text(cr, (point_t){.x = subway->texts[i].p.x, .y = subway->texts[i].p.y}, subway->texts[i].color, subway->texts[i].text, subway->texts[i].font_size);
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


// Free Subway's allocated memory
void free_subway(subway_t *subway)
{
        for (int i = 0; i < NUM_DRAW_COMMANDS; i++) {
                switch ((commands_e)i) {
                        case TEXT:
                                free(subway->texts);
                                break;

                        case STATION:
                                free(subway->stations);
                                break;

                        case LINE:
                                free(subway->lines);
                                break;

                        case STOP:
                                free(subway->stops);
                                break;

                        case OFFSET:
                        case ALPHA:
                        case BACKGROUND:
                        case SCALE:
                                break;

                        default:
                                printf("Error code %d in function: void free_subway();", i);
                                exit(1);
                }
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
