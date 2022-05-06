#include <stdio.h>
#include <stdlib.h>
#include "subway.h"


int main (int argc, char **argv)
{
        if (argc != 2) {
                printf("Error there should be exactly one argument, got %d", argc-1);
                exit(1);
        }

        // Init
        cairo_objects_t co = init_cairo();
        cairo_t *cr = *co.cr;
        cairo_surface_t *surface = *co.surface;
        subway_t subway = {NULL, NULL, NULL, NULL, 0, 0, 0, 0};

        // Read input and render image
        read_input(cr, &subway, argv[1]);
        render_subway(cr, &subway);

        // Save the image to the disk
        cairo_destroy(cr);
        cairo_surface_write_to_png(surface, "hello.png");
        cairo_surface_destroy(surface);

        return 0;
}
